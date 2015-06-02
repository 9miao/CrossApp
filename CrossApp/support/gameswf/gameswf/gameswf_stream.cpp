// gameswf_stream.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// SWF stream wrapper class, for loading variable-length data from a
// stream, and keeping track of SWF tag boundaries.
#include "gameswf_stream.h"
#include "gameswf_log.h"
#include "gameswf_types.h"
#include "../base/tu_file.h"
#include <string.h>


namespace gameswf
{
	
	stream::stream(tu_file* input)
		:
		m_input(input),
		m_current_byte(0),
		m_unused_bits(0)
	{
	}


	stream::~stream()
	{
	}

	bool stream::read_bool() {
		return read_uint(1) ? true : false;
	}

// @@ better?
// 	int	stream::read_uint(int bitcount)
// 	{
//		assert(bitcount <= 24);
// 		while (m_unused_bits < bitcount)
// 		{
// 			// Get more data.
// 			m_current_bits |= m_input->read_byte() << m_unused_bits;
// 			m_unused_bits += 8;
// 		}

// 		int	result = m_current_bits & ((1 << bitcount) - 1);
// 		m_current_bits >>= bitcount;
// 		m_unused_bits -= bitcount;
		
// 		return result;
// 	}


	// The variable-length encoding for u30, u32, and s32 uses one to five bytes,
	// depending on the magnitude of the value encoded.
	// Each byte contributes its low seven bits to the value. 
	// If the high (eighth) bit of a byte is set, then the next byte of the abcFile 
	// is also part of the value. 
	// In the case of s32, sign extension is applied: the
	// seventh bit of the last byte of the encoding is propagated to fill out
	// the 32 bits of the decoded value.

	Uint32 stream::read_vu32()
	{
		Uint32 result = read_u8();
		if ((result & 0x00000080) == 0)
		{
			return result;
		}

		result = (result & 0x0000007F) | read_u8() << 7;
		if ((result & 0x00004000) == 0)
		{
			return result;
		}

		result = (result & 0x00003FFF) | read_u8() << 14;
		if ((result & 0x00200000) == 0)
		{
			return result;
		}

		result = (result & 0x001FFFFF) | read_u8() << 21;
		if ((result & 0x10000000) == 0)
		{
			return result;
		}

		result = (result & 0x0FFFFFFF) | read_u8() << 28;
		return result;
	}
	
	int	stream::read_uint(int bitcount)
	// Reads a bit-packed unsigned integer from the stream
	// and returns it.  The given bitcount determines the
	// number of bits to read.
	{
		assert(bitcount <= 32 && bitcount >= 0);
			
		Uint32	value = 0;

		int	bits_needed = bitcount;
		while (bits_needed > 0)
		{
			if (m_unused_bits) {
				if (bits_needed >= m_unused_bits) {
					// Consume all the unused bits.
					value |= (m_current_byte << (bits_needed - m_unused_bits));

					bits_needed -= m_unused_bits;

					m_current_byte = 0;
					m_unused_bits = 0;

				} else {
					// Consume some of the unused bits.
					value |= (m_current_byte >> (m_unused_bits - bits_needed));

					// mask off the bits we consumed.
					m_current_byte &= ((1 << (m_unused_bits - bits_needed)) - 1);

					m_unused_bits -= bits_needed;

					// We're done.
					bits_needed = 0;
				}
			} else {
				m_current_byte = m_input->read_byte();
				m_unused_bits = 8;
			}
		}

		assert(bits_needed == 0);

		return value;
	}


	int	stream::read_sint(int bitcount)
	// Reads a bit-packed little-endian signed integer
	// from the stream.  The given bitcount determines the
	// number of bits to read.
	{
		assert(bitcount <= 32 && bitcount >= 0);

		Sint32	value = (Sint32) read_uint(bitcount);

		// Sign extend...
		if (value & (1 << (bitcount - 1))) {
			value |= -1 << bitcount;
		}

//		IF_DEBUG(log_msg("stream::read_sint(%d) == %d\n", bitcount, value));

		return value;
	}


	float	stream::read_fixed()
	{
		m_unused_bits = 0;
		Sint32	val = m_input->read_le32();
		return (float) val / 65536.0f;
	}

	float	stream::read_float16()
	// 1 bit for the sign
	// 5 bits for the exponent, with an exponent bias of 16
	// 10 bits for the mantissa
	{
		m_unused_bits = 0;

		Uint16	val = m_input->read_le16();
		Uint32 x = (val & 0x8000) << 16;
		int exponent = (val & 0x7C00) >> 10;

		// Special Values
		if (exponent)
		{
			x |= (exponent + (127 - 16)) << 23;	// exponent
		}

		// mantissa
		x |= (val & 0x3FF) << 13;

		float f;
		assert(sizeof(float) == sizeof(Uint32));
		memcpy(&f, &x, sizeof(float));
		return f;
	}

	float	stream::read_float()
	// 1 bit for the sign
	// 8 bits for the exponent, with an exponent bias of 127
	// 23 bits for the mantissa
	{
		m_unused_bits = 0;

		// not tested
		Uint32 val = m_input->read_le32();
		float f;
		assert(sizeof(float) == sizeof(Uint32));
		memcpy(&f, &val, sizeof(float));
		return f;
	}

	double	stream::read_double()
	// 1 bit for the sign
	// 11 bits for the exponent, with an exponent bias of 1023
	// 52 bits for the mantissa
	{
		m_unused_bits = 0;

		// not tested
		Uint64	val = m_input->read_le64();
		double f;
		assert(sizeof(double) == sizeof(Uint64));
		memcpy(&f, &val, sizeof(double));
		return f;
	}

	void	stream::align() { m_unused_bits = 0; m_current_byte = 0; }

	Uint8	stream::read_u8() { align(); return m_input->read_byte(); }
	Sint8	stream::read_s8() { align(); return m_input->read_byte(); }
	Uint16	stream::read_u16()
	{
		align();
//		IF_DEBUG(printf("filepos = %d ", SDL_RWtell(m_input)));
		int	val = m_input->read_le16();
//		IF_DEBUG(log_msg("val = 0x%X\n", val));
		return val;
	}
	Sint16	stream::read_s16() { align(); return m_input->read_le16(); }
	Uint32	stream::read_u32()
	{
		align();
		Uint32	val = m_input->read_le32();
		return val;
	}
	Sint32	stream::read_s32() { align(); return m_input->read_le32(); }


	void stream::read_string(tu_string* str)
	{
		align();
		str->resize(0);

		while (char c = read_u8())
		{
			*str += c;
		}
	}

	void	stream::read_string_with_length(tu_string* str)
	{
		align();
		str->resize(0);

		int	len = read_u8();
		for (int i = 0; i < len; i++)
		{
			*str += read_u8();
		}
	}

	void	stream::read_string_with_length(int len, tu_string* str)
	{
//???		align();
		str->resize(0);

		for (int i = 0; i < len; i++)
		{
			*str += read_u8();
		}
	}

	int	stream::get_position()
	// Return our current (byte) position in the input stream.
	{
		return m_input->get_position();
	}


	void	stream::set_position(int pos)
	// Set the file position to the given value.
	{
		align();

		// If we're in a tag, make sure we're not seeking outside the tag.
		if (m_tag_stack.size() > 0)
		{
			int	end_pos = m_tag_stack.back();
			assert(pos <= end_pos);
			end_pos = end_pos;	// inhibit warning
			// @@ check start pos somehow???
		}

		// Do the seek.
		m_input->set_position(pos);
	}


	int	stream::get_tag_end_position()
	// Return the file position of the end of the current tag.
	{
		assert(m_tag_stack.size() > 0);

		return m_tag_stack.back();
	}


	int	stream::open_tag()
	// Return the tag type.
	{
		align();
		int	tag_header = read_u16();
		int	tag_type = tag_header >> 6;
		int	tag_length = tag_header & 0x3F;
		assert(m_unused_bits == 0);
		if (tag_length == 0x3F) {
			tag_length = m_input->read_le32();
		}

		IF_VERBOSE_PARSE(log_msg("---------------tag type = %d, tag length = %d\n", tag_type, tag_length));
			
		// Remember where the end of the tag is, so we can
		// fast-forward past it when we're done reading it.
		m_tag_stack.push_back(get_position() + tag_length);

		return tag_type;
	}


	void	stream::close_tag()
	// Seek to the end of the most-recently-opened tag.
	{
		assert(m_tag_stack.size() > 0);
		int	end_pos = m_tag_stack.back();
		m_tag_stack.pop_back();

		if (end_pos != get_position())
		{
			IF_VERBOSE_PARSE( log_msg( "tag is not correctly read, tag length is not respected\n" ) );
		}

		m_input->set_position(end_pos);

		m_unused_bits = 0;
	}

} // end namespace gameswf

	
// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
