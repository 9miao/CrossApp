// tu_file_SDL.cpp	-- Ignacio Castaño, Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// tu_file constructor, for creating a tu_file from an SDL_RWops*
// stream.  In its own source file so that if clients of the base
// library don't call it, it won't get pulled in by the linker and
// won't try to link with SDL.

#ifdef TU_USE_SDL

#include "tu_file.h"
#include "utility.h"
#include <SDL.h>


// TODO: add error detection and reporting!!!

static int sdl_read_func(void* dst, int bytes, void* appdata) 
{
	assert(dst);
	assert(appdata);
	int	result = SDL_RWread((SDL_RWops*) appdata, dst, 1, bytes);
	if (result == -1)
	{
		// @@ set appdata->m_error?
		return 0;
	}
	return result;
}

static int sdl_write_func(const void* src, int bytes, void* appdata)
{
	assert(src);
	assert(appdata);
	int	result = SDL_RWwrite((SDL_RWops*) appdata, src, 1, bytes);
	if (result == -1)
	{
		// @@ set m_errer?
		return 0;
	}
	return result;
}

static int sdl_seek_func(int pos, void *appdata)
{
	assert(pos >= 0);
	assert(appdata);
	return SDL_RWseek((SDL_RWops*) appdata, pos, SEEK_SET);
}

static int sdl_seek_to_end_func(void *appdata)
{
	assert(appdata);
	return SDL_RWseek((SDL_RWops*) appdata, 0, SEEK_END);
}

static int sdl_tell_func(const void *appdata)
{
	assert(appdata);
	return SDL_RWtell((SDL_RWops*) appdata);
}

static bool sdl_get_eof_func(void* appdata)
{
	assert(appdata);

	int	cur_pos = sdl_tell_func(appdata);
	sdl_seek_to_end_func(appdata);
	int	end_pos = sdl_tell_func(appdata);
	if (end_pos <= cur_pos)
	{
		return true;
	}
	else
	{
		sdl_seek_func(cur_pos, appdata);
		return false;
	}
}

static int sdl_close_func(void *appdata)
{
	assert(appdata);
	int	result = SDL_RWclose((SDL_RWops*) appdata);
	if (result != 0)
	{
		return TU_FILE_CLOSE_ERROR;
	}
	return 0;
}

	
tu_file::tu_file(SDL_RWops* sdl_stream, bool autoclose)
// Create a tu_file object that can be used to read/write stuff.  Use
// an SDL_RWops* as the underlying implementation.
//
// If autoclose is true, then the sdl_stream has SDL_RWclose()
// called on it when the resulting file object is destructed.
{
	assert(sdl_stream);

	m_data = (void*) sdl_stream;
	m_read = sdl_read_func;
	m_write = sdl_write_func;
	m_seek = sdl_seek_func;
	m_seek_to_end = sdl_seek_to_end_func;
	m_tell = sdl_tell_func;
	m_get_eof = sdl_get_eof_func;
	m_close = autoclose ? sdl_close_func : NULL;
	m_error = TU_FILE_NO_ERROR;
}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
#endif //#ifdef TU_USE_SDL
