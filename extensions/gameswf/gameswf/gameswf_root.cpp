// gameswf_root.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation for SWF player.

// Useful links:
//
// http://sswf.sourceforge.net/SWFalexref.html
// http://www.openswf.org

//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//


#include "gameswf_movie_def.h"
#include "gameswf_render.h"
#include "gameswf_root.h"
#include "gameswf_sprite.h"
#include "../base/tu_random.h"

#ifdef _WIN32
#	define stricmp _stricmp
#else
#	define stricmp strcasecmp
#endif                                  

namespace gameswf
{

	static tu_mutex s_gameswf_engine;
	tu_mutex& gameswf_engine_mutex()
	{
		return s_gameswf_engine;
	}

	root::root(player* player, movie_def_impl* def)	:
		m_def(def),
		m_movie(NULL),
		m_viewport_x0(0),
		m_viewport_y0(0),
		m_viewport_width(1),
		m_viewport_height(1),
		m_pixel_scale(1.0f),
		m_background_color(0, 0, 0, 255),
		m_mouse_x(0),
		m_mouse_y(0),
		m_mouse_buttons(0),
		m_userdata(NULL),
		m_on_event_load_called(false),
		m_shift_key_state(false),
		m_current_active_entity(NULL),

		// the first time we needs do advance() and
		// then display() certainly
		m_time_remainder(1.0f),

		m_frame_time(1.0f),
		m_player(player)
	{
		assert(m_def != NULL);
		set_display_viewport(0, 0, (int) m_def->get_width_pixels(), (int) m_def->get_height_pixels());
		m_frame_time = 1.0f / get_frame_rate();
		player->set_root(this);
	}

	root::~root()
	{
		assert(m_movie != NULL);
		m_movie = NULL;

		assert(m_def != NULL);
		m_def = NULL;
	}

	void root::set_active_entity(character* ch)
	{
		m_current_active_entity = ch;
	}

	void	root::generate_mouse_button_events(mouse_button_state* ms)
	{
		gc_ptr<character> active_entity  = ms->m_active_entity; // entity that currently owns the mouse pointer
		gc_ptr<character> topmost_entity = ms->m_topmost_entity;// what's underneath the mouse right now (updated every frame at the beginning)

		// set cursor as passive/active object
		if( active_entity != NULL && active_entity->can_handle_mouse_event() )
		{
			render::set_cursor(render_handler::ACTIVE_CURSOR);
		}
		else
		{
			render::set_cursor(render_handler::SYSTEM_CURSOR);
		}

		if( ms->m_mouse_button_state_last == 1 )
		{
			// Mouse button was down.

			// Handle trackAsMenu dragOver
			if( active_entity == NULL || active_entity->get_track_as_menu() )
			{
				if( (topmost_entity != NULL) && (topmost_entity != active_entity) && topmost_entity->get_track_as_menu() )
				{
					// Transfer to topmost entity, dragOver
					active_entity = topmost_entity;
					active_entity->on_event( event_id::DRAG_OVER );
					ms->m_mouse_inside_entity_last = true;
				}
			}

			// Handle onDragOut, onDragOver
			if( !ms->m_mouse_inside_entity_last && (topmost_entity == active_entity) )
			{
				// onDragOver
				if (active_entity != NULL)
				{
					active_entity->on_event( event_id::DRAG_OVER );
				}
				ms->m_mouse_inside_entity_last = true;
			}
			else if( ms->m_mouse_inside_entity_last && (topmost_entity != active_entity) )
			{
				// onDragOut
				if (active_entity != NULL)
				{
					active_entity->on_event( event_id::DRAG_OUT );
				}
				ms->m_mouse_inside_entity_last = false;
			}

			// Handle onRelease, onReleaseOutside
			if (ms->m_mouse_button_state_current == 0)
			{
				m_mouse_listener.notify(event_id::MOUSE_UP);

				if( topmost_entity == active_entity )
				{
					// onRelease
					if( active_entity != NULL )
					{
						active_entity->on_event( event_id::RELEASE );
					}
				}
				else if( active_entity != NULL && !active_entity->get_track_as_menu() )
				{
					// onReleaseOutside
					active_entity->on_event( event_id::RELEASE_OUTSIDE );
				}
			}
		}

		if (ms->m_mouse_button_state_last == 0)
		{
			// Mouse button was up.

			// mouse up
			if( ms->m_mouse_button_state_current == 0 )
			{
				// New active entity is whatever is below the mouse right now.
				if (topmost_entity != active_entity)
				{
					// onRollOut
					if (active_entity != NULL && ms->m_mouse_inside_entity_last)
					{
						active_entity->on_event( event_id::ROLL_OUT );
					}

					active_entity = topmost_entity;

					// onRollOver
					if (active_entity != NULL)
					{
						active_entity->on_event( event_id::ROLL_OVER );
					}

					ms->m_mouse_inside_entity_last = true;
				}
			}
			else
			// mouse button press
			{
				// onPress

				m_mouse_listener.notify(event_id::MOUSE_DOWN);

				// set/kill focus
				// It's another entity ?
				if (m_current_active_entity != active_entity)
				{
					// First to clean focus
					if (m_current_active_entity != NULL)
					{
						m_current_active_entity->on_event(event_id::KILLFOCUS);
						m_current_active_entity = NULL;
					}

					// Then to set focus
					if (active_entity != NULL)
					{
						if (active_entity->on_event(event_id::SETFOCUS))
						{
							m_current_active_entity = active_entity;
						}
					}
				}

				if (active_entity != NULL)
				{
					active_entity->on_event(event_id::PRESS);
				}
				ms->m_mouse_inside_entity_last = true;
			}
		}

		// Write the (possibly modified) gc_ptr copies back
		// into the state struct.
		ms->m_active_entity = active_entity;
		ms->m_topmost_entity = topmost_entity;
		ms->m_mouse_button_state_last = ms->m_mouse_button_state_current;
	}

	// movie info

	int	root::get_movie_version()
	{
		return m_def->get_version();
	}

	int	root::get_movie_width()
	{
		return (int) m_def->get_width_pixels();
	}

	int	root::get_movie_height()
	{
		return (int) m_def->get_height_pixels();
	}

	float	root::get_movie_fps()
	{
		return m_def->get_frame_rate();
	}

	void notify_key_object(player* player, key::code k, bool down);

	void change_focus_character(character *movie, character *active_entity, bool shift)
	{
		bool killfocus = false;
		int  tab = 0;
		int  lastvalid = -1;

		// lets see if focus is in the root
		int i = 0;
		for (i = 0; movie->get_character(i) != NULL; ++i)
		{
			if (movie->get_character(i)->is(AS_EDIT_TEXT) == false)
			{
				continue;
			}

			if (movie->get_character(i)->can_handle_mouse_event() == false)
			{
				continue;
			}

			if (movie->get_character(i) == active_entity)
			{
				if (lastvalid != -1 && shift)
				{
					movie->get_character(i)->on_event(event_id::KILLFOCUS);
					movie->get_character(lastvalid)->on_event(event_id::SETFOCUS);
				}

				//now kill focus on current active event
				tab = i;

				//now i need to find which event to set focus on
				killfocus = true;
				continue;
			}

			lastvalid = i;

			if (killfocus)
			{
				movie->get_character(tab)->on_event(event_id::KILLFOCUS);
				movie->get_character(i)->on_event(event_id::SETFOCUS);
				killfocus = false;
			}
		}

		if (movie->get_character(i) == NULL)
		{
			for (i = 0; movie->get_character(i) != NULL; ++i)
			{
				if (stricmp(movie->get_character(i)->type_of(), "movieclip") != 0)
				{
					continue;
				}

				change_focus_character(movie->get_character(i), active_entity, shift);
			}
		}
	}

	void	root::notify_key_event(player* player, key::code k, bool down)
	{
		// multithread plugins can call gameswf core therefore we should 
		// use gameswf mutex to lock gameswf engine
		gameswf_engine_mutex().lock();

		// First notify global Key object
		// listeners that uses the last keypressed code
		player->notify_key_object(k, down);

		// Notify keypress listeners.
		if (down)
		{
			m_keypress_listener.notify(event_id(event_id::KEY_PRESS, (key::code) k));
		}

		// A bit of a hack huh
		if( k == key::SHIFT )
		{
			m_shift_key_state = down;
		}

		if (((key::code)k == key::TAB) && down)
		{
			bool killfocus = false;
			int	 tab = 0;
			int  lastvalid = -1;

			// lets see if focus is in the root
			int i = 0;
			for (i = 0; get_character(i) != NULL; ++i)
			{
				character* movie = get_character(i);
				assert(movie);

				if (movie->is(AS_EDIT_TEXT) == false)
				{
					continue;
				}

				if (movie->can_handle_mouse_event() == false)
				{
					continue;
				}

				if (movie == m_current_active_entity.get_ptr())
				{
					if (lastvalid != -1 && m_shift_key_state)
					{
						movie->on_event(event_id::KILLFOCUS);
						get_character(lastvalid)->on_event(event_id::SETFOCUS);
						break;
					}
					//we only want to kill focus if there is another event below this one
					tab = i;

					//now i need to find which event to set focus on
					killfocus = true;
					continue;
				}

				lastvalid = i;

				if (killfocus)
				{
					get_character(tab)->on_event(event_id::KILLFOCUS);
					movie->on_event(event_id::SETFOCUS);
					killfocus = false;
					break;
				}
			}

			if (get_character(i) == NULL)
			{
				for (int i = 0; get_character(i) != NULL; ++i)
				{
					if (stricmp(get_character(i)->type_of(), "movieclip") != 0)
					{
						continue;
					}

					change_focus_character(get_character(i), m_current_active_entity.get_ptr(), m_shift_key_state);
				}
			}
		}

		gameswf_engine_mutex().unlock();
	}

	void	root::set_root_movie(character* root_movie)
	{
		m_movie = root_movie;
		assert(m_movie != NULL);
	}

	void	root::set_display_viewport(int x0, int y0, int w, int h)
	{
		m_viewport_x0 = x0;
		m_viewport_y0 = y0;
		m_viewport_width = w;
		m_viewport_height = h;

		// Recompute pixel scale.
		float	scale_x = m_viewport_width / TWIPS_TO_PIXELS(m_def->m_frame_size.width());
		float	scale_y = m_viewport_height / TWIPS_TO_PIXELS(m_def->m_frame_size.height());
		m_pixel_scale = fmax_swf(scale_x, scale_y);
	}


	void	root::notify_mouse_state(int x, int y, int buttons)
	// The host app uses this to tell the movie where the
	// user's mouse pointer is.
	{
		bool is_mouse_moved = (x !=  m_mouse_x) || (y != m_mouse_y);
		m_mouse_x = x;
		m_mouse_y = y;
		m_mouse_buttons = buttons;

		if (is_mouse_moved)
		{
			m_mouse_listener.notify(event_id::MOUSE_MOVE);
		}
	}

	void	root::get_mouse_state(int* x, int* y, int* buttons)
	// Use this to retrieve the last state of the mouse, as set via
	// notify_mouse_state().  Coordinates are in PIXELS, NOT TWIPS.
	{
		assert(x);
		assert(y);
		assert(buttons);

		*x = m_mouse_x;
		*y = m_mouse_y;
		*buttons = m_mouse_buttons;
	}

	character*	root::get_root_movie() const { return m_movie.get_ptr(); }

	void	root::set_drag_state(character::drag_state& ds)
	{
		m_drag_state = ds;

		// do we need to calculate the offset?
		character* ch = ds.GetCharacter();
		if (ch == NULL || ds.IsLockCentered())
		{
			return;
		}

		// get the character's origin
		point origin(0, 0);
		matrix chMatrix = ch->get_world_matrix();
		point worldOrigin;
		chMatrix.transform( &worldOrigin, origin );

		// Get current mouse coordinates
		int x, y, buttons;
		get_mouse_state(&x, &y, &buttons);

		// ... in 'world' space (i.e. twips)
		point worldMouse(PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y));

		// we need the offset from the origin of the character
		float xOffset = worldMouse.m_x - worldOrigin.m_x;
		float yOffset = worldMouse.m_y - worldOrigin.m_y;
		m_drag_state.SetOffset(xOffset, yOffset);
	}

	void	root::stop_drag()
	{
		m_drag_state.Reset();
	}

	// Implement mouse-dragging for this movie.
	void	root::do_mouse_drag()		
	{
		character* draggingChar = m_drag_state.GetCharacter();
		if (draggingChar == NULL)
		{
			return;
		}

		// handle if the character isn't valid anymore
		if (draggingChar->is_alive() == false)
		{
			// no longer valid
			m_drag_state.Reset();
			return;
		}

		// get the current mouse
		int	x, y, buttons;
		get_mouse_state( &x, &y, &buttons );

		// ... in world coordinates (twips)
		point worldMouse( PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y) );

		matrix parentWorldMat;
		if (draggingChar->m_parent != NULL)
		{
			parentWorldMat = draggingChar->m_parent->get_world_matrix();
		}

		// if we're not locked to the center - adjust by the offset we started dragging by
		if(m_drag_state.IsLockCentered() == false)
		{
			worldMouse.m_x -= m_drag_state.OffsetX();
			worldMouse.m_y -= m_drag_state.OffsetY();
		}

		rect origBoundRect;
		if (m_drag_state.GetBounds(&origBoundRect))
		{
			// bounds are in local coordinate space
			rect bounds;
			bounds.enclose_transformed_rect( parentWorldMat, origBoundRect );

			// Clamp mouse coords within a defined rect.
			worldMouse.m_x = fclamp(worldMouse.m_x, bounds.m_x_min, bounds.m_x_max);
			worldMouse.m_y = fclamp(worldMouse.m_y, bounds.m_y_min, bounds.m_y_max);
		}

		point parentMouse;
		parentWorldMat.transform_by_inverse( &parentMouse, worldMouse );

		// Place our origin so that it coincides with the mouse coords
		// in our parent frame.
		matrix local = draggingChar->get_matrix();
		local.m_[0][2] = parentMouse.m_x;//set translation x
		local.m_[1][2] = parentMouse.m_y;//set translation y
		draggingChar->set_matrix( local );
	}


	movie_definition*	root::get_movie_definition() { return m_movie->get_movie_definition(); }

	// 0-based!!
	int	root::get_current_frame() const { return m_movie->get_current_frame(); }
	float	root::get_frame_rate() const { return m_def->get_frame_rate(); }
	void	root::set_frame_rate(float rate)
	{
		if (rate >= 1 && rate <= 120)
		{
			m_frame_time = 1.0f / rate;
			m_def->set_frame_rate(rate); 
		}
	}

	float	root::get_pixel_scale() const
	// Return the size of a logical movie pixel as
	// displayed on-screen, with the current device
	// coordinates.
	{
		return m_pixel_scale;
	}

	// @@ Is this one necessary?
	character*	root::get_character(int character_id)
	{
		return m_movie->get_character(character_id);
	}

	void	root::set_background_color(const rgba& color)
	{
		m_background_color = color;
	}

	void	root::set_background_alpha(float alpha)
	{
		m_background_color.m_a = iclamp(frnd(alpha * 255.0f), 0, 255);
	}

	float	root::get_background_alpha() const
	{
		return m_background_color.m_a / 255.0f;
	}

	void	root::advance(float delta_time)
	{
		// Lock gameswf engine. Video is running in separate thread and
		// it calls gameswf functions from separate thread to set
		// status of netstream object
		gameswf_engine_mutex().lock();

		// Handle mouse dragging
		do_mouse_drag();

		// Handle the mouse.
		character *te;
		m_movie->get_topmost_mouse_entity(te, PIXELS_TO_TWIPS(m_mouse_x), PIXELS_TO_TWIPS(m_mouse_y));
		m_mouse_button_state.m_topmost_entity = te;

		m_mouse_button_state.m_mouse_button_state_current = (m_mouse_buttons & 1);
		generate_mouse_button_events(&m_mouse_button_state);

		// advance Action script objects (interval timers, xmlsocket, ...)
		m_listener.advance(delta_time);

		m_time_remainder += delta_time;
		if (m_time_remainder >= m_frame_time)
		{

			// mark all as garbage
			m_player->set_as_garbage();

			// this should be called infinitely to not repeat
			// the game situation after restart
			tu_random::next_random();

			if (m_on_event_load_called == false)
			{
				set_flash_vars(m_player->m_flash_vars);
				if (m_def->m_is_avm2)
				{
					const abc_def* adef  = m_def->get_abc();
					if (adef)
					{
						as_environment env(m_player.get_ptr());
						gameswf::call_method(adef->get_script_function(), 
							&env, as_value(m_movie.get_ptr()), 0, 0);
					}
				}
			}

			if (m_player->get_force_realtime_framerate() == true)
			{
				while (m_time_remainder >= m_frame_time)
				{
					m_movie->advance(m_frame_time);
					m_time_remainder -= m_frame_time;
				}
			}
			else
			{
				m_movie->advance(delta_time);
				m_time_remainder = fmod(m_time_remainder - m_frame_time, m_frame_time);
			}

			if (m_on_event_load_called == false)
			{
				// Must do loading events.  For child sprites this is
				// done by the dlist, but root movies don't get added
				// to a dlist, so we do it here.
				m_on_event_load_called = true;
				m_movie->on_event(event_id::LOAD);
			}

			m_player->clear_garbage();
		}

		gameswf_engine_mutex().unlock();
	}

	// 0-based!!
	void	root::goto_frame(int target_frame_number) { m_movie->goto_frame(target_frame_number); }

	bool	root::has_looped() const { return m_movie->has_looped(); }

	void	root::display(float*mat)
	{
		if (m_movie->get_visible() == false)
		{
			// Don't display.
			return;
		}

		gameswf::render::begin_display(mat,
			m_background_color,
			m_viewport_x0, m_viewport_y0,
			m_viewport_width, m_viewport_height,
			m_def->m_frame_size.m_x_min, m_def->m_frame_size.m_x_max,
			m_def->m_frame_size.m_y_min, m_def->m_frame_size.m_y_max);

		m_movie->display();

		gameswf::render::end_display();
	}

	bool	root::goto_labeled_frame(const char* label)
	{
		int	target_frame = -1;
		if (m_def->get_labeled_frame(label, &target_frame))
		{
			goto_frame(target_frame);
			return true;
		}
		else
		{
			IF_VERBOSE_ACTION(
				log_error("error: movie_impl::goto_labeled_frame('%s') unknown label\n", label));
			return false;
		}
	}

	void	root::set_play_state(character::play_state s) { m_movie->set_play_state(s); }
    void root::set_play_run(){set_play_state((character::play_state)1);}
    void root::set_play_stop(){set_play_state((character::play_state)0);}

	character::play_state	root::get_play_state() const { return m_movie->get_play_state(); }

	void	root::set_variable(const char* path_to_var, const char* new_value)
	{
		m_movie->set_variable(path_to_var, new_value);
	}

	void	root::set_variable(const char* path_to_var, const wchar_t* new_value)
	{
		m_movie->set_variable(path_to_var, new_value);
	}

	const char*	root::get_variable(const char* path_to_var) const
	{
		return m_movie->get_variable(path_to_var);
	}

	// For ActionScript interfacing convenience.
	const char*	root::call_method(const char* method_name, const char* method_arg_fmt, ...)
	{
		assert(m_movie != NULL);

		va_list	args;
		va_start(args, method_arg_fmt);
		const char*	result = m_movie->call_method_args(method_name, method_arg_fmt, args);
		va_end(args);

		return result;
	}

	const char*	root::call_method_args(const char* method_name, const char* method_arg_fmt, va_list args)
	{
		assert(m_movie != NULL);
		return m_movie->call_method_args(method_name, method_arg_fmt, args);
	}
	
	tu_string root::call_method(const char* method_name, as_value * arguments, int argument_count )
	{
		assert(m_movie != NULL);
		return m_movie->call_method(method_name, arguments, argument_count);
	}

	void	root::set_visible(bool visible) { m_movie->set_visible(visible); }
	bool	root::get_visible() const { return m_movie->get_visible(); }

	void* root::get_userdata() { return m_userdata; }
	void root::set_userdata(void * ud ) { m_userdata = ud;  }

	void	root::attach_display_callback(const char* path_to_object, void (*callback)(void* user_ptr), void* user_ptr)
	{
		m_movie->attach_display_callback(path_to_object, callback, user_ptr);
	}

	// parse Flash vars and create _root variables
	// Flash vars come in the format:
	// myvar1=value1,myvar2=value2,myvar3=value3,...
	void root::set_flash_vars(const tu_string& vars)
	{
		for (const char* word = vars.c_str(); *word; )
		{
			bool nullDel = false;
			const char* delimiter = strchr(word, '=');
			if (delimiter == NULL)
			{
				// no value
				return;
			}
			tu_string varname(word, int(delimiter - word));

			word = delimiter + 1;
			delimiter = strchr(word, ',');
			if (delimiter == NULL)
			{
				delimiter = vars.c_str() + vars.size();
				nullDel = true;
			}
			tu_string value(word, int(delimiter - word));
		
			get_root_movie()->set_member(varname, value.c_str());

			if (nullDel)
				return;

			word = delimiter + 1;
			
		}

	}

	bool root::is_avm2() const
	{
		return m_def->m_is_avm2;
	}

}
