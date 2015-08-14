// zlib_adapter.cpp	-- Thatcher Ulrich 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code to wrap zlib compression/decompression around a tu_file
// stream.


#include "zlib_adapter.h"
#include "tu_file.h"
#include "utility.h"


#if !TU_CONFIG_LINK_TO_ZLIB


// Stubs, in case client doesn't want to link to zlib.
namespace zlib_adapter
{
	tu_file*	make_inflater(tu_file* in) { return NULL; }
	tu_file*	make_deflater(tu_file* out) { return NULL; }
}


#else // TU_CONFIG_LINK_TO_ZLIB


#include <zlib.h>


namespace zlib_adapter
{
	const int	ZBUF_SIZE = 4096;

	struct inflater_impl
	{
		tu_file*	m_in;
		z_stream	m_zstream;
		int	m_initial_stream_pos;	// position of the input stream where we started inflating.
		int	m_logical_stream_pos;	// current stream position of uncompressed data.
		bool	m_at_eof;

		unsigned char	m_rawdata[ZBUF_SIZE];

		int	m_error;


		inflater_impl(tu_file* in)
		// Constructor.
			:
			m_in(in),
			m_initial_stream_pos(in->get_position()),
			m_logical_stream_pos(0),
			m_at_eof(false),
			m_error(0)
		{
			assert(m_in);

			m_zstream.zalloc = (alloc_func)0;
			m_zstream.zfree = (free_func)0;
			m_zstream.opaque = (voidpf)0;

			m_zstream.next_in  = 0;
			m_zstream.avail_in = 0;

			m_zstream.next_out = 0;
			m_zstream.avail_out = 0;

			int	err = inflateInit(&m_zstream);
			if (err != Z_OK) {
				//log_error("error: inflater_impl::ctor() inflateInit() returned %d\n", err);
				m_error = 1;
				return;
			}

			// Ready to go!
		}


		void	reset()
		// Discard current results and rewind to the beginning.
		// Necessary in order to seek backwards.
		{
			m_error = 0;
			m_at_eof = 0;
			int	err = inflateReset(&m_zstream);
			if (err != Z_OK) {
				m_error = 1;
				return;
			}

			m_zstream.next_in = 0;
			m_zstream.avail_in = 0;

			m_zstream.next_out = 0;
			m_zstream.avail_out = 0;

			// Rewind the underlying stream.
			m_in->set_position(m_initial_stream_pos);

			m_logical_stream_pos = 0;
		}


		int	inflate_from_stream(void* dst, int bytes)
		{
			if (m_error)
			{
				return 0;
			}

			m_zstream.next_out = (unsigned char*) dst;
			m_zstream.avail_out = bytes;

			for (;;)
			{
				if (m_zstream.avail_in == 0)
				{
					// Get more raw data.
					int	new_bytes = m_in->read_bytes(m_rawdata, ZBUF_SIZE);
					if (new_bytes == 0)
					{
						// The cupboard is bare!  We have nothing to feed to inflate().
						break;
					}
					else
					{
						m_zstream.next_in = m_rawdata;
						m_zstream.avail_in = new_bytes;
					}
				}

				int	err = inflate(&m_zstream, Z_SYNC_FLUSH);
				if (err == Z_STREAM_END)
				{
					m_at_eof = true;
					break;
				}
				if (err != Z_OK)
				{
					// something's wrong.
					m_error = 1;
					break;
				}

				if (m_zstream.avail_out == 0)
				{
					break;
				}
			}

			int	bytes_read = bytes - m_zstream.avail_out;
			m_logical_stream_pos += bytes_read;

			return bytes_read;
		}

		void	rewind_unused_bytes()
		// If we have unused bytes in our input buffer, rewind
		// to before they started.
		{
			if (m_zstream.avail_in > 0)
			{
				int	pos = m_in->get_position();
				int	rewound_pos = pos - m_zstream.avail_in;
				assert(pos >= 0);
				assert(pos >= m_initial_stream_pos);
				assert(rewound_pos >= 0);
				assert(rewound_pos >= m_initial_stream_pos);

				m_in->set_position(rewound_pos);
			}
		}
	};


	int	inflate_read(void* dst, int bytes, void* appdata)
	// Return number of bytes actually read.
	{
		inflater_impl*	inf = (inflater_impl*) appdata;
		if (inf->m_error)
		{
			return 0;
		}

		return inf->inflate_from_stream(dst, bytes);
	}


	int	inflate_write(const void* src, int bytes, void* appdata)
	// Return number of bytes actually written.
	{
		// *In*flaters can't write!!!
		assert(0);
		return 0;
	}


	int	inflate_seek(int pos, void* appdata)
	// Try to go to pos.  Return actual pos.
	{
		inflater_impl*	inf = (inflater_impl*) appdata;
		if (inf->m_error)
		{
			return inf->m_logical_stream_pos;
		}

		// If we're seeking backwards, then restart from the beginning.
		if (pos < inf->m_logical_stream_pos)
		{
			inf->reset();
		}

		unsigned char	temp[ZBUF_SIZE];

		// Now seek forwards, by just reading data in blocks.
		while (inf->m_logical_stream_pos < pos)
		{
			int	to_read = pos - inf->m_logical_stream_pos;
			int	to_read_this_time = imin_swf(to_read, ZBUF_SIZE);
			assert(to_read_this_time > 0);

			int	bytes_read = inf->inflate_from_stream(temp, to_read_this_time);
			assert(bytes_read <= to_read_this_time);
			if (bytes_read == 0)
			{
				// Trouble; can't seek any further.
				break;
			}
		}

		assert(inf->m_logical_stream_pos <= pos);

		return inf->m_logical_stream_pos;
	}


	int	inflate_seek_to_end(void* appdata)
	{
		inflater_impl*	inf = (inflater_impl*) appdata;
		if (inf->m_error)
		{
			return inf->m_logical_stream_pos;
		}

		// Keep reading until we can't read any more.

		unsigned char	temp[ZBUF_SIZE];

		// Seek forwards.
		for (;;)
		{
			int	bytes_read = inf->inflate_from_stream(temp, ZBUF_SIZE);
			if (bytes_read == 0)
			{
				// We've seeked as far as we can.
				break;
			}
		}

		return inf->m_logical_stream_pos;
	}

	int	inflate_tell(const void* appdata)
	{
		inflater_impl*	inf = (inflater_impl*) appdata;

		return inf->m_logical_stream_pos;
	}

	bool	inflate_get_eof(void* appdata)
	{
		inflater_impl*	inf = (inflater_impl*) appdata;

		return inf->m_at_eof;
	}

	int	inflate_close(void* appdata)
	{
		inflater_impl*	inf = (inflater_impl*) appdata;

		inf->rewind_unused_bytes();
		int	err = inflateEnd(&(inf->m_zstream));

		delete inf;

		if (err != Z_OK)
		{
			return TU_FILE_CLOSE_ERROR;
		}

		return 0;
	}


	tu_file*	make_inflater(tu_file* in)
	{
		assert(in);

		inflater_impl*	inflater = new inflater_impl(in);
		return new tu_file(
			inflater,
			inflate_read,
			inflate_write,
			inflate_seek,
			inflate_seek_to_end,
			inflate_tell,
			inflate_get_eof,
			inflate_close);
	}


	// @@ TODO
	// tu_file*	make_deflater(tu_file* out) { ... }
}

#endif // TU_CONFIG_LINK_TO_ZLIB


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
