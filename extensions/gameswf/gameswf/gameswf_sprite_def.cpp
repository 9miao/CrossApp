// gameswf_sprite_def.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation for SWF player.

// Useful links:
//
// http://sswf.sourceforge.net/SWFalexref.html
// http://www.openswf.org

//
// sprite_definition
//


// A sprite is a mini movie-within-a-movie.  It doesn't define
// its own characters; it uses the characters from the parent
// movie, but it has its own frame counter, display list, etc.
//
// The sprite implementation is divided into a
// sprite_definition and a sprite_instance.  The _definition
// holds the immutable data for a sprite, while the _instance
// contains the state for a specific instance being updated
// and displayed in the parent movie's display list.

#include "gameswf_stream.h"
#include "gameswf_dlist.h"
#include "gameswf_sprite_def.h"

#if TU_CONFIG_LINK_TO_ZLIB
#include <zlib.h>
#endif // TU_CONFIG_LINK_TO_ZLIB


namespace gameswf
{

	sprite_definition::sprite_definition(player* player, movie_definition_sub* m) : 
		movie_definition_sub(player),
		m_movie_def(m)
	{
		// create empty sprite_definition (it is used for createEmptyMovieClip() method)
		if (m == NULL)
		{
			set_frame_count(1);
			inc_loading_frame();
			m_playlist.resize(1);
			m_playlist[0].push_back(new execute_tag());
		}
		else
		{
			assert(m_movie_def);
		}
	}

	sprite_definition::~sprite_definition()
	{
		break_loading();
		// Release our playlist data.
		{for (int i = 0, n = m_playlist.size(); i < n; i++)
		{
			for (int j = 0, m = m_playlist[i].size(); j < m; j++)
			{
				delete m_playlist[i][j];
			}
		}}
	}

	void	sprite_definition::read(stream* in)
		// Read the sprite info.  Consists of a series of tags.
	{
		int	tag_end = in->get_tag_end_position();

		set_frame_count(in->read_u16());

		m_playlist.resize(get_frame_count());	// need a playlist for each frame

		IF_VERBOSE_PARSE(log_msg("  frames = %d\n", get_frame_count()));

		while ((Uint32) in->get_position() < (Uint32) tag_end && get_break_loading() == false)
		{
			int	tag_type = in->open_tag();
			loader_function lf = NULL;
			if (tag_type == 1)
			{
				// show frame tag -- advance to the next frame.
				IF_VERBOSE_PARSE(log_msg("  show_frame (sprite)\n"));
				inc_loading_frame();
			}
			else if (get_tag_loader(tag_type, &lf))
			{
				// call the tag loader.	 The tag loader should add
				// characters or tags to the movie data structure.
				(*lf)(in, tag_type, this);
			}
			else
			{
				// no tag loader for this tag type.
				log_msg("*** no tag loader for type %d\n", tag_type);
			}

			in->close_tag();
		}

		IF_VERBOSE_PARSE(log_msg("  -- sprite END --\n"));
	}
}
