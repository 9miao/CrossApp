// tu_file.h	-- Ignacio Castaño, Thatcher Ulrich 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A very generic file class that can be customized with callbacks.


#ifndef TU_FILE_H
#define TU_FILE_H


#include "tu_config.h"
#include <stdio.h>
#include "tu_types.h"
#include "tu_swap.h"
#include "utility.h"
#include "container.h"

struct membuf;

#ifdef TU_USE_SDL
struct SDL_RWops;
#endif


enum
{
	TU_FILE_NO_ERROR = 0,
	TU_FILE_OPEN_ERROR,
	TU_FILE_READ_ERROR,
	TU_FILE_WRITE_ERROR,
	TU_FILE_SEEK_ERROR,
	TU_FILE_CLOSE_ERROR
};


// a file abstraction that can be customized with callbacks.
// Designed to be easy to hook up to FILE*, SDL_RWops*, or
// whatever stream type(s) you might use in your game or
// libraries.
class tu_file
{
public:
	typedef int (* read_func)(void* dst, int bytes, void* appdata);
	typedef int (* write_func)(const void* src, int bytes, void* appdata);
	typedef int (* seek_func)(int pos, void* appdata);
	typedef int (* seek_to_end_func)(void* appdata);
	typedef int (* tell_func)(const void* appdata);
	typedef bool (* get_eof_func)(void* appdata);
	typedef int (* close_func)(void* appdata);

	// The generic constructor; supply functions for the implementation.
	exported_module tu_file(
		void * appdata,
		read_func rf,
		write_func wf,
		seek_func sf,
		seek_to_end_func ef,
		tell_func tf,
		get_eof_func gef,
		close_func cf=NULL);

	// Make a file from an ordinary FILE*.
	exported_module tu_file(FILE* fp, bool autoclose);

#ifdef TU_USE_SDL
	// Optional: if you're using SDL, this is a constructor to create
	// a tu_file from an SDL_RWops* stream.
	exported_module tu_file(SDL_RWops* sdl_stream, bool autoclose);
#endif

	// Open a file using ordinary fopen().  Automatically closes the
	// file when we are destroyed.
	exported_module tu_file(const char* name, const char* mode);

	// Make a memory-buffer file for read/write.
	enum memory_buffer_enum { memory_buffer };
	exported_module tu_file(memory_buffer_enum m);

	// A read-only memory-buffer with predefined data.
	exported_module tu_file(memory_buffer_enum m, int size, void* data);

	exported_module ~tu_file();

	// Copy remaining contents of *in into *this.
	exported_module void copy_from(tu_file* in);
	// Copy remaining contents of *this into *out.
	exported_module void copy_to(membuf* out);

	// Copy a fixed number of bytes from *in to *this.
	// Returns number of bytes copied.
	exported_module int	copy_bytes(tu_file* in, int bytes);

	Uint64	read_le64();
	Uint32 	read_le32();
	Uint16 	read_le16();
	Uint64 	read_be64();
	Uint32 	read_be32();
	Uint16 	read_be16();
	void 	write_le64(Uint64 u);
	void 	write_le32(Uint32 u);
	void 	write_le16(Uint16 u);
	void	write_be64(Uint64 u);
	void 	write_be32(Uint32 u);
	void 	write_be16(Uint16 u);

	// read/write a single byte
	exported_module Uint8 	read_byte() { return read8(); }
	exported_module void	write_byte(Uint8 u) { write8(u); }

	exported_module int	size()
	{
		int current = get_position();
		go_to_end();
		int file_size = get_position();
		set_position(current);
		return file_size;
	}
	
	// Read/write a byte buffer.
	// Returns number of bytes read/written.
	exported_module int 	read_bytes(void* dst, int num) { return m_read(dst, num, m_data); }
	exported_module int 	write_bytes(const void* src, int num) { return m_write(src, num, m_data); }
	
	// write a 0-terminated string.
	exported_module void 	write_string(const char* src);
	
	// Read up to max_length characters, returns the number of characters read
	// Stops at the first '\n' character if it comes before max_length.
	// Removes the last '\r' character
	//
	// Guarantees termination of the string.
	exported_module int	read_string(char* dst, int max_length);

	// float/double IO
	exported_module void	write_float32(float value);
	exported_module void	write_double64(double value);
	exported_module float	read_float32();
	exported_module double	read_double64();

	// get/set pos
	exported_module int	get_position() const { return m_tell(m_data); }
	exported_module void 	set_position(int p) { m_seek(p, m_data); }
	exported_module void	go_to_end() { m_seek_to_end(m_data); }
	exported_module bool	get_eof() { return m_get_eof(m_data); }

	exported_module int	get_error() { return m_error; }

	// printf-style convenience function.
	int	printf(const char* fmt, ...);

	// UNSAFE back door, for testing only.
	void*	get_app_data_DEBUG() { return m_data; }


private:
	Uint64	read64() { Uint64 u; m_read(&u, 8, m_data); return u; }
	Uint32	read32() { Uint32 u; m_read(&u, 4, m_data); return u; }
	Uint16	read16() { Uint16 u; m_read(&u, 2, m_data); return u; }
	Uint8	read8() { Uint8 u; m_read(&u, 1, m_data); return u; }

	void	write64(Uint64 u) { m_write(&u, 8, m_data); }
	void	write32(Uint32 u) { m_write(&u, 4, m_data); }
	void	write16(Uint16 u) { m_write(&u, 2, m_data); }
	void	write8(Uint8 u) { m_write(&u, 1, m_data); }

	void	close();

	void * 	m_data;
	read_func 	m_read;
	write_func 	m_write;
	seek_func 	m_seek;
	seek_to_end_func 	m_seek_to_end;
	tell_func 	m_tell;
	get_eof_func	m_get_eof;
	close_func 	m_close;
	int	m_error;
};


//
// Some inline stuff.
//


#if _TU_LITTLE_ENDIAN_
	inline Uint64	tu_file::read_le64() { return read64(); }
	inline Uint32	tu_file::read_le32() { return read32(); }
	inline Uint16	tu_file::read_le16() { return read16(); }
	inline Uint64	tu_file::read_be64() { return swap64(read64()); }
	inline Uint32	tu_file::read_be32() { return swap32(read32()); }
	inline Uint16	tu_file::read_be16() { return swap16(read16()); }
	inline void	tu_file::write_le64(Uint64 u) { write64(u); }
	inline void	tu_file::write_le32(Uint32 u) { write32(u); }
	inline void	tu_file::write_le16(Uint16 u) { write16(u); }
	inline void	tu_file::write_be64(Uint64 u) { write64(swap64(u)); }
	inline void	tu_file::write_be32(Uint32 u) { write32(swap32(u)); }
	inline void	tu_file::write_be16(Uint16 u) { write16(swap16(u)); }
#else // not _TU_LITTLE_ENDIAN_
	inline Uint64	tu_file::read_le64() { return swap64(read64()); }
	inline Uint32	tu_file::read_le32() { return swap32(read32()); }
	inline Uint16	tu_file::read_le16() { return swap16(read16()); }
	inline Uint64	tu_file::read_be64() { return read64(); }
	inline Uint32	tu_file::read_be32() { return read32(); }
	inline Uint16	tu_file::read_be16() { return read16(); }
	inline void	tu_file::write_le64(Uint64 u) { write64(swap64(u)); }
	inline void	tu_file::write_le32(Uint32 u) { write32(swap32(u)); }
	inline void	tu_file::write_le16(Uint16 u) { write16(swap16(u)); }
	inline void	tu_file::write_be64(Uint64 u) { write64(u); }
	inline void	tu_file::write_be32(Uint32 u) { write32(u); }
	inline void	tu_file::write_be16(Uint16 u) { write16(u); }
#endif	// not _TU_LITTLE_ENDIAN_


inline void	tu_file::write_float32(float value)
// Write a 32-bit little-endian float to this file.
{
	union alias {
		float	f;
		Uint32	i;
	} u;
	compiler_assert(sizeof(alias) == sizeof(Uint32));

	u.f = value;
	write_le32(u.i);
}


inline float	tu_file::read_float32()
// Read a 32-bit little-endian float from this file.
{
	union {
		float	f;
		Uint32	i;
	} u;
	compiler_assert(sizeof(u) == sizeof(u.i));

	u.i = read_le32();
	return u.f;
}


inline void		tu_file::write_double64(double value)
// Write a 64-bit little-endian double to this file.
{
	union {
		double	d;
		Uint64	l;
	} u;
	compiler_assert(sizeof(u) == sizeof(u.l));

	u.d = value;
	write_le64(u.l);
}


inline double	tu_file::read_double64()
// Read a little-endian 64-bit double from this file.
{
	union {
		double	d;
		Uint64	l;
	} u;
	compiler_assert(sizeof(u) == sizeof(u.l));

	u.l = read_le64();
	return u.d;
}


#endif // TU_FILE_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
