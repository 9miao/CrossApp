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

#ifndef GAMESWF_SPRITE_DEF_H
#define GAMESWF_SPRITE_DEF_H


//#include "gameswf.h"
//#include "container.h"

namespace gameswf
{

	struct sprite_definition : public movie_definition_sub
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_SPRITE_DEF };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return movie_definition_sub::is(class_id);
		}

		movie_definition_sub*	     m_movie_def;		// parent movie.
		swf_array<swf_array<execute_tag*> >  m_playlist;	// movie control events for each frame.
		stringi_hash<int>	     m_named_frames;	// stores 0-based frame #'s

		sprite_definition(player* player, movie_definition_sub* m);
		~sprite_definition();

		// overloads from movie_definition
		virtual float	get_width_pixels() const { return 1; }
		virtual float	get_height_pixels() const { return 1; }
		virtual float	get_frame_rate() const
		{
			if (m_movie_def)
			{
				return m_movie_def->get_frame_rate(); 
			}
			return 0;
		}
		virtual int	get_version() const
		{
			if (m_movie_def)
			{
				return m_movie_def->get_version();
			}
			return 0;
		}

		virtual void	add_abc(tu_string& name, abc_def* abc) { assert(0); };
		virtual void	add_symbol_class(int character_id, const tu_string& class_name) { assert(0); };
		virtual void	add_scene(int character_id, const tu_string& class_name) { assert(0); };
		virtual void	add_frame_label(int character_id, const tu_string& class_name) { assert(0); };
		virtual void	add_character(int id, character_def* ch) { log_error("add_character tag appears in sprite tags!\n"); }
		virtual void	add_font(int id, font* ch) { log_error("add_font tag appears in sprite tags!\n"); }
		virtual font*	get_font(int id) { return m_movie_def->get_font(id); }
		virtual void	set_jpeg_loader(jpeg::input* j_in) { assert(0); }
		virtual jpeg::input*	get_jpeg_loader() { return NULL; }
		virtual bitmap_character_def*	get_bitmap_character(int id) { return m_movie_def->get_bitmap_character(id); }
		virtual void	add_bitmap_character(int id, bitmap_character_def* ch) { log_error("add_bc appears in sprite tags!\n"); }
		virtual sound_sample*	get_sound_sample(int id) { return m_movie_def->get_sound_sample(id); }
		virtual void	add_sound_sample(int id, sound_sample* sam) { log_error("add sam appears in sprite tags!\n"); }

		// @@ would be nicer to not inherit these...
		virtual create_bitmaps_flag	get_create_bitmaps() const { assert(0); return DO_LOAD_BITMAPS; }
		virtual create_font_shapes_flag	get_create_font_shapes() const { assert(0); return DO_LOAD_FONT_SHAPES; }
		virtual int	get_bitmap_info_count() const { assert(0); return 0; }
		virtual bitmap_info*	get_bitmap_info(int i) const { assert(0); return NULL; }
		virtual void	add_bitmap_info(bitmap_info* bi) { assert(0); }

		virtual void	export_resource(const tu_string& symbol, character_def* res) { log_error("can't export from sprite\n"); }
		virtual character_def*	get_exported_resource(const tu_string& sym) { return m_movie_def->get_exported_resource(sym); }
		virtual void	add_import(const tu_string& source_url, int id, const tu_string& symbol) { assert(0); }
		virtual void	visit_imported_movies(import_visitor* v) { assert(0); }
		virtual character_def*	get_character_def(int id)
		{
			return m_movie_def->get_character_def(id);
		}

		virtual void	output_cached_data(tu_file* out, const cache_options& options)
		{
			// Nothing to do.
			return;
		}
		virtual void	input_cached_data(tu_file* in)
		{
			// Nothing to do.
			return;
		}

		virtual root*	create_instance()
		{
			return NULL;
		}

		// overloads from character_def
		virtual character*	create_character_instance(character* parent, int id);


		/* sprite_definition */
		virtual void	add_execute_tag(execute_tag* c)
		{
			m_playlist[get_loading_frame()].push_back(c);
		}

		/* sprite_definition */
		virtual void	add_init_action(int sprite_id, execute_tag* c)
		{
			// Sprite def's should not have do_init_action tags in them!  (@@ correct?)
			log_error("sprite_definition::add_init_action called!  Ignored.\n");
		}

		/* sprite_definition */
		virtual void	add_frame_name(const char* name)
			// Labels the frame currently being loaded with the
			// given name.	A copy of the name string is made and
			// kept in this object.
		{
			tu_string	n = name;
			int	currently_assigned = 0;
			if (m_named_frames.get(n, &currently_assigned) == true)
			{
				log_error("add_frame_name(%d, '%s') -- frame name already assigned to frame %d; overriding\n",
					get_loading_frame(),
					name,
					currently_assigned);
			}
			m_named_frames.set(n, get_loading_frame());	// stores 0-based frame #
		}

		/* sprite_definition */
		bool	get_labeled_frame(const char* label, int* frame_number)
			// Returns 0-based frame #
		{
			return m_named_frames.get(label, frame_number);
		}

		const swf_array<execute_tag*>&	get_playlist(int frame_number)
		// frame_number is 0-based
		{
			return m_playlist[frame_number];
		}

		virtual const swf_array<execute_tag*>*	get_init_actions(int frame_number)
		{
			// Sprites do not have init actions in their
			// playlists!  Only the root movie
			// (movie_def_impl) does (@@ correct?)
			return NULL;
		}

		void	read(stream* in);
		// Read the sprite info.  Consists of a series of tags.
	};
}

#endif
