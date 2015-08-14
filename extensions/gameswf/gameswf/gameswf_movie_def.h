// gameswf_movie_def.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.


#ifndef GAMESWF_MOVIE_DEF_IMPL_H
#define GAMESWF_MOVIE_DEF_IMPL_H


#include "gameswf.h"
#include "gameswf_root.h"
#include "gameswf_mutex.h"
#include "gameswf_abc.h"

#include "../base/container.h"
#include "../base/utility.h"

namespace gameswf
{

	struct action_buffer;
	struct bitmap_character_def;
	struct bitmap_info;
	struct character;
	struct character_def;
	struct display_info;
	struct execute_tag;
	struct font;
	struct root;
	
	//
	// This is the client program's interface to the definition of
	// a movie (i.e. the shared constant source info).
	//
	struct movie_definition : public character_def
	{
		movie_definition(player* player) :
			character_def(player)
		{
		}

		virtual int	get_version() const = 0;
		virtual float	get_width_pixels() const = 0;
		virtual float	get_height_pixels() const = 0;
		virtual int	get_frame_count() const = 0;
		virtual float	get_frame_rate() const = 0;

		// This calls add_ref() on the character internally.
		// Call drop_ref() on the character when you're done with it.
		// Or use smart_ptr<T> from base/smart_ptr.h if you want.
		virtual root*	create_instance() = 0;

		virtual void	output_cached_data(tu_file* out, const cache_options& options) = 0;
		virtual void	input_cached_data(tu_file* in) = 0;

		// Causes this movie def to generate texture-mapped
		// versions of all the fonts it owns.  This improves
		// speed and quality of text rendering.	 The
		// texture-map data is serialized in the
		// output/input_cached_data() calls, so you can
		// preprocess this if you load cached data.

		//
		// (optional) API to support gameswf::create_movie_no_recurse().
		//

		// Call visit_imported_movies() to retrieve a list of
		// names of movies imported into this movie.
		// visitor->visit() will be called back with the name
		// of each imported movie.
		struct import_visitor
		{
			virtual ~import_visitor() {}
			virtual void	visit(const char* imported_movie_filename) = 0;
		};

		virtual void	visit_imported_movies(import_visitor* visitor) = 0;

		//
		// (optional) API to support host-driven creation of textures.
		//
		// Create the movie using gameswf::create_movie_no_recurse(..., DO_NOT_LOAD_BITMAPS),
		// and then initialize each bitmap info via get_bitmap_info_count(), get_bitmap_info(),
		// and bitmap_info::init_*_image() or your own subclassed API.
		//
		// E.g.:
		//
		// // During preprocessing:
		// // This will create bitmap_info's using the rgba, rgb, alpha contructors.
		// my_def = gameswf::create_movie_no_recurse("myfile.swf", DO_LOAD_BITMAPS);
		// int ct = my_def->get_bitmap_info_count();
		// for (int i = 0; i < ct; i++)
		// {
		//	my_bitmap_info_subclass*	bi = NULL;
		//	my_def->get_bitmap_info(i, (bitmap_info**) &bi);
		//	my_precomputed_textures.push_back(bi->m_my_internal_texture_reference);
		// }
		// // Save out my internal data.
		// my_precomputed_textures->write_into_some_cache_stream(...);
		//
		// // Later, during run-time loading:
		// my_precomputed_textures->read_from_some_cache_stream(...);
		// // This will create blank bitmap_info's.
		// my_def = gameswf::create_movie_no_recurse("myfile.swf", DO_NOT_LOAD_BITMAPS);
		//
		// // Push cached texture info into the movie's bitmap_info structs.
		// int	ct = my_def->get_bitmap_info_count();
		// for (int i = 0; i < ct; i++)
		// {
		//	my_bitmap_info_subclass*	bi = (my_bitmap_info_subclass*) my_def->get_bitmap_info(i);
		//	bi->set_internal_texture_reference(my_precomputed_textures[i]);
		// }
		virtual int	get_bitmap_info_count() const = 0;
		virtual bitmap_info*	get_bitmap_info(int i) const = 0;
	};


	// Extra internal interfaces added to movie_definition
	struct movie_definition_sub : public movie_definition 
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_MOVIE_DEF_SUB };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return movie_definition::is(class_id);
		}

		movie_definition_sub(player* player) :
			movie_definition(player),
			m_ss_id(-1),
			m_ss_format(sound_handler::FORMAT_RAW),
			m_ss_start(-1),
			m_has_metadata(false),
			m_is_avm2(false),
			m_use_network(false),
			m_frame_count(0),
			m_loading_frame(0),
			m_break_loading(false)
		{
		}

		~movie_definition_sub()
		{
			break_loading();
			sound_handler* sound = get_sound_handler();
			if (sound)
			{
				sound->delete_sound(m_ss_id);
			}
		}

		void	wait_frame(int frame)
		{
			if (is_multithread())
			{
				while (frame > m_loading_frame - 1)
				{
//					printf("wait for frame %d, loaded %d\n", frame + 1, m_loading_frame);
					m_frame.wait();
				}
			}
		}

		inline int	get_loading_frame() const
		{
			assert(m_loading_frame >= 0 && m_loading_frame <= m_frame_count);
			return m_loading_frame; 
		}

		void	inc_loading_frame() 
		{ 
			m_loading_frame++;
			if (is_multithread())
			{
//				printf("signal for frame %d\n", m_loading_frame);
				signal();
			}
		}

		inline int	get_frame_count() const { return m_frame_count; }
		void	set_frame_count(int frames)
		{
			m_frame_count = frames;

			// ALEX: some SWF files have been seen that have 0-frame sprites.
			// The Macromedia player behaves as if they have 1 frame.
			if (m_frame_count < 1)
			{
				m_frame_count = 1;
			}
		}

		virtual const swf_array<execute_tag*>&	get_playlist(int frame_number) = 0;
		virtual const swf_array<execute_tag*>*	get_init_actions(int frame_number) = 0;
		virtual character_def*	get_exported_resource(const tu_string& symbol) = 0;
		virtual character_def*	get_character_def(int id) = 0;

		virtual bool	get_labeled_frame(const char* label, int* frame_number) = 0;

		// For use during creation.
		virtual void	add_symbol_class(int character_id, const tu_string& class_name) = 0;
		virtual void	add_scene(int character_id, const tu_string& class_name) = 0;
		virtual void	add_frame_label(int character_id, const tu_string& class_name) = 0;
		virtual void	add_abc(tu_string& name, abc_def* abc) = 0;
		virtual void	add_character(int id, character_def* ch) = 0;
		virtual void	add_font(int id, font* ch) = 0;
		virtual font*	get_font(int id) = 0;
		virtual void	add_execute_tag(execute_tag* c) = 0;
		virtual void	add_init_action(int sprite_id, execute_tag* c) = 0;
		virtual void	add_frame_name(const char* name) = 0;
		virtual void	set_jpeg_loader(jpeg::input* j_in) = 0;
		virtual jpeg::input*	get_jpeg_loader() = 0;
		virtual bitmap_character_def*	get_bitmap_character(int character_id) = 0;
		virtual void	add_bitmap_character(int character_id, bitmap_character_def* ch) = 0;
		virtual sound_sample*	get_sound_sample(int character_id) = 0;
		virtual void	add_sound_sample(int character_id, sound_sample* sam) = 0;
		virtual void	export_resource(const tu_string& symbol, character_def* res) = 0;
		virtual void	add_import(const tu_string& source_url, int id, const tu_string& symbol_name) = 0;
		virtual void	add_bitmap_info(bitmap_info* ch) = 0;

		virtual create_bitmaps_flag	get_create_bitmaps() const = 0;
		virtual create_font_shapes_flag	get_create_font_shapes() const = 0;
		virtual bool is_multithread() const { return false; }

		// it's used when user has pressed Esc button to break the loading of the .swf file
		bool get_break_loading() const { return m_break_loading; }
		void break_loading() { m_break_loading = true; }

                // TODO: make these private!
		// stream sound variables
		int m_ss_id;	// stream sound handler_id
		sound_handler::format_type m_ss_format;	// used in tag 19
		int m_ss_start;	// start frame

		// flash9
		bool m_has_metadata;
		bool m_is_avm2;
		bool m_use_network;

	private:

		void	signal()
		{
			// printf("signal %d frames is loaded\n", m_loading_frame);
			m_frame.signal();
		}

		int	m_frame_count;
		int	m_loading_frame;
		tu_condition m_frame;
		bool m_break_loading;
	};

	//
	// Helper for movie_def_impl
	//
	struct import_info
	{
		tu_string	m_source_url;
		int		m_character_id;
		tu_string	m_symbol;

		import_info():
			m_character_id(-1)
		{
		}

		import_info(const tu_string& source, int id, const tu_string& symbol):
			m_source_url(source),
			m_character_id(id),
			m_symbol(symbol)
		{
		}
	};


	//
	// movie_def_impl
	//
	// This class holds the immutable definition of a movie's
	// contents.  It cannot be played directly, and does not hold
	// current state; for that you need to call create_instance()
	// to get a movie_instance.
	//
	struct movie_def_impl : public movie_definition_sub
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_MOVIE_DEF };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return movie_definition_sub::is(class_id);
		}

		gameswf::hash<int, gc_ptr<character_def> >	m_characters;
		gameswf::hash<int, gc_ptr<font> >	 m_fonts;
		gameswf::hash<int, gc_ptr<bitmap_character_def> >	m_bitmap_characters;
		gameswf::hash<int, gc_ptr<sound_sample> >	m_sound_samples;
		swf_array<swf_array<execute_tag*> >	   m_playlist;	// A list of movie control events for each frame.
		swf_array<swf_array<execute_tag*> >	   m_init_action_list;	// Init actions for each frame.
		stringi_hash<int>		   m_named_frames;	// 0-based frame #'s
		stringi_hash<gc_ptr<character_def> > m_exports;

		// Items we import.
		swf_array<import_info>	m_imports;

		// Movies we import from; hold a ref on these, to keep them alive
		swf_array<gc_ptr<movie_definition> >	m_import_source_movies;

		// Bitmaps used in this movie; collected in one place to make
		// it possible for the host to manage them as textures.
		swf_array<gc_ptr<bitmap_info> >	m_bitmap_list;

		create_bitmaps_flag	m_create_bitmaps;
		create_font_shapes_flag	m_create_font_shapes;

		rect	m_frame_size;
		float	m_frame_rate;
		int	m_version;
		volatile uint32	m_loaded_length;

		jpeg::input*	m_jpeg_in;

		stream*	m_str;
		Uint32	m_file_end_pos;
		tu_file*	m_zlib_in;
		tu_file*	m_origin_in;
		gc_ptr<tu_thread> m_thread;
		gc_ptr<root> m_instance;	// cached movie instance.

		// for AVM2, Flash9
		tu_string m_abc_name;
		gc_ptr<abc_def> m_abc;
		gameswf::hash<int, tu_string>	m_symbol_class;
		gameswf::hash<int, tu_string>	m_scene;
		gameswf::hash<int, tu_string>	m_frame_label;	// this is't labeled_frame !!!

		movie_def_impl(player* player, create_bitmaps_flag cbf, create_font_shapes_flag cfs);
		~movie_def_impl();

		root*	create_instance();
		root*	create_root();
		void clear_instance();

		float	get_frame_rate() const;
		void	set_frame_rate(float rate);
		float	get_width_pixels() const;
		float	get_height_pixels() const;
		virtual int	get_version() const;
		uint32	get_file_bytes() const;
		uint32	get_loaded_bytes() const;
		virtual create_bitmaps_flag	get_create_bitmaps() const;
		virtual create_font_shapes_flag	get_create_font_shapes() const;
		virtual void	add_bitmap_info(bitmap_info* bi);
		virtual int	get_bitmap_info_count() const;
		virtual bitmap_info*	get_bitmap_info(int i) const;
		virtual void	export_resource(const tu_string& symbol, character_def* res);
		virtual character_def*	get_exported_resource(const tu_string& symbol);
		virtual void	add_import(const tu_string& source_url, int id, const tu_string& symbol);
		bool	in_import_table(int character_id);
		virtual void	visit_imported_movies(import_visitor* visitor);
		virtual void	add_abc(tu_string& name, abc_def* abc);
		const abc_def * get_abc() const{ return m_abc.get_ptr(); }
		virtual void	add_symbol_class(int character_id, const tu_string& class_name);
		virtual void	add_scene(int character_id, const tu_string& class_name);
		virtual void	add_frame_label(int character_id, const tu_string& class_name);
		void	add_character(int character_id, character_def* c);
		character_def*	get_character_def(int character_id);
		bool	get_labeled_frame(const char* label, int* frame_number);
		void	add_font(int font_id, font* f);
		font*	get_font(int font_id);
		font*	find_font(const char* name) const;
		bitmap_character_def*	get_bitmap_character(int character_id);
		void	add_bitmap_character(int character_id, bitmap_character_def* ch);
		sound_sample*	get_sound_sample(int character_id);
		virtual void	add_sound_sample(int character_id, sound_sample* sam);
		void	add_execute_tag(execute_tag* e);
		void	add_init_action(int sprite_id, execute_tag* e);
		void	add_frame_name(const char* name);

		void	set_jpeg_loader(jpeg::input* j_in);
		jpeg::input*	get_jpeg_loader();

		virtual const swf_array<execute_tag*>&	get_playlist(int frame_number);

		virtual const swf_array<execute_tag*>*	get_init_actions(int frame_number);
		void	read(tu_file* in);
		void	read_tags();
		void	get_owned_fonts(swf_array<font*>* fonts);

		void	output_cached_data(tu_file* out, const cache_options& options);
		void	input_cached_data(tu_file* in);

		virtual bool is_multithread() const { return m_thread != NULL; }

		// flash9
		virtual as_function* instanciate_class(character* ch) const;
	};

	void movie_def_loader(void* arg);

	typedef void (*loader_function)(stream* input, int tag_type, movie_definition_sub* m);
	bool get_tag_loader(int tag_type, loader_function* lf);

}

#endif
