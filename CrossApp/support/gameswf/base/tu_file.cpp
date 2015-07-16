// tu_file.cpp	-- Ignacio Castaño, Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A file class that can be customized with callbacks.

//#include "../../../platform/CCCommon.h"
#include "tu_file.h"
#include "utility.h"
#include "container.h"
#include "membuf.h"

//
// tu_file functions using FILE
//

static int std_read_func(void* dst, int bytes, void* appdata) 
// Return the number of bytes actually read.  EOF or an error would
// cause that to not be equal to "bytes".
{
	assert(appdata);
	assert(dst);
	return (int) fread( dst, 1, bytes, (FILE *)appdata );
}

static int std_write_func(const void* src, int bytes, void* appdata)
// Return the number of bytes actually written.
{
	assert(appdata);
	assert(src);
	return (int) fwrite( src, 1, bytes, (FILE *)appdata );
}

static int std_seek_func(int pos, void *appdata)
// Return 0 on success, or TU_FILE_SEEK_ERROR on failure.
{
	assert(appdata);
	clearerr((FILE*) appdata);	// make sure EOF flag is cleared.
	int	result = fseek((FILE*)appdata, pos, SEEK_SET);
	if (result == EOF)
	{
		// @@ TODO should set m_error to something relevant based on errno.
		return TU_FILE_SEEK_ERROR;
	}
	return 0;
}

static int std_seek_to_end_func(void *appdata)
// Return 0 on success, TU_FILE_SEEK_ERROR on failure.
{
	assert(appdata);
	int	result = fseek((FILE*)appdata, 0, SEEK_END);
	if (result == EOF)
	{
		// @@ TODO should set m_error to something relevant based on errno.
		return TU_FILE_SEEK_ERROR;
	}
	return 0;
}

static int std_tell_func(const void *appdata)
// Return the file position, or -1 on failure.
{
	assert(appdata);
	return ftell((FILE*)appdata);
}

static bool std_get_eof_func(void *appdata)
// Return true if we're at EOF.
{
	assert(appdata);
	if (feof((FILE*) appdata))
	{
		return true;
	}
	else
	{
		return false;
	}
}

static int std_close_func(void *appdata)
// Return 0 on success, or TU_FILE_CLOSE_ERROR on failure.
{
	assert(appdata);
	int	result = fclose((FILE*)appdata);
	if (result == EOF)
	{
		// @@ TODO should set m_error to something relevant based on errno.
		return TU_FILE_CLOSE_ERROR;
	}
	return 0;
}


//
// tu_file functions using a readable/writable memory buffer
//


struct filebuf
{
	membuf m_;
	int	m_position;
	bool	m_read_only;

	filebuf()
		:
		m_position(0),
		m_read_only(false)
	{
	}

	filebuf(int size, void* data)
		:
		m_(membuf::READ_ONLY, data, size),
		m_position(0),
		m_read_only(true)
	{
	}

	~filebuf()
	{
	}

	bool	resize(int new_size)
	// Return false if we couldn't resize.
	{
		if (m_read_only) return false;

		m_.resize(new_size);

		// Hm, does this make sense?  We're truncating the file, so clamping the cursor.
		// Alternative would be to disallow resize, but that doesn't seem good either.
		if (m_position > m_.size())
		{
			m_position = m_.size();
		}

		return true;
	}

	bool	is_valid()
	{
		return
			m_position >= 0
			&& m_position <= m_.size();
	}

	unsigned char*	get_cursor() { return ((unsigned char*) m_.data()) + m_position; }
};


static int mem_read_func(void* dst, int bytes, void* appdata) 
// Return the number of bytes actually read.  EOF or an error would
// cause that to not be equal to "bytes".
{
	assert(appdata);
	assert(dst);

	filebuf* buf = (filebuf*) appdata;
	assert(buf->is_valid());

	int	bytes_to_read = imin_swf(bytes, buf->m_.size() - buf->m_position);
	if (bytes_to_read)
	{
		memcpy(dst, buf->get_cursor(), bytes_to_read);
	}
	buf->m_position += bytes_to_read;

	return bytes_to_read;
}


static int mem_write_func(const void* src, int bytes, void* appdata)
// Return the number of bytes actually written.
{
	assert(appdata);
	assert(src);

	filebuf* buf = (filebuf*) appdata;
	assert(buf->is_valid());

	// Expand buffer if necessary.
	int	bytes_to_expand = imax_swf(0, buf->m_position + bytes - buf->m_.size());
	if (bytes_to_expand)
	{
		if (buf->resize(buf->m_.size() + bytes_to_expand) == false)
		{
			// Couldn't expand!
			return 0;
		}
	}

	memcpy(buf->get_cursor(), src, bytes);
	buf->m_position += bytes;

	return bytes;
}

static int mem_seek_func(int pos, void *appdata)
// Return 0 on success, or TU_FILE_SEEK_ERROR on failure.
{
	assert(appdata);
	assert(pos >= 0);

	filebuf* buf = (filebuf*) appdata;
	assert(buf->is_valid());

	if (pos < 0)
	{
		buf->m_position = 0;
		return TU_FILE_SEEK_ERROR;
	}

	if (pos > buf->m_.size())
	{
		buf->m_position = buf->m_.size();
		return TU_FILE_SEEK_ERROR;
	}

	buf->m_position = pos;
	return 0;
}

static int mem_seek_to_end_func(void* appdata)
// Return 0 on success, TU_FILE_SEEK_ERROR on failure.
{
	assert(appdata);

	filebuf* buf = (filebuf*) appdata;
	assert(buf->is_valid());

	buf->m_position = buf->m_.size();
	return 0;
}

static int mem_tell_func(const void* appdata)
// Return the file position, or -1 on failure.
{
	assert(appdata);

	filebuf* buf = (filebuf*) appdata;
	assert(buf->is_valid());

	return buf->m_position;
}

static bool	mem_get_eof_func(void* appdata)
// Return true if we're positioned at the end of the buffer.
{
	assert(appdata);

	filebuf* buf = (filebuf*) appdata;
	assert(buf->is_valid());

	return buf->m_position >= buf->m_.size();
}

static int mem_close_func(void* appdata)
// Return 0 on success, or TU_FILE_CLOSE_ERROR on failure.
{
	assert(appdata);

	filebuf* buf = (filebuf*) appdata;
	assert(buf->is_valid());

	delete buf;

	return 0;
}


tu_file::tu_file(
	void * appdata,
	read_func rf,
	write_func wf,
	seek_func sf,
	seek_to_end_func ef,
	tell_func tf,
	get_eof_func gef,
	close_func cf)
// Create a file using the custom callbacks.
{
	m_data = appdata;
	m_read = rf;
	m_write = wf;
	m_seek = sf;
	m_seek_to_end = ef;
	m_tell = tf;
	m_get_eof = gef;
	m_close = cf;
	m_error = TU_FILE_NO_ERROR;
}


tu_file::tu_file(FILE* fp, bool autoclose=false)
// Create a file from a standard file pointer.
{
	m_data = (void *)fp;
	m_read = std_read_func;
	m_write = std_write_func;
	m_seek = std_seek_func;
	m_seek_to_end = std_seek_to_end_func;
	m_tell = std_tell_func;
	m_get_eof = std_get_eof_func;
	m_close = autoclose ? std_close_func : NULL;
	m_error = TU_FILE_NO_ERROR;
}

tu_file::tu_file(const char * name, const char * mode) :
// Create a file from the given name and the given mode.
	m_data(NULL),
	m_read(NULL),
	m_write(NULL),
	m_seek(NULL),
	m_seek_to_end(NULL),
	m_tell(NULL),
	m_get_eof(NULL),
	m_close(NULL),
	m_error(TU_FILE_OPEN_ERROR)
{
	assert(name);
	m_data = fopen(name, mode);
	if (m_data)
	{
		m_read = std_read_func;
		m_write = std_write_func;
		m_seek = std_seek_func;
		m_seek_to_end = std_seek_to_end_func;
		m_tell = std_tell_func;
		m_get_eof = std_get_eof_func;
		m_close = std_close_func;
		m_error = TU_FILE_NO_ERROR;
	}
}


tu_file::tu_file(memory_buffer_enum m)
// Create a read/write memory buffer.
{
	//	m_data = new filebuf;
	//	should actually be
	m_data = new filebuf();	// thanks to Eli Curtz 

	m_read = mem_read_func;
	m_write = mem_write_func;
	m_seek = mem_seek_func;
	m_seek_to_end = mem_seek_to_end_func;
	m_tell = mem_tell_func;
	m_get_eof = mem_get_eof_func;
	m_close = mem_close_func;
	m_error = TU_FILE_NO_ERROR;
}


tu_file::tu_file(memory_buffer_enum m, int size, void* data)
// Create a read-only memory buffer, using the given data.
{
	//	m_data = new membuf(data, size);
	//	should actually be
	m_data = new filebuf(size, data);	// thanks to Eli Curtz 

	m_read = mem_read_func;
	m_write = mem_write_func;
	m_seek = mem_seek_func;
	m_seek_to_end = mem_seek_to_end_func;
	m_tell = mem_tell_func;
	m_get_eof = mem_get_eof_func;
	m_close = mem_close_func;
	m_error = TU_FILE_NO_ERROR;
}


tu_file::~tu_file()
// Close this file when destroyed.
{
	close();
}


void tu_file::close() 
// Close this file.
{ 
	if (m_close)
	{
		m_close(m_data);
	}
	m_data = NULL; 
	m_read = NULL; 
	m_write = NULL; 
	m_seek = NULL; 
	m_tell = NULL; 
	m_close = NULL; 
}


void	tu_file::copy_from(tu_file* src)
// Copy remaining contents of *src into *this.
{
	// @@ bah, should buffer this!
	while (src->get_eof() == false)
	{
		Uint8	b = src->read8();
		if (src->get_error())
		{
			break;
		}

		write8(b);
	}
}


void	tu_file::copy_to(membuf* dst)
// Copy remaining contents of *this into *dst.
{
	static const int BUFSIZE = 4096;
	
	while (get_eof() == false)
	{
		// Make room at the end of dst.
		dst->resize(dst->size() + BUFSIZE);
		int bytes_read = read_bytes(((char*) dst->data()) + dst->size() - BUFSIZE, BUFSIZE);
		if (bytes_read < BUFSIZE) {
			// Didn't use everything we allocated; trim the unused bytes.
			dst->resize(dst->size() - (BUFSIZE - bytes_read));
		}

		if (get_error())
		{
			break;
		}
	}
}


int	tu_file::copy_bytes(tu_file* src, int byte_count)
// Copy a fixed number of bytes from *src into *this.  Return the
// number of bytes copied.
{
	static const int	BUFSIZE = 4096;
	char	buffer[BUFSIZE];

	int	bytes_left = byte_count;
	while (bytes_left)
	{
		int	to_copy = imin_swf(bytes_left, BUFSIZE);

		int	read_count = src->read_bytes(buffer, to_copy);
		int	write_count = write_bytes(buffer, read_count);

		assert(write_count <= read_count);
		assert(read_count <= to_copy);
		assert(to_copy <= bytes_left);

		bytes_left -= write_count;
		if (write_count < to_copy)
		{
			// Some kind of error; abort.
			return byte_count - bytes_left;
		}
	}

	assert(bytes_left == 0);

	return byte_count;
}


void tu_file::write_string(const char* src)
{
	while (*src)
	{
		write8(*src);
		src++;
	}
}

int tu_file::read_string(char* dst, int max_length) 
{
	int i = 0;
	while (i < max_length)
	{
		dst[i] = read8();
		if (get_eof() == true || dst[i] == '\n' || dst[i] == 0)
		{
			// remove the last '\r'
			if (i > 0 && dst[i - 1] == '\r')
			{
				i--;
			}

			dst[i] = 0;
			return i + 1;
		}
		i++;
	}
	dst[i - 1] = 0;	// force termination.
	return i;
}



#include <stdarg.h>
#include <string.h>

#ifdef _WIN32
#define vsnprintf	_vsnprintf
#endif // _WIN32


int	tu_file::printf(const char* fmt, ...)
// Use printf-like semantics to send output to this stream.
{
	// Workspace for vsnprintf formatting.
	static const int	BUFFER_SIZE = 1000;
	char	buffer[BUFFER_SIZE];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, BUFFER_SIZE, fmt, ap);
	va_end(ap);

	return write_bytes(buffer, (int) strlen(buffer));
}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
