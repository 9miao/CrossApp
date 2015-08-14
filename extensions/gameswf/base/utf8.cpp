// utf8.cpp	-- Thatcher Ulrich 2004   -*- coding: utf-8;-*-

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.  THE AUTHOR DOES NOT WARRANT THIS CODE.

// Utility code for dealing with UTF-8 encoded text.
//
// Much useful info at "UTF-8 and Unicode FAQ" http://www.cl.cam.ac.uk/~mgk25/unicode.html


#include "utf8.h"


Uint32	utf8::decode_next_unicode_character(const char** utf8_buffer)
{
	Uint32	uc;
	char	c;

	// Security considerations:
	//
	// If we hit a zero byte, we want to return 0 without stepping
	// the buffer pointer past the 0.
	//
	// If we hit an "overlong sequence"; i.e. a character encoded
	// in a longer multibyte string than is necessary, then we
	// need to discard the character.  This is so attackers can't
	// disguise dangerous characters or character sequences --
	// there is only one valid encoding for each character.
	//
	// If we decode characters { 0xD800 .. 0xDFFF } or { 0xFFFE,
	// 0xFFFF } then we ignore them; they are not valid in UTF-8.

// This isn't actually an invalid character; it's a valid char that
// looks like an inverted question mark.
#define INVALID 0x0FFFD

#define FIRST_BYTE(mask, shift)		\
	uc = (c & (mask)) << (shift);

#define NEXT_BYTE(shift)						\
	c = **utf8_buffer;						\
	if (c == 0) return 0; /* end of buffer, do not advance */	\
	if ((c & 0xC0) != 0x80) return INVALID; /* standard check */	\
	(*utf8_buffer)++;						\
	uc |= (c & 0x3F) << shift;

	c = **utf8_buffer;
	if (c == 0) return 0;	// End of buffer.  Do not advance.

	(*utf8_buffer)++;
	if ((c & 0x80) == 0) return (Uint32) c;	// Conventional 7-bit ASCII.

	// Multi-byte sequences.
	if ((c & 0xE0) == 0xC0)
	{
		// Two-byte sequence.
		FIRST_BYTE(0x1F, 6);
		NEXT_BYTE(0);
		if (uc < 0x80) return INVALID;	// overlong
		return uc;
	}
	else if ((c & 0xF0) == 0xE0)
	{
		// Three-byte sequence.
		FIRST_BYTE(0x0F, 12);
		NEXT_BYTE(6);
		NEXT_BYTE(0);
		if (uc < 0x800) return INVALID;	// overlong
		if (uc >= 0x0D800 && uc <= 0x0DFFF) return INVALID;	// not valid ISO 10646
		if (uc == 0x0FFFE || uc == 0x0FFFF) return INVALID;	// not valid ISO 10646
		return uc;
	}
	else if ((c & 0xF8) == 0xF0)
	{
		// Four-byte sequence.
		FIRST_BYTE(0x07, 18);
		NEXT_BYTE(12);
		NEXT_BYTE(6);
		NEXT_BYTE(0);
		if (uc < 0x010000) return INVALID;	// overlong
		return uc;
	}
	else if ((c & 0xFC) == 0xF8)
	{
		// Five-byte sequence.
		FIRST_BYTE(0x03, 24);
		NEXT_BYTE(18);
		NEXT_BYTE(12);
		NEXT_BYTE(6);
		NEXT_BYTE(0);
		if (uc < 0x0200000) return INVALID;	// overlong
		return uc;
	}
	else if ((c & 0xFE) == 0xFC)
	{
		// Six-byte sequence.
		FIRST_BYTE(0x01, 30);
		NEXT_BYTE(24);
		NEXT_BYTE(18);
		NEXT_BYTE(12);
		NEXT_BYTE(6);
		NEXT_BYTE(0);
		if (uc < 0x04000000) return INVALID;	// overlong
		return uc;
	}
	else
	{
		// Invalid.
		return INVALID;
	}
}


void	utf8::encode_unicode_character(char* buffer, int* index, Uint32 ucs_character)
{
	if (ucs_character <= 0x7F)
	{
		// Plain single-byte ASCII.
		buffer[(*index)++] = (char) ucs_character;
	}
	else if (ucs_character <= 0x7FF)
	{
		// Two bytes.
		buffer[(*index)++] = 0xC0 | (ucs_character >> 6);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 0) & 0x3F);
	}
	else if (ucs_character <= 0xFFFF)
	{
		// Three bytes.
		buffer[(*index)++] = 0xE0 | (ucs_character >> 12);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 6) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 0) & 0x3F);
	}
	else if (ucs_character <= 0x1FFFFF)
	{
		// Four bytes.
		buffer[(*index)++] = 0xF0 | (ucs_character >> 18);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 12) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 6) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 0) & 0x3F);
	}
	else if (ucs_character <= 0x3FFFFFF)
	{
		// Five bytes.
		buffer[(*index)++] = 0xF8 | (ucs_character >> 24);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 18) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 12) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 6) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 0) & 0x3F);
	}
	else if (ucs_character <= 0x7FFFFFFF)
	{
		// Six bytes.
		buffer[(*index)++] = 0xFC | (ucs_character >> 30);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 24) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 18) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 12) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 6) & 0x3F);
		buffer[(*index)++] = 0x80 | ((ucs_character >> 0) & 0x3F);
	}
	else
	{
		// Invalid char; don't encode anything.
	}
}


#ifdef UTF8_UNIT_TEST

// Compile this test case with something like:
//
// gcc utf8.cpp -g -I.. -DUTF8_UNIT_TEST -lstdc++ -o utf8_test
//
//    or
//
// cl utf8.cpp -Zi -Od -DUTF8_UNIT_TEST -I..
//
// If possible, try running the test program with the first arg
// pointing at the file:
//
// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
// 
// and examine the results by eye to make sure they are acceptable to
// you.


#include "utility.h"
#include <stdio.h>


bool	check_equal(const char* utf8_in, const Uint32* ucs_in)
{
	for (;;)
	{
		Uint32	next_ucs = *ucs_in++;
		Uint32	next_ucs_from_utf8 = utf8::decode_next_unicode_character(&utf8_in);
		if (next_ucs != next_ucs_from_utf8)
		{
			return false;
		}
		if (next_ucs == 0)
		{
			assert(next_ucs_from_utf8 == 0);
			break;
		}
	}

	return true;
}


void	log_ascii(const char* line)
{
	for (;;)
	{
		unsigned char	c = (unsigned char) *line++;
		if (c == 0)
		{
			// End of line.
			return;
		}
		else if (c != '\n'
			 && (c < 32 || c > 127))
		{
			// Non-printable as plain ASCII.
			printf("<0x%02X>", (int) c);
		}
		else
		{
			printf("%c", c);
		}
	}
}


void	log_ucs(const Uint32* line)
{
	for (;;)
	{
		Uint32	uc = *line++;
		if (uc == 0)
		{
			// End of line.
			return;
		}
		else if (uc != '\n'
			 && (uc < 32 || uc > 127))
		{
			// Non-printable as plain ASCII.
			printf("<U-%04X>", uc);
		}
		else
		{
			printf("%c", (char) uc);
		}
	}
}


int main(int argc, const char* argv[])
{
	// Simple canned test.
	{
		const char*	test8 = "Ignacio Castaño";
		const Uint32	test32[] =
			{
				0x49, 0x67, 0x6E, 0x61, 0x63,
				0x69, 0x6F, 0x20, 0x43, 0x61,
				0x73, 0x74, 0x61, 0xF1, 0x6F,
				0x00
			};

		assert(check_equal(test8, test32));
	}

	// If user passed an arg, try reading the file as UTF-8 encoded text.
	if (argc > 1)
	{
		const char*	filename = argv[1];
		FILE*	fp = fopen(filename, "rb");
		if (fp == NULL)
		{
			printf("Can't open file '%s'\n", filename);
			return 1;
		}

		// Read lines from the file, encode/decode them, and highlight discrepancies.
		const int LINE_SIZE = 200;	// max line size
		char	line_buffer_utf8[LINE_SIZE];
		char	reencoded_utf8[6 * LINE_SIZE];
		Uint32	line_buffer_ucs[LINE_SIZE];

		int	byte_counter = 0;
		for (;;)
		{
			int	c = fgetc(fp);
			if (c == EOF)
			{
				// Done.
				break;
			}
			line_buffer_utf8[byte_counter++] = c;
			if (c == '\n' || byte_counter >= LINE_SIZE - 2)
			{
				// End of line.  Process the line.
				line_buffer_utf8[byte_counter++] = 0;	// terminate.

				// Decode into UCS.
				const char*	p = line_buffer_utf8;
				Uint32*	q = line_buffer_ucs;
				for (;;)
				{
					Uint32	uc = utf8::decode_next_unicode_character(&p);
					*q++ = uc;

					assert(q < line_buffer_ucs + LINE_SIZE);
					assert(p < line_buffer_utf8 + LINE_SIZE);

					if (uc == 0) break;
				}

				// Encode back into UTF-8.
				q = line_buffer_ucs;
				int	index = 0;
				for (;;)
				{
					Uint32	uc = *q++;
					assert(index < LINE_SIZE * 6 - 6);
					int	last_index = index;
					utf8::encode_unicode_character(reencoded_utf8, &index, uc);
					assert(index <= last_index + 6);
					if (uc == 0) break;
				}

// This can be useful for debugging.
#if 0
				// Show the UCS and the re-encoded UTF-8.
				log_ucs(line_buffer_ucs);
				log_ascii(reencoded_utf8);
#endif // 0

				assert(check_equal(line_buffer_utf8, line_buffer_ucs));
				assert(check_equal(reencoded_utf8, line_buffer_ucs));

				// Start next line.
				byte_counter = 0;
			}
		}

		fclose(fp);
	}

	return 0;
}


#endif // UTF8_UNIT_TEST


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
