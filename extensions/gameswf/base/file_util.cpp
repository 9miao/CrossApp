// file_util.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2005

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A file class that can be customized with callbacks.


#include "file_util.h"
#include "utility.h"
#include <string.h>


static const char* reverse_scan(const char* begin, const char* end, char c)
// Scans in reverse, from *(end-1) through *begin, until it finds a
// character matching c.  If none is found, returns end, else returns
// a pointer to the char.
{
	assert(begin <= end);
	
	const char* p = end;
	while (p > begin) {
		p--;
		if (*p == c) {
			return p;
		}
	}

	return end;
}


const char* file_util::get_extension(const char* path)
{
	int len = (int) strlen(path);
	const char* last_dot = reverse_scan(path, path + len, '.');
	const char* last_slash = reverse_scan(last_dot, path + len, '/');

	if (last_dot[0] && last_slash[0] == 0) {
		return last_dot + 1;
	}

	// No apparent file extension, return an empty string.
	return path + len;
}



// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
