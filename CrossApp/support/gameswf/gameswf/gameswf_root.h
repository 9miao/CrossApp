// gameswf_root.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.

//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//


#ifndef GAMESWF_ROOT_H
#define GAMESWF_ROOT_H


#include "gameswf.h"
#include "gameswf_action.h"
#include "gameswf_types.h"
#include "gameswf_log.h"
#include "gameswf_character.h"
#include "gameswf_mutex.h"
#include "gameswf_listener.h"
#include "gameswf_player.h"
#include <assert.h>
#include "../base/container.h"
#include "../base/utility.h"
#include "../base/weak_ptr.h"
#include <stdarg.h>

namespace gameswf
{
	exported_module tu_mutex& gameswf_engine_mutex();

	struct movie_def_impl;

	//
	// Helper to generate mouse events, given mouse state & history.
	//

	struct mouse_button_state
	{
		gc_ptr<character> m_active_entity;	// entity that currently owns the mouse pointer
		gc_ptr<character> m_topmost_entity;	// what's underneath the mouse right now

		bool	m_mouse_button_state_last;		// previous state of mouse button
		bool	m_mouse_button_state_current;		// current state of mouse button

		bool	m_mouse_inside_entity_last;	// whether mouse was inside the active_entity last frame

		mouse_button_state() :
			m_mouse_button_state_last(0),
			m_mouse_button_state_current(0),
			m_mouse_inside_entity_last(false)
		{
		}
	};

	//
	// root
	//
	// Global, shared root state for a movie and all its characters.
	//
	struct root : public ref_counted
	{
		gc_ptr<movie_def_impl>	m_def;
		gc_ptr<character>	m_movie;
		int			m_viewport_x0, m_viewport_y0, m_viewport_width, m_viewport_height;
		float		m_pixel_scale;

		rgba		m_background_color;
		int			m_mouse_x, m_mouse_y, m_mouse_buttons;
		void*		m_userdata;
		character::drag_state	m_drag_state;	// @@ fold this into m_mouse_button_state?
		mouse_button_state m_mouse_button_state;
		bool		m_on_event_load_called;
		bool        m_shift_key_state;

		gc_ptr<character> m_current_active_entity;
		float	m_time_remainder;
		float m_frame_time;

		// listeners
		listener m_keypress_listener;
		listener m_listener;
		listener m_mouse_listener;

		gameswf::weak_ptr<player> m_player;

		root(player* player, movie_def_impl* def);
		~root();

		void set_active_entity(character* ch);
		void	generate_mouse_button_events(mouse_button_state* ms);
		void	set_root_movie(character* root_movie);

		exported_module void	set_display_viewport(int x0, int y0, int w, int h);
		exported_module void	notify_mouse_state(int x, int y, int buttons);
		virtual void	get_mouse_state(int* x, int* y, int* buttons);
		exported_module character*	get_root_movie() const;

		void	do_mouse_drag();
		void	set_drag_state(character::drag_state& ds);
		void	stop_drag();
		movie_definition*	get_movie_definition();

		exported_module int	get_current_frame() const;
		exported_module int	get_frame_count() { return get_root_movie()->get_frame_count(); }
		float	get_frame_rate() const;
		void	set_frame_rate(float rate);

		virtual float	get_pixel_scale() const;

		character*	get_character(int character_id);
		exported_module void	set_background_color(const rgba& color);
		exported_module void	set_background_alpha(float alpha);
		float	get_background_alpha() const;
		exported_module void	advance(float delta_time);

		exported_module void	goto_frame(int target_frame_number);
		virtual bool	has_looped() const;

		exported_module void	display(float*mat);

		virtual bool	goto_labeled_frame(const char* label);
		virtual void	set_play_state(character::play_state s);
		virtual void	set_play_run();
        virtual void	set_play_stop();

		virtual character::play_state	get_play_state() const;
		virtual void	set_variable(const char* path_to_var, const char* new_value);
		virtual void	set_variable(const char* path_to_var, const wchar_t* new_value);
		virtual const char*	get_variable(const char* path_to_var) const;
		virtual const char*	call_method(const char* method_name, const char* method_arg_fmt, ...);
		virtual const char*	call_method_args(const char* method_name, const char* method_arg_fmt, 
			va_list args);
		virtual tu_string	call_method(const char* method_name, as_value * arguments, int argument_count );

		virtual void	set_visible(bool visible);
		virtual bool	get_visible() const;

		virtual void* get_userdata();
		virtual void set_userdata(void * ud );

		virtual void	attach_display_callback(const char* path_to_object,
			void (*callback)(void* user_ptr), void* user_ptr);

		virtual int	get_movie_version();
		virtual int	get_movie_width();
		virtual int	get_movie_height();
		virtual float	get_movie_fps();

		// External interface for the host to report key events.
		void	notify_key_event(player* player, key::code k, bool down);

		void set_flash_vars(const tu_string& vars);
		bool is_avm2() const;

		void add_listener(as_object* obj)  { m_listener.add(obj); }
		bool remove_listener(as_object* obj) { return m_listener.remove(obj); }
		void add_mouse_listener(as_object* obj)  { m_mouse_listener.add(obj); }
		bool remove_mouse_listener(as_object* obj) { return m_mouse_listener.remove(obj); }
		void add_keypress_listener(as_object* obj)  { m_keypress_listener.add(obj); }
		bool remove_keypress_listener(as_object* obj) { return m_keypress_listener.remove(obj); }
	};
}

#endif
