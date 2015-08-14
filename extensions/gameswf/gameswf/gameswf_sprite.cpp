// gameswf_sprite.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

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

#include "gameswf_action.h"
#include "gameswf_impl.h"
#include "gameswf_stream.h"
#include "gameswf_sprite_def.h"
#include "gameswf_sprite.h"
#include "gameswf_as_sprite.h"
#include "gameswf_text.h"
#include "gameswf_as_classes/as_string.h"

namespace gameswf
{

	struct as_mcloader;

	const char*	next_slash_or_dot(const char* word);
	void	execute_actions(as_environment* env, const swf_array<action_buffer*>& action_list);

	// this stuff should be high optimized
	// thus I can't use here set_member(...);
	sprite_instance::sprite_instance(player* player, movie_definition_sub* def,	root* r, character* parent, int id) :
		character(player, parent, id),
		m_def(def),
		m_root(r),
		m_play_state(PLAY),
		m_current_frame(0),
		m_update_frame(true), 
		m_as_environment(player),
		m_mouse_state(UP),
		m_enabled(true),
		m_on_event_load_called(false),
		m_script(NULL)
	{
		assert(m_def != NULL);
		assert(m_root != NULL);

		//m_root->add_ref();	// @@ circular!
		m_as_environment.set_target(this);
		
		// Initialize the flags for init action executed.
		m_init_actions_executed.resize(m_def->get_frame_count());
		memset(&m_init_actions_executed[0], 0,
			sizeof(m_init_actions_executed[0]) * m_init_actions_executed.size());

		m_player->set_alive(this);
		set_ctor(as_global_movieclip_ctor);
	}

	sprite_instance::~sprite_instance()
	{
		delete m_script;
	}

	bool sprite_instance::has_keypress_event()
	{
		as_value unused;
		return get_member("onKeyPress", &unused);
	}

	void sprite_instance::get_bound(rect* bound)
	{
		int i, n = m_display_list.size();
		if (n == 0)
		{
			return;
		}

		bound->m_x_min = FLT_MAX;
		bound->m_x_max = - FLT_MAX;
		bound->m_y_min = FLT_MAX;
		bound->m_y_max = - FLT_MAX;

		matrix m = get_matrix();
		for (i = 0; i < n; i++)
		{
			character* ch = m_display_list.get_character(i);
			if (ch != NULL)
			{
				rect ch_bound;
				ch->get_bound(&ch_bound);

				m.transform(&ch_bound);

				bound->expand_to_rect(ch_bound);
			}
		}
	}

	character* sprite_instance::add_empty_movieclip(const char* name, int depth)
	{
		cxform color_transform;
		matrix matrix;

		// empty_sprite_def will be deleted during deliting sprite
		sprite_definition* empty_sprite_def = new sprite_definition(get_player(), NULL);

		sprite_instance* sprite =	new sprite_instance(get_player(), empty_sprite_def, m_root, this, 0);
		sprite->set_name(name);

		m_display_list.add_display_object( sprite, depth, true, color_transform, matrix, 0.0f, 0, 0); 

		return sprite;
	}

	void	sprite_instance::set_play_state(play_state s)
	// Stop or play the sprite.
	{
		// pauses stream sound
		sound_handler* sound = get_sound_handler();
		if (sound)
		{
			if (m_def->m_ss_id >= 0)
			{
				sound->pause(m_def->m_ss_id, m_play_state == PLAY ? true : false);
			}
		}
		m_play_state = s;
	}

	sprite_instance::play_state	sprite_instance::get_play_state() const
	{
		return m_play_state; 
	}

	// Functions that qualify as mouse event handlers.

	bool sprite_instance::can_handle_mouse_event()
	// Return true if we have any mouse event handlers.
	{
		static const tu_stringi FN_NAMES[] =
		{
			"onKeyPress",
			"onRelease",
			"onDragOver",
			"onDragOut",
			"onPress",
			"onReleaseOutside",
			"onRollout",
			"onRollover",
		};

		if (is_enabled())
		{
			for (size_t i = 0; i < TU_ARRAYSIZE(FN_NAMES); i++)
			{
				as_value dummy;
				if (get_member(FN_NAMES[i], &dummy))
				{
					return true;
				}
			}
		}
		return false;
	}


	bool sprite_instance::get_topmost_mouse_entity( character * &top_ent, float x, float y)
	// Return the topmost entity that the given point
	// covers that can receive mouse events.  NULL if
	// none.  Coords are in parent's frame.
	{
		if (get_visible() == false) {
			return NULL;
		}

		matrix	m = get_matrix();
		point	p;
		m.transform_by_inverse(&p, point(x, y));

		character*	top_te = NULL;
		bool this_has_focus = false;
		int i, n = m_display_list.size();

		top_ent = NULL;

		// Go backwards, to check higher objects first.
		for (i = n - 1; i >= 0; i--)
		{
			character* ch = m_display_list.get_character(i);
			character* te = NULL;

			if (ch != NULL && ch->get_visible())
			{
				if (ch->get_topmost_mouse_entity(te, p.m_x, p.m_y))
				{
					this_has_focus = true;
					// The containing entity that 1) is closest to root and 2) can
					// handle mouse events takes precedence.
					if (te && te->can_handle_mouse_event())
					{
						top_te = te;
						break;
					}
				}
			}
		}

		//  THIS is closest to root
		if (this_has_focus && can_handle_mouse_event())
		{
			top_ent = this;
		}
		else
		// else character which has event is closest to root
		if (top_te)
		{
			top_ent = top_te;
		}

		// else if we have focus then return not NULL
		return this_has_focus;
	}


	// This code is very tricky and hard to get right.  It should
	// only be changed when verified by an automated test.  Here
	// is the procedure:
	//
	// 1. Identify a bug or desired feature.
	//
	// 2. Create a .swf file that isolates the bug or new feature.
	// The .swf should use trace() statements to log what it is
	// doing and define the correct behavior.
	//
	// 3. Collect the contents of flashlog.txt from the standalone
	// Macromedia flash player.  Create a new test file under
	// tests/ where the first line is the name of the new test
	// .swf, and the rest of the file is the correct trace()
	// output.
	//
	// 4. Verify that gameswf fails the new test (by running
	// ./gameswf_batch_test.py tests/name_of_new_test.txt)
	//
	// 5. Fix gameswf behavior.  Add the new test to
	// passing_tests[] in gameswf_batch_test.py.
	//
	// 6. Verify that all the other passing tests still pass!
	// (Run ./gameswf_batch_test.py and make sure all tests are
	// OK!  If not, then the new behavior is actually a
	// regression.)
	void sprite_instance::advance(float delta_time)
	{
		// child movieclip frame rate is the same the root movieclip frame rate
		// that's why it is not needed to analyze 'm_time_remainder'
		if (m_on_event_load_called == false)
		{
			// clip sprite onload 
			// _root.onLoad() will not be executed since do_actions()
			// for frame(0) has not executed yet.
			// _root.onLoad() will be executed later in root::advance()
			m_def->instanciate_registered_class(this);
			on_event(event_id::LOAD);
		}

		// execute actions from gotoAndPlay(n) or gotoAndStop(n) frame
		if (m_goto_frame_action_list.size() > 0)
		{
			execute_actions(&m_as_environment, m_goto_frame_action_list);
			m_goto_frame_action_list.clear();
		}

		// Update current and next frames.
		if (m_play_state == PLAY)
		{
			int prev_frame = m_current_frame;
			if (m_on_event_load_called)
			{
				m_current_frame++;
				if (m_current_frame >= m_def->get_frame_count())
				{
					m_current_frame = 0;
				}
			}

			// Execute the current frame's tags.
			// execute_frame_tags(0) already executed in dlist.cpp 
			if (m_current_frame != prev_frame) 
			{ 
				// Macromedia Flash does not call remove display object tag
				// for 1-st frame therefore we should do it for it :-)
				if (m_current_frame == 0 && m_def->get_frame_count() > 1)
				{
					// affected depths
					const swf_array<execute_tag*>& playlist = m_def->get_playlist(0);
					swf_array<int> affected_depths;
					for (int i = 0; i < playlist.size(); i++)
					{
						int depth = (playlist[i]->get_depth_id_of_replace_or_add_tag()) >> 16;
						if (depth != -1)
						{
							affected_depths.push_back(depth);
						}
					}

					if (affected_depths.size() > 0)
					{
						m_display_list.clear_unaffected(affected_depths);
					}
					else
					{
						m_display_list.clear();
					}
				}
				execute_frame_tags(m_current_frame);
			}
		}

		if (m_on_event_load_called)
		{
			on_event(event_id::ENTER_FRAME);
		}

		do_actions();

		// Advance everything in the display list.
		m_display_list.advance(delta_time);

		m_on_event_load_called = true;

		// 'this' and its variables is not garbage
		this_alive();
		m_player->set_alive(this);

	}

	// this and all children (bush) is not garbage
	// called from button instance only
	void sprite_instance::alive()
	{
		this_alive();
		m_player->set_alive(this);

		// mark display list as useful
		for (int i = 0, n = m_display_list.size(); i < n; i++)
		{
			character*	ch = m_display_list.get_character(i);
			if (ch)
			{
				ch->alive();
			}
		}
	}

	// Execute the tags associated with the specified frame.
	// frame is 0-based
	void sprite_instance::execute_frame_tags(int frame, bool state_only)
	{
		// Keep this (particularly m_as_environment) alive during execution!
		gc_ptr<as_object>	this_ptr(this);

		assert(frame >= 0);
		assert(frame < m_def->get_frame_count());

		m_def->wait_frame(frame);

		// Execute this frame's init actions, if necessary.
		if (m_init_actions_executed[frame] == false)
		{
			const swf_array<execute_tag*>*	init_actions = m_def->get_init_actions(frame);
			if (init_actions && init_actions->size() > 0)
			{
				// Need to execute these actions.
				for (int i= 0; i < init_actions->size(); i++)
				{
					execute_tag*	e = (*init_actions)[i];
					e->execute(this);
				}

				// Mark this frame done, so we never execute these init actions
				// again.
				m_init_actions_executed[frame] = true;
			}
		}

		const swf_array<execute_tag*>&	playlist = m_def->get_playlist(frame);
		for (int i = 0; i < playlist.size(); i++)
		{
			execute_tag*	e = playlist[i];

			if (state_only)
			{
				e->execute_state(this);
			}
			else
			{
				e->execute(this);
			}
		}

		// start stream sound
		if (state_only == false)
		{
			sound_handler* sound = get_sound_handler();
			if (sound)
			{
				if (m_def->m_ss_start == frame)
				{
					if (m_def->m_ss_id >= 0)
					{
						sound->stop_sound(m_def->m_ss_id);
						sound->play_sound(NULL, m_def->m_ss_id, 0);
					}
				}
			}
		}

		// flash9
		set_frame_script(frame);

	}

	// Execute the tags associated with the specified frame, IN REVERSE.
	// I.e. if it's an "add" tag, then we do a "remove" instead.
	// Only relevant to the display-list manipulation tags: add, move, remove, replace.
	void	sprite_instance::execute_frame_tags_reverse(int frame)
	{
		// frame is 0-based
		// Keep this (particularly m_as_environment) alive during execution!
		gc_ptr<as_object>	this_ptr(this);

		assert(frame >= 0);
		assert(frame < m_def->get_frame_count());

		const swf_array<execute_tag*>&	playlist = m_def->get_playlist(frame);
		for (int i = 0; i < playlist.size(); i++)
		{
			execute_tag*	e = playlist[i];
			e->execute_state_reverse(this, frame);
		}
	}


	execute_tag*	sprite_instance::find_previous_replace_or_add_tag(int frame, int depth, int id)
	{
		uint32	depth_id = ((depth & 0x0FFFF) << 16) | (id & 0x0FFFF);

		for (int f = frame - 1; f >= 0; f--)
		{
			const swf_array<execute_tag*>&	playlist = m_def->get_playlist(f);
			for (int i = playlist.size() - 1; i >= 0; i--)
			{
				execute_tag*	e = playlist[i];
				if (e->get_depth_id_of_replace_or_add_tag() == depth_id)
				{
					return e;
				}
			}
		}

		return NULL;
	}

	// Execute any remove-object tags associated with the specified frame.
	// frame is 0-based
	void	sprite_instance::execute_remove_tags(int frame)
	{
		assert(frame >= 0);
		assert(frame < m_def->get_frame_count());

		const swf_array<execute_tag*>&	playlist = m_def->get_playlist(frame);
		for (int i = 0; i < playlist.size(); i++)
		{
			execute_tag*	e = playlist[i];
			if (e->is_remove_tag())
			{
				e->execute_state(this);
			}
		}
	}

	void	sprite_instance::do_actions()
	// Take care of this frame's actions.
	{
		// Keep m_as_environment alive during any method calls!
		gc_ptr<as_object>	this_ptr(this);

		execute_actions(&m_as_environment, m_action_list);
		m_action_list.resize(0);

		// flash9
		if (m_frame_script != NULL)
		{
			// run frame script once per frame
			gameswf::call_method(m_frame_script.get_ptr(), &m_as_environment, this, 0, 0);
			m_frame_script = NULL;
		}
	}

	void	sprite_instance::set_frame_script(int frame)
	{
		m_frame_script = NULL;
		if (m_script)
		{
			m_script->get(frame, &m_frame_script);
		}
	}

	void sprite_instance::do_actions(const swf_array<action_buffer*>& action_list)
	{
		for (int i = 0; i < action_list.size(); i++)
		{
			action_list[i]->execute(&m_as_environment);
		}
	}

	void sprite_instance::goto_frame(const tu_string& target_frame)
	{
		// Flash tries to convert STRING to NUMBER,
		// if the conversion is OK then Flash uses this NUMBER as target_frame.
		// else uses arg as label of target_frame
		// Thanks Francois Guibert
		double number_value;

		// try string as number
		if (string_to_number(&number_value, target_frame.c_str()))
		{
			goto_frame((int) number_value - 1);	// Convert to 0-based
		}
		else
		{
			goto_labeled_frame(target_frame.c_str());
		}
	}

	// Set the sprite state at the specified frame number.
	// 0-based frame numbers!!  (in contrast to ActionScript and Flash MX)
	void sprite_instance::goto_frame(int target_frame_number)
	{
		//default property is to stop on goto frame
		m_play_state = STOP;

		// Macromedia Flash ignores goto_frame(bad_frame)
		if (target_frame_number > m_def->get_frame_count() - 1 || target_frame_number < 0 || target_frame_number == m_current_frame)	// to prevent infinitive recursion
		{
			return;
		}

		if (target_frame_number < m_current_frame)
		{
			for (int f = m_current_frame; f > target_frame_number; --f)
			{
				execute_frame_tags_reverse(f);	
			}
		}
		else
		{
			for (int f = m_current_frame + 1; f < target_frame_number; ++f)
			{
				execute_frame_tags(f, true);
			}
		}

		m_action_list.clear();
		execute_frame_tags(target_frame_number, false);

		m_current_frame = target_frame_number;

		// actions from gotoFrame() will be executed in advance()
		// Macromedia Flash does goto_frame then run actions from this frame.
		// We do too.
		m_goto_frame_action_list = m_action_list;
	}


	// Look up the labeled frame, and jump to it.
	bool sprite_instance::goto_labeled_frame(const char* label)
	{
		int	target_frame = -1;
		if (m_def->get_labeled_frame(label, &target_frame))
		{
			goto_frame(target_frame);
			return true;
		}
		else
		{
			IF_VERBOSE_ACTION(log_error("error: movie_impl::goto_labeled_frame('%s') unknown label\n", label));
			return false;
		}
	}

	void sprite_instance::display()
	{
		if (get_visible() == false)
		{
			// We're invisible, so don't display!
			return;
		}

		// force advance just loaded (by loadMovie(...)) movie
		if (m_on_event_load_called == false)
		{
			advance(1);
		}

		// is the movieclip masked ?
		if (m_mask_clip != NULL)
		{
			render::begin_submit_mask();

			m_mask_clip->set_visible(true);
			m_mask_clip->display();
			m_mask_clip->set_visible(false);

			render::end_submit_mask();

			m_display_list.display();

			render::disable_mask();
		}
		else
		{
			m_display_list.display();
		}

		do_display_callback();
	}

	character* sprite_instance::add_display_object( Uint16 character_id, const tu_string& name,
		const swf_array<swf_event*>& event_handlers, int depth, bool replace_if_depth_is_occupied,
		const cxform& color_transform, const matrix& matrix, float ratio, Uint16 clip_depth, Uint8 blend_mode)
		// Add an object to the display list.
	{
		assert(m_def != NULL);

		character_def*	cdef = m_def->get_character_def(character_id);
		if (cdef == NULL)
		{
			log_error("sprite::add_display_object(): unknown cid = %d\n", character_id);
			return NULL;
		}

		// If we already have this object on this
		// plane, then move it instead of replacing it.
		character*	existing_char = m_display_list.get_character_at_depth(depth);
		if (existing_char && character_id == existing_char->get_id() && name == existing_char->get_name())
		{
			move_display_object(depth, true, color_transform, true, matrix, ratio, clip_depth, blend_mode);
			return NULL;
		}

		assert(cdef);
		gc_ptr<character>	ch = cdef->create_character_instance(this, character_id);
		assert(ch != NULL);
		ch->set_name(name);

		// Attach event handlers (if any).
		for (int i = 0, n = event_handlers.size(); i < n; i++)
		{
			const tu_stringi& name = event_handlers[i]->m_event.get_function_name();
			ch->set_member(name, event_handlers[i]->m_method);
		}

		m_display_list.add_display_object( ch.get_ptr(), depth, replace_if_depth_is_occupied, color_transform,
			matrix, ratio, clip_depth, blend_mode);

		assert(ch == NULL || gc_collector::debug_get_ref_count(ch) > 1);
		return ch.get_ptr();
	}


	void sprite_instance::move_display_object( int depth, bool use_cxform, const cxform& color_xform,
			bool use_matrix, const matrix& mat, float ratio, Uint16 clip_depth, Uint8 blend_mode)
		// Updates the transform properties of the object at
		// the specified depth.
	{
		m_display_list.move_display_object(depth, use_cxform, color_xform, use_matrix, mat, ratio, clip_depth, blend_mode);
	}


	/*sprite_instance*/
	void sprite_instance::replace_display_object( Uint16 character_id, const char* name, int depth,
		bool use_cxform, const cxform& color_transform, bool use_matrix, const matrix& mat, float ratio,
		Uint16 clip_depth, Uint8 blend_mode)
	{
		assert(m_def != NULL);

		character_def*	cdef = m_def->get_character_def(character_id);
		if (cdef == NULL)
		{
			log_error("sprite::replace_display_object(): unknown cid = %d\n", character_id);
			return;
		}
		assert(cdef);

		gc_ptr<character>	ch = cdef->create_character_instance(this, character_id);
		assert(ch != NULL);

		if (name != NULL && name[0] != 0)
		{
			ch->set_name(name);
		}

		m_display_list.replace_display_object( ch.get_ptr(), depth, use_cxform, color_transform, use_matrix,
			mat, ratio, clip_depth, blend_mode);

//		ch->on_event(event_id::CONSTRUCT);	// isn't tested
	}

	character* sprite_instance::replace_me(character_def*	def)
	{
		assert(def);
		character* parent = get_parent();

		// is 'this' root ?
		if (parent == NULL)
		{
			log_error("character can't replace _root\n");
			return NULL;
		}

		character* ch = def->create_character_instance(parent, 0);

		ch->set_parent(parent);
		parent->replace_display_object(ch, get_name(), get_depth(), false, get_cxform(), false, get_matrix(),
			get_ratio(), get_clip_depth(), get_blend_mode());

		return ch;
	}

	character* sprite_instance::replace_me(movie_definition* md)
	{
		assert(md);
		character* parent = get_parent();

		// is 'this' root ?
		if (parent == NULL)
		{
			root* new_inst = md->create_instance();
			character* ch = new_inst->get_root_movie();
			m_player->set_root(new_inst);

			return ch;
		}

		sprite_instance* sprite = new sprite_instance(get_player(), cast_to<movie_def_impl>(md), get_root(), parent, -1);

		sprite->set_parent(parent);
		sprite->set_root(get_root());

		parent->replace_display_object( sprite, get_name(), get_depth(), false, get_cxform(), false,
			get_matrix(), get_ratio(), get_clip_depth(), get_blend_mode());

		return sprite;
	}

	void sprite_instance::replace_display_object( character* ch, const char* name, int depth, bool use_cxform,
			const cxform& color_transform, bool use_matrix, const matrix& mat, float ratio, Uint16 clip_depth,
			Uint8 blend_mode)
	{
		assert(ch != NULL);

		if (name != NULL && name[0] != 0)
		{
			ch->set_name(name);
		}

		m_display_list.replace_display_object( ch, depth, use_cxform, color_transform, use_matrix, mat,
			ratio, clip_depth, blend_mode);
	}

	// Remove the object at the specified depth.
	// If id != -1, then only remove the object at depth with matching id.
	void sprite_instance::remove_display_object(int depth, int id)
	{
		m_display_list.remove_display_object(depth, id);
	}

	void sprite_instance::clear_display_objects()
	// Remove all display objects
	{
		m_display_list.clear();
	}

	// Add the given action buffer to the list of action
	// buffers to be processed at the end of the next
	// frame advance.
	void sprite_instance::add_action_buffer(action_buffer* a)
	{
		m_action_list.push_back(a);
	}

	// For debugging -- return the id of the character at the specified depth.
	// Return -1 if nobody's home.
	int	sprite_instance::get_id_at_depth(int depth)
	{
		int	index = m_display_list.get_display_index(depth);
		if (index == -1)
		{
			return -1;
		}

		character*	ch = m_display_list.get_display_object(index).m_character.get_ptr();

		return ch->get_id();
	}

	int	sprite_instance::get_highest_depth()
	{
		return m_display_list.get_highest_depth();
	}

	//
	// ActionScript support
	//

	void sprite_instance::set_variable(const char* path_to_var, const char* new_value)
	{
		assert(m_parent == NULL);	// should only be called on the root movie.

		if (path_to_var == NULL)
		{
			log_error("error: NULL path_to_var passed to set_variable()\n");
			return;
		}
		if (new_value == NULL)
		{
			log_error("error: NULL passed to set_variable('%s', NULL)\n", path_to_var);
			return;
		}

		swf_array<with_stack_entry>	empty_with_stack;
		tu_string	path(path_to_var);
		as_value	val(new_value);

		m_as_environment.set_variable(path, val, empty_with_stack);
	}

	void sprite_instance::set_variable(const char* path_to_var, const wchar_t* new_value)
	{
		if (path_to_var == NULL)
		{
			log_error("error: NULL path_to_var passed to set_variable()\n");
			return;
		}
		if (new_value == NULL)
		{
			log_error("error: NULL passed to set_variable('%s', NULL)\n", path_to_var);
			return;
		}

		assert(m_parent == NULL);	// should only be called on the root movie.

		swf_array<with_stack_entry>	empty_with_stack;
		tu_string	path(path_to_var);
		as_value	val(new_value);

		m_as_environment.set_variable(path, val, empty_with_stack);
	}

	const char*	sprite_instance::get_variable(const char* path_to_var) const
	{
		assert(m_parent == NULL);	// should only be called on the root movie.

		swf_array<with_stack_entry>	empty_with_stack;
		tu_string	path(path_to_var);

		// NOTE: this is static so that the string
		// value won't go away after we return!!!
		// It'll go away during the next call to this
		// function though!!!  NOT THREAD SAFE!
		static as_value	val;

		val = m_as_environment.get_variable(path, empty_with_stack);

		return val.to_string();	// ack!
	}

	// useful for catching of the calls
	bool sprite_instance::set_member(const tu_stringi& name, const as_value& val)
	{
		// first try built-ins sprite properties
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			default:
				break;

			case M_MOUSE_MOVE:
			{
				if (val.is_function())
				{
					m_root->add_mouse_listener(this);
				}
				else
				{
					m_root->remove_mouse_listener(this);
				}
				break;
			}
		
			case M_ENABLED:
			{
				as_value new_val(val);
				call_watcher(name, as_value(m_enabled), &new_val);
				m_enabled = new_val.to_bool();
				return true;
			}
		}

		return character::set_member(name, val);
	}

	// Set *val to the value of the named member and
	// return true, if we have the named member.
	// Otherwise leave *val alone and return false.
	bool sprite_instance::get_member(const tu_stringi& name, as_value* val)
	{

		// first try built-ins sprite methods
		if (get_builtin(BUILTIN_SPRITE_METHOD, name, val))
		{
			return true;
		}

		// then try built-ins sprite properties
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			case M_ENABLED:
			{
				val->set_bool(m_enabled);
				return true;
			}

			case M_CURRENTFRAME:
			{
				int n = get_current_frame();
				if (n >= 0)
				{
					val->set_int(n + 1);
				}
				else
				{
					val->set_undefined();
				}
				return true;
			}
			case M_TOTALFRAMES:
			{
				// number of frames.  Read only.
				int n = get_frame_count();
				if (n >= 0)
				{
					val->set_int(n);
				}
				else
				{
					val->set_undefined();
				}
				return true;
			}
			case M_FRAMESLOADED:
			{
				int n = get_loading_frame();
				if (n >= 0)
				{
					val->set_int(n);
				}
				else
				{
					val->set_undefined();
				}
				return true;
			}
			default:
				break;
		}

		// Not a built-in property.  Check items on our display list.
		character*	ch = m_display_list.get_character_by_name_i(name);
		if (ch)
		{
			// Found object.
			val->set_as_object(ch);
			return true;
		}

		// finally try standart character properties & movieclip variables
		return character::get_member(name, val);
	}

	void sprite_instance::call_frame_actions(const as_value& frame_spec)
	// Execute the actions for the specified frame.	 The
	// frame_spec could be an integer or a string.
	{
		int	frame_number = -1;

		// Figure out what frame to call.
		if (frame_spec.is_string())
		{
			if (m_def->get_labeled_frame(frame_spec.to_string(), &frame_number) == false)
			{
				// Try converting to integer.
				frame_number = frame_spec.to_int();
			}
		}
		else
		{
			// convert from 1-based to 0-based
			frame_number = frame_spec.to_int() - 1;
		}

		if (frame_number < 0 || frame_number >= m_def->get_frame_count())
		{
			// No dice.
			log_error("error: call_frame('%s') -- unknown frame\n", frame_spec.to_string());
			return;
		}

		int	top_action = m_action_list.size();

		// Execute the actions.
		const swf_array<execute_tag*>&	playlist = m_def->get_playlist(frame_number);
		for (int i = 0; i < playlist.size(); i++)
		{
			execute_tag*	e = playlist[i];
			if (e->is_action_tag())
			{
				e->execute(this);
			}
		}

		// Execute any new actions triggered by the tag,
		// leaving existing actions to be executed.
		while (m_action_list.size() > top_action)
		{
			m_action_list[top_action]->execute(&m_as_environment);
			m_action_list.remove(top_action);
		}
		assert(m_action_list.size() == top_action);
	}


	/* sprite_instance */

	void sprite_instance::stop_drag()
	{
		assert(m_parent == NULL);	// we must be the root movie!!!

		m_root->stop_drag();
	}

	character* sprite_instance::clone_display_object(const tu_string& newname, int depth)
	// Duplicate the object with the specified name and add it with a new name 
	// at a new depth.
	{
		sprite_instance* parent = cast_to<sprite_instance>(get_parent());
		sprite_instance* ch = NULL; 
		if (parent) 
		{ 
			// clone a previous external loaded movie ?
			if (get_id() == -1)	
			{
				ch = new sprite_instance(get_player(), cast_to<movie_def_impl>(m_def.get_ptr()), 
					get_root(),	parent,	-1);

				ch->set_parent(parent);
				ch->set_root(get_root());
				ch->set_name(newname);

				parent->m_display_list.add_display_object( ch,  depth, true, get_cxform(),  get_matrix(),
						get_ratio(),  get_clip_depth(), get_blend_mode()); 
			}
			else
			{
				ch = new sprite_instance(get_player(), m_def.get_ptr(), get_root(),	parent,	0);
				ch->set_parent(parent);
				ch->set_root(get_root());
				ch->set_name(newname);

				//TODO: test
				if (m_canvas != NULL)
				{
					*ch->get_canvas() = *get_canvas();
				}

				parent->m_display_list.add_display_object( ch,  depth, true, get_cxform(),  get_matrix(), 
							get_ratio(),  get_clip_depth(), get_blend_mode()); 
			}

			// copy this's members to new created character
			copy_to(ch);

		}
		else
		{
			log_error("can't clone _root\n");
		}
		return ch;
	}

	void sprite_instance::remove_display_object(const tu_string& name)
	// Remove the object with the specified name.
	{
		character* ch = m_display_list.get_character_by_name(name);
		if (ch)
		{
			// @@ TODO: should only remove movies that were created via clone_display_object --
			// apparently original movies, placed by anim events, are immune to this.
			remove_display_object(ch->get_depth(), ch->get_id());
		}
	}

	void sprite_instance::remove_display_object(character* ch)
	// Remove the object with the specified pointer.
	{
		m_display_list.remove_display_object(ch);
	}

	bool sprite_instance::on_event(const event_id& id)
	// Dispatch event handler(s), if any.
	{
		// Keep m_as_environment alive during any method calls!
		gc_ptr<as_object>	this_ptr(this);

		// In ActionScript 2.0, event method names are CASE SENSITIVE.
		// In ActionScript 1.0, event method names are CASE INSENSITIVE.
		const tu_stringi&	method_name = id.get_function_name().to_tu_stringi();
		as_value	method;
		if (get_member(method_name, &method))
		{
			int nargs = 0;
			if (id.m_args)
			{
				nargs = id.m_args->size();
				for (int i = nargs - 1; i >= 0; i--)
				{
					m_as_environment.push((*id.m_args)[i]);
				}
			}

			gameswf::call_method(method, &m_as_environment, this, nargs, 
				m_as_environment.get_top_index());

			m_as_environment.drop(nargs);
			return true;
		}
		return false;
	}

	const char* sprite_instance::call_method_args(const char* method_name, const char* method_arg_fmt, va_list args)
	{
		// Keep m_as_environment alive during any method calls!
		gc_ptr<as_object>	this_ptr(this);

		return call_method_parsed(&m_as_environment, this, method_name, method_arg_fmt, args);
	}
	
	tu_string sprite_instance::call_method(const char* method_name, as_value * arguments, int argument_count )
	{
		return gameswf::call_method( &m_as_environment, this, method_name, arguments, argument_count );
	}

	void sprite_instance::attach_display_callback(const char* path_to_object, void (*callback)(void*), void* user_ptr)
	{
		assert(m_parent == NULL);	// should only be called on the root movie.

		swf_array<with_stack_entry>	dummy;
		as_value	obj = m_as_environment.get_variable(tu_string(path_to_object), dummy);

		character*	m = cast_to<character>(obj.to_object());
		if (m)
		{
			m->set_display_callback(callback, user_ptr);
		}
	}

	bool sprite_instance::hit_test(character* ch)
	{
		rect this_bound;
		get_bound(&this_bound);
		if (m_parent != NULL)
		{
			m_parent->get_world_matrix().transform(&this_bound);
		}

		rect ch_bound;
		ch->get_bound(&ch_bound);
		if (ch->m_parent != NULL)
		{
			ch->m_parent->get_world_matrix().transform(&ch_bound);
		}

		rect r;		// intersection
		r.m_x_min = fmax_swf(this_bound.m_x_min, ch_bound.m_x_min);
		r.m_y_min = fmax_swf(this_bound.m_y_min, ch_bound.m_y_min);
		r.m_x_max = fmin_swf(this_bound.m_x_max, ch_bound.m_x_max);
		r.m_y_max = fmin_swf(this_bound.m_y_max, ch_bound.m_y_max);
		r.twips_to_pixels();

		if (r.m_x_min < r.m_x_max && r.m_y_min < r.m_y_max)
		{
#if TU_USE_FLASH_COMPATIBLE_HITTEST == 1
			return true;
#else

			// this hitTest is not compatible with Flash but
			// it works with absolutely accuracy 
			// if you want hitTest two bitmaps you should trace they into shapes

			rgba background_color(0, 0, 0, 0);
			movie_def_impl* def = cast_to<movie_def_impl>(get_root()->get_movie_definition());

			render::begin_display(background_color,
				get_root()->m_viewport_x0, get_root()->m_viewport_y0,
				get_root()->m_viewport_width, get_root()->m_viewport_height,
				def->m_frame_size.m_x_min, def->m_frame_size.m_x_max,
				def->m_frame_size.m_y_min, def->m_frame_size.m_y_max);

			render::begin_submit_mask();
			display();

			render::begin_submit_mask();
			ch->display();

			bool hittest = render::test_stencil_buffer(r, 2);

			render::disable_mask();
			render::disable_mask();

			render::end_display();

			return hittest;

#endif

		}
		return false;
	}

	bool	sprite_instance::hit_test(double x, double y, bool test_shape)
	{
		rect this_bound;
		
		get_bound(&this_bound);
		
		if (m_parent != NULL)
		{
			m_parent->get_world_matrix().transform(&this_bound);
		}
		
		this_bound.twips_to_pixels();

		if (this_bound.point_test((float) x, (float) y))
		{
			if( !test_shape )
			{
				return true;
			}
			else
			{
				// this hitTest is not compatible with Flash but
				// it works with absolutely accuracy 
				// if you want hitTest two bitmaps you should trace they into shapes

				rgba background_color(0, 0, 0, 0);
				movie_def_impl* def = cast_to<movie_def_impl>(get_root()->get_movie_definition());

				render::begin_display(NULL,background_color,
					get_root()->m_viewport_x0, get_root()->m_viewport_y0,
					get_root()->m_viewport_width, get_root()->m_viewport_height,
					def->m_frame_size.m_x_min, def->m_frame_size.m_x_max,
					def->m_frame_size.m_y_min, def->m_frame_size.m_y_max);

				render::begin_submit_mask();
				display();

				rect r;

				r.m_x_min = (float) x;
				r.m_y_min = (float) y;

				r.m_x_max = r.m_x_min + 1;
				r.m_y_max = r.m_y_min + 1;


				bool hittest = render::test_stencil_buffer(r, 1);

				render::end_submit_mask();
				render::disable_mask();

				render::end_display();

				return hittest;
			}
		}
		return false;
	}

	uint32	sprite_instance::get_file_bytes() const
	{
		movie_def_impl* root_def = cast_to<movie_def_impl>(m_def.get_ptr());
		if (root_def)
		{
			return root_def->get_file_bytes();
		}
		return 0;
	}

	uint32 sprite_instance::get_loaded_bytes() const
	{
		movie_def_impl* root_def = cast_to<movie_def_impl>(m_def.get_ptr());
		if (root_def)
		{
			return root_def->get_loaded_bytes();
		}
		return 0;
	}

	character* sprite_instance::create_text_field(const char* name, int depth, int x, int y, int width, int height)
	// Creates a new, empty text field as a child of the movie clip
	{
		edit_text_character_def* textdef = new edit_text_character_def(get_player(), width, height);

		character* textfield = textdef->create_character_instance(this, 0);
		textfield->set_name(name);

		matrix m;
		m.concatenate_translation(PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y));

		cxform color_transform;
		m_display_list.add_display_object(textfield, depth, true, color_transform, m, 0.0f, 0, 0); 

//		textfield->on_event(event_id::CONSTRUCT);	// isn't tested
		return textfield;
	}

	void sprite_instance::clear_refs(gameswf::hash<as_object*, bool>* visited_objects, as_object* this_ptr)
	{
		// Is it a reentrance ?
		if (visited_objects->get(this, NULL))
		{
			return;
		}

		// will be set in as_object::clear_refs
//		visited_objects->set(this, true);

		as_object::clear_refs(visited_objects, this_ptr);

		// clear display list
		m_display_list.clear_refs(visited_objects, this_ptr);

		// clear self-refs from environment
		m_as_environment.clear_refs(visited_objects, this_ptr);
	}

	sprite_instance* sprite_instance::attach_movie(const tu_string& id, 
		const tu_string name, 
		int depth)
	{

		// check the import.
		character_def* res = find_exported_resource(id);
		if (res == NULL)
		{
			IF_VERBOSE_ACTION(log_msg("import error: resource '%s' is not exported\n", id.c_str()));
			return NULL;
		}

		sprite_definition* sdef = cast_to<sprite_definition>(res);
		if (sdef == NULL)
		{
			return NULL;
		}

		sprite_instance* sprite = new sprite_instance(get_player(), sdef, get_root(), this, -1);
		sprite->set_name(name);

		m_display_list.add_display_object( sprite, depth, true, m_color_transform, m_matrix, 0.0f, 0, 0); 

		sprite->advance(1);	// force advance
		return sprite;
	}

	void sprite_instance::dump(tu_string& tabs)
	{
		tabs += "  ";
		printf("%s*** movieclip 0x%p ***\n", tabs.c_str(), this);
		as_object::dump(tabs);
		m_display_list.dump(tabs);
		tabs.resize(tabs.size() - 2);
	}

	character_def* sprite_instance::find_exported_resource(const tu_string& symbol)
	{
		movie_definition_sub*	def = cast_to<movie_def_impl>(get_movie_definition());
		if (def)
		{
			character_def* res = def->get_exported_resource(symbol);
			if (res)
			{
				return res;
			}
		}

		// try parent 
		character* parent = get_parent();
		if (parent)
		{
			return parent->find_exported_resource(symbol);
		}

		IF_VERBOSE_ACTION(log_msg("can't find exported resource '%s'\n", symbol.c_str()));
		return NULL;
	}

	void sprite_instance::set_fps(float fps)
	{
		get_root()->set_frame_rate(fps);
	}

	void sprite_instance::local_to_global(as_object* obj)
	{
		as_value x;
		obj->get_member("x", &x);
		as_value y;
		obj->get_member("y", &y);
		if (x.is_number() == false || y.is_number() == false)
		{
			return;
		}

		point pt(x.to_float(), y.to_float());
		pt.pixels_to_twips();
		point result;
		matrix m = get_world_matrix();
		m.transform(&result, pt);
		result.twips_to_pixels();

		obj->set_member("x", result.m_x);
		obj->set_member("y", result.m_y);
	}

	void sprite_instance::global_to_local(as_object* obj)
	{
		as_value x;
		obj->get_member("x", &x);
		as_value y;
		obj->get_member("y", &y);
		if (x.is_number() == false || y.is_number() == false)
		{
			return;
		}

		point pt(x.to_float(), y.to_float());
		pt.pixels_to_twips();
		point result;
		matrix m;
		m.set_inverse(get_world_matrix());
		m.transform(&result, pt);
		result.twips_to_pixels();

		obj->set_member("x", result.m_x);
		obj->set_member("y", result.m_y);
	}

	canvas* sprite_instance::get_canvas()
	{
		if (m_canvas == NULL)
		{
			canvas* canvas_def = new canvas(get_player());
			m_canvas = canvas_def->create_character_instance(this, -1);

			matrix identity;
			m_display_list.add_display_object( m_canvas.get_ptr(), get_highest_depth(),
					true, m_color_transform, identity, 0.0f, 0, 0); 
		}
		return cast_to<canvas>(m_canvas->get_character_def());
	}

	void sprite_instance::enumerate(as_environment* env)
	{
		assert(env);

		// enumerate variables
		character::enumerate(env);

		// enumerate characters
		for (int i = 0, n = m_display_list.size(); i<n; i++)
		{
			character* ch = m_display_list.get_character(i);
			if (ch)
			{
				const tu_string& name = ch->get_name();
				if (name.size() > 0)
				{
					env->push(name);
				}
			}
		}
	}

	bool sprite_instance::is_enabled() const
	{
		if (m_enabled)
		{
			// check parent
			character* parent = get_parent();
			if (parent)
			{
				return parent->is_enabled();
			}
			return true;
		}
		return false;
	}

	// flash9
	void sprite_instance::add_script(int frame, as_function* func)
	// frame is 0-based
	{
		if (frame >= 0 && frame < m_def->get_frame_count())
		{
			if (m_script == NULL)
			{
				m_script = new gameswf::hash<int, gc_ptr<as_function> >;
			}
			m_script->set(frame, func);

			if (frame == m_current_frame)
			{
				set_frame_script(frame);
			}
		}
	}
}
