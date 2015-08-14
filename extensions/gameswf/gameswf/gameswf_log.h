// gameswf_log.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Helpers for logging messages & errors.


#ifndef GAMESWF_LOG_H
#define GAMESWF_LOG_H

#include "gameswf.h"

namespace gameswf
{
	// Printf-style interfaces.

#ifdef __GNUC__
	// use the following to catch errors: (only with gcc)
	void	log_msg(const char* fmt, ...) __attribute__((format (printf, 1, 2)));
	void	log_error(const char* fmt, ...) __attribute__((format (printf, 1, 2)));
#else	// not __GNUC__
	exported_module void	log_msg(const char* fmt, ...);
	exported_module void	log_error(const char* fmt, ...);
#endif	// not __GNUC__

}


#endif // GAMESWF_LOG_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
