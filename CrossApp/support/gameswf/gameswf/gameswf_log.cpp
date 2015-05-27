// gameswf_log.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Helpers for logging messages & errors.


#include "gameswf_log.h"
#include "gameswf.h"

#include <stdio.h>
#include <stdarg.h>


namespace gameswf
{

	void standard_logger(const bool error, const char* message)
	{
		if (error)
		{
			fprintf(stderr, message);
		}
		else
		{
			// NORMAL or VERBOSE
			printf(message);
		}
	}

	// Function pointer to log callback.
	static void (*s_log_callback)(bool error, const char* message) = standard_logger;

	// Workspace for vsnprintf formatting.
	static const int	BUFFER_SIZE = 4096;
	static char	s_buffer[BUFFER_SIZE];


	void	register_log_callback(void (*callback)(bool error, const char* message))
	// The host app can use this to install a function to receive log
	// & error messages from gameswf.
	//
	// Pass in NULL to inhibit logging of messages & errors.
	{
		s_log_callback = callback;
	}


#ifdef _WIN32
#define vsnprintf	_vsnprintf
#endif // _WIN32

#define FORMAT_INTO_BUFFER(fmt)				\
		va_list ap;				\
		va_start(ap, fmt);			\
		vsnprintf(s_buffer, BUFFER_SIZE, fmt, ap);	\
		va_end(ap);

	void	log_msg(const char* fmt, ...)
	// Printf-style informational log.
	{
		if (s_log_callback == NULL)
		{
			return;
		}

		FORMAT_INTO_BUFFER(fmt);

		s_log_callback(false, s_buffer);
	}


	void	log_error(const char* fmt, ...)
	// Printf-style error log.
	{
		if (s_log_callback == NULL)
		{
			return;
		}

		FORMAT_INTO_BUFFER(fmt);

		s_log_callback(true, s_buffer);
	}
}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

