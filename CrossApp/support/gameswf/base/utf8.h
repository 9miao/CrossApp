// utf8.h	-- Thatcher Ulrich <tu@tulrich.com> 2004

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.  THE AUTHOR DOES NOT WARRANT THIS CODE.

// Utility code for dealing with UTF-8 encoded text.


#ifndef UTF8_H
#define UTF8_H


#include "tu_types.h"


namespace utf8
{
	// Return the next Unicode character in the UTF-8 encoded
	// buffer.  Invalid UTF-8 sequences produce a U+FFFD character
	// as output.  Advances *utf8_buffer past the character
	// returned, unless the returned character is '\0', in which
	// case the buffer does not advance.
	Uint32	decode_next_unicode_character(const char** utf8_buffer);

	// Encodes the given UCS character into the given UTF-8
	// buffer.  Writes the data starting at buffer[offset], and
	// increments offset by the number of bytes written.
	//
	// May write up to 6 bytes, so make sure there's room in the
	// buffer!
	void	encode_unicode_character(char* buffer, int* offset, Uint32 ucs_character);
}


#endif // UTF8_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
