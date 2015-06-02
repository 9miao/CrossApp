// file_util.h	-- Thatcher Ulrich 2005

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Misc file-handling utilities.


#ifndef FILE_UTIL_H
#define FILE_UTIL_H


#include "tu_config.h"


namespace file_util {
	// Return a pointer to the file extension.  This is the last
	// bit of the filename after the last '.'.  If there's no '.',
	// or it appears before the last '/', then the return value
	// points to an empty string.
	//
	// Does not point to the '.' itself;
	// i.e. get_extension("test.txt") will return "txt".
	const char* get_extension(const char* path);
}


#endif // FILE_UTIL_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
