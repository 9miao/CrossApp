// logger.h	-- Thatcher Ulrich <tu@tulrich.com> 2006

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Helpers for logging messages & errors.


#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H

#include "container.h"

namespace logger
{
	// Log to stdout/stderr.
	void set_standard_log_handlers();

	// Toggle this to enable verbose logging by the standard log
	// handlers.
	extern bool FLAG_verbose_log;

	// Use this to register your own log handler.  If you don't
	// register a log handler, the logs don't go anywhere.
	enum log_type {
		VERBOSE,
		NORMAL,
		ERROR,
	};
	exported_module void	register_log_callback(void (*callback)(log_type e, const char* message));


	// gcc has some nice printf arg checking stuff.
#ifdef __GNUC__
	#define ATTRCHECK __attribute__((format (printf, 1, 2)))
#else
	#define ATTRCHECK
#endif
	
	// These are the logging functions.  Printf-style interfaces.

	// Verbose log.
	void	vmsg(const char* fmt, ...) ATTRCHECK;
	// Normal log.
	void	msg(const char* fmt, ...) ATTRCHECK;
	// Error log.
	void	error(const char* fmt, ...) ATTRCHECK;
}

// Shorter aliases for the logging functions.  TODO add __FILE__, __LINE__ etc.
#define LOG logger::msg
#define VLOG logger::vmsg
#define LOGERROR logger::error


#endif // BASE_LOGGER_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
