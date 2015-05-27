// logger.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2006

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Helpers for logging messages & errors.


#include "logger.h"

#include <stdio.h>
#include <stdarg.h>


namespace logger
{
	// Function pointer to log callback.
	static void (*s_log_callback)(log_type e, const char* message) = NULL;

	void	register_log_callback(void (*callback)(log_type e, const char* message))
	// The host app can use this to install a function to receive log
	// & error messages from gameswf.
	//
	// Pass in NULL to inhibit logging of messages & errors.
	{
		s_log_callback = callback;
	}


	void	register_log_callback(void (*callback)(log_type e, const char* message));


	void standard_logger(log_type e, const char* message)
	{
		if (e == ERROR) {
			fprintf(stderr, message);
		} else {
			// NORMAL or VERBOSE
			printf(message);
		}
	}

	
	// Log to stdout/stderr.
	void set_standard_log_handlers()
	{
		register_log_callback(standard_logger);
	}


	// Toggle this to enable verbose logging by the standard log
	// handlers.
	bool FLAG_verbose_log = false;


#ifdef _WIN32
#define vsnprintf	_vsnprintf
#endif // _WIN32

	static const int	BUFFER_SIZE = 500;

#define FORMAT_INTO_BUFFER(fmt)				\
		char	buffer[BUFFER_SIZE];		\
		va_list ap;				\
		va_start(ap, fmt);			\
		vsnprintf(buffer, BUFFER_SIZE, fmt, ap);\
		va_end(ap);


	void	vmsg(const char* fmt, ...)
	// Printf-style informational log.
	{
		if (FLAG_verbose_log && s_log_callback)
		{
			FORMAT_INTO_BUFFER(fmt);
			s_log_callback(VERBOSE, buffer);
		}
	}


	void	msg(const char* fmt, ...)
	// Printf-style informational log.
	{
		if (s_log_callback)
		{
			FORMAT_INTO_BUFFER(fmt);
			s_log_callback(NORMAL, buffer);
		}
	}


	void	error(const char* fmt, ...)
	// Printf-style error log.
	{
		if (s_log_callback)
		{
			FORMAT_INTO_BUFFER(fmt);
			s_log_callback(ERROR, buffer);
		}
	}
}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
