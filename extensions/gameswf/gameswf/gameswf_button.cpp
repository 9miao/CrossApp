// gameswf_button.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// SWF buttons.  Mouse-sensitive update/display, actions, etc.


#include "gameswf_button.h"
#include "gameswf_action.h"
#include "gameswf_render.h"
#include "gameswf_sound.h"
#include "gameswf_stream.h"
#include "gameswf_movie_def.h"
#include "gameswf_filters.h"

/*

Observations about button & mouse behavior

Entities that receive mouse events: only buttons and sprites, AFAIK

When the mouse button goes down, it becomes "captured" by whatever
element is topmost, directly below the mouse at that moment.  While
the mouse is captured, no other entity receives mouse events,
regardless of how the mouse or other elements move.

The mouse remains captured until the mouse button goes up.  The mouse
remains captured even if the element that captured it is removed from
the display list.

If the mouse isn't above a button or sprite when the mouse button goes
down, then the mouse is captured by the background (i.e. mouse events
just don't get sent, until the mouse button goes up again).

Mouse events:

+------------------+---------------+-------------------------------------+
| Event            | Mouse Button  | description                         |
=========================================================================
| onRollOver       |     up        | sent to topmost entity when mouse   |
|                  |               | cursor initially goes over it       |
+------------------+---------------+-------------------------------------+
| onRollOut        |     up        | when mouse leaves entity, after     |
|                  |               | onRollOver                          |
+------------------+---------------+-------------------------------------+
| onPress          |  up -> down   | sent to topmost entity when mouse   |
|                  |               | button goes down.  onRollOver       |
|                  |               | always precedes onPress.  Initiates |
|                  |               | mouse capture.                      |
+------------------+---------------+-------------------------------------+
| onRelease        |  down -> up   | sent to active entity if mouse goes |
|                  |               | up while over the element           |
+------------------+---------------+-------------------------------------+
| onDragOut        |     down      | sent to active entity if mouse      |
|                  |               | is no longer over the entity        |
+------------------+---------------+-------------------------------------+
| onReleaseOutside |  down -> up   | sent to active entity if mouse goes |
|                  |               | up while not over the entity.       |
|                  |               | onDragOut always precedes           |
|                  |               | onReleaseOutside                    |
+------------------+---------------+-------------------------------------+
| onDragOver       |     down      | sent to active entity if mouse is   |
|                  |               | dragged back over it after          |
|                  |               | onDragOut                           |
+------------------+---------------+-------------------------------------+

There is always one active entity at any given time (considering NULL to
be an active entity, representing the background, and other objects that
don't receive mouse events).

When the mouse button is up, the active entity is the topmost element
directly under the mouse pointer.

When the mouse button is down, the active entity remains whatever it
was when the button last went down.

The active entity is the only object that receives mouse events.

!!! The "trackAsMenu" property alters this behavior!  If trackAsMenu
is set on the active entity, then onReleaseOutside is filtered out,
and onDragOver from another entity is allowed (from the background, or
another trackAsMenu entity). !!!


Pseudocode:

active_entity = NULL
mouse_button_state = UP
mouse_inside_entity_state = false
frame loop:
  if mouse_button_state == DOWN

    // Handle trackAsMenu
    if (active_entity->trackAsMenu)
      possible_entity = topmost entity below mouse
      if (possible_entity != active_entity && possible_entity->trackAsMenu)
        // Transfer to possible entity
	active_entity = possible_entity
	active_entity->onDragOver()
	mouse_inside_entity_state = true;

    // Handle onDragOut, onDragOver
    if (mouse_inside_entity_state == false)
      if (mouse is actually inside the active_entity)
        // onDragOver
	active_entity->onDragOver()
        mouse_inside_entity_state = true;

    else // mouse_inside_entity_state == true
      if (mouse is actually outside the active_entity)
        // onDragOut
	active_entity->onDragOut()
	mouse_inside_entity_state = false;

    // Handle onRelease, onReleaseOutside
    if (mouse button is up)
      if (mouse_inside_entity_state)
        // onRelease
        active_entity->onRelease()
      else
        // onReleaseOutside
	if (active_entity->trackAsMenu == false)
          active_entity->onReleaseOutside()
      mouse_button_state = UP
    
  if mouse_button_state == UP
    new_active_entity = topmost entity below the mouse
    if (new_active_entity != active_entity)
      // onRollOut, onRollOver
      active_entity->onRollOut()
      active_entity = new_active_entity
      active_entity->onRollOver()
    
    // Handle press
    if (mouse button is down)
      // onPress
      active_entity->onPress()
      mouse_inside_entity_state = true
      mouse_button_state = DOWN

*/


namespace gameswf
{
	struct button_character_instance : public character
	{
		gc_ptr<button_character_definition>	m_def;
		swf_array<gc_ptr<character> >	m_record_character;

		enum mouse_flags
		{
			IDLE = 0,
			FLAG_OVER = 1,
			FLAG_DOWN = 2,
			OVER_DOWN = FLAG_OVER|FLAG_DOWN,

			// aliases
			OVER_UP = FLAG_OVER,
			OUT_DOWN = FLAG_DOWN
		};
		int	m_last_mouse_flags, m_mouse_flags;		
		enum e_mouse_state
		{
			UP = 0,
			DOWN,
			OVER
		};
		e_mouse_state m_mouse_state;
		bool m_enabled;

		button_character_instance(player* player, button_character_definition* def, character* parent, int id) :
			character(player, parent, id),
			m_def(def),
			m_last_mouse_flags(IDLE),
			m_mouse_flags(IDLE),
			m_mouse_state(UP),
			m_enabled(true)
		{
			assert(m_def != NULL);

			int r, r_num =  m_def->m_button_records.size();
			m_record_character.resize(r_num);

			movie_definition_sub*	movie_def = cast_to<movie_definition_sub>(get_movie_definition());
 			assert(movie_def);

			for (r = 0; r < r_num; r++)
			{
				button_record*	bdef = &m_def->m_button_records[r];

				if (bdef->m_character_def == NULL)
				{
					// Resolve the character id.
					bdef->m_character_def = movie_def->get_character_def(bdef->m_character_id);
				}
				assert(bdef->m_character_def != NULL);

				const matrix&	mat = m_def->m_button_records[r].m_button_matrix;
				const cxform&	cx = m_def->m_button_records[r].m_button_cxform;

				gc_ptr<character>	ch = bdef->m_character_def->create_character_instance(this, id);
				m_record_character[r] = ch;
				ch->set_matrix(mat);
				ch->set_cxform(cx);

	//			ch->on_event(event_id::CONSTRUCT);	// isn't tested
			}
		}

		~button_character_instance()
		{
		}

		virtual	void	execute_frame_tags(int frame, bool state_only)
		{
			// Keep this (particularly m_as_environment) alive during execution!
			gc_ptr<as_object>	this_ptr(this);
			assert(frame == 0);
			for (int i = 0; i < m_def->m_button_records.size(); i++)
			{
				if (m_record_character[i] != NULL)
				{
					m_record_character[i]->execute_frame_tags(frame, state_only);
				}
			}
		}

		virtual bool has_keypress_event()
		{
			for (int i = 0; i < m_def->m_button_actions.size(); i++)
			{
				if (m_def->m_button_actions[i].m_conditions & 0xFE00)	// check up on CondKeyPress: UB[7]
				{
					return true;
				}
			}	
			return false;
		}

		root*	get_root() { return get_parent()->get_root(); }
		character*	get_root_movie() const { return get_parent()->get_root_movie(); }

		virtual void	advance(float delta_time)
		{
			matrix	mat = get_world_matrix();

			// Advance our relevant characters.
			{for (int i = 0; i < m_def->m_button_records.size(); i++)
			{
				button_record&	rec = m_def->m_button_records[i];
				if (m_record_character[i] == NULL)
				{
					continue;
				}

				// Matrix
				matrix sub_matrix = mat;
				sub_matrix.concatenate(rec.m_button_matrix);

				// Advance characters that are activated by the new mouse state
				if (((m_mouse_state == UP) && (rec.m_up)) ||
					((m_mouse_state == DOWN) && (rec.m_down)) ||
					((m_mouse_state == OVER) && (rec.m_over)))
				{
					m_record_character[i]->advance(delta_time);
				}
				else
				{
					m_record_character[i]->alive();
				}
			}}
		}

		void	display()
		{
			for (int i = 0; i < m_def->m_button_records.size(); i++)
			{
				button_record&	rec = m_def->m_button_records[i];
				if (m_record_character[i] == NULL)
				{
					continue;
				}
				if ((m_mouse_state == UP && rec.m_up)
					|| (m_mouse_state == DOWN && rec.m_down)
					|| (m_mouse_state == OVER && rec.m_over))
				{
					m_record_character[i]->display();
				}
			}

			do_display_callback();
		}

		inline int	transition(int a, int b) const
		// Combine the flags to avoid a conditional. It would be faster with a macro.
		{
			return (a << 2) | b;
		}


		virtual bool get_topmost_mouse_entity( character * &te, float x, float y)
		// Return the topmost entity that the given point covers.  NULL if none.
		// I.e. check against ourself.
		{
			if (get_visible() == false || is_enabled() == false)
			{
				return false;
			}

			point	p;
			get_matrix().transform_by_inverse(&p, point(x, y));

			for (int i = 0; i < m_def->m_button_records.size(); i++)
			{
				button_record&	rec = m_def->m_button_records[i];
				if (rec.m_character_id < 0 || rec.m_hit_test == false)
				{
					continue;
				}

				// Find the mouse position in button-record space.
				point	sub_p;
				rec.m_button_matrix.transform_by_inverse(&sub_p, p);

				if (rec.m_character_def->point_test_local(sub_p.m_x, sub_p.m_y))
				{
					// The mouse is inside the shape.
					te = this;
					return true;
					// @@ Are there any circumstances where this is correct:
					//return m_record_character[i].get_ptr();
				}
			}

			return false;
		}

		virtual bool	on_event(const event_id& id)
		{
			// Keep this & parent alive during execution!
			const gc_ptr<character> this_ptr(this);
			const gc_ptr<character> parent(get_parent());

			// 'this or 'parent' object is died
			if (parent == NULL || m_def == NULL)
			{
				return false;
			}

			as_object::on_event(id);

			const gc_ptr<button_character_definition> def(m_def);

			bool called = false;
			if (id.m_id == event_id::KEY_PRESS)
			{
				static const event_id s_key[32] =
				{
					event_id(),
					event_id(event_id::KEY_PRESS, key::LEFT),
					event_id(event_id::KEY_PRESS, key::RIGHT),
					event_id(event_id::KEY_PRESS, key::HOME),
					event_id(event_id::KEY_PRESS, key::END),
					event_id(event_id::KEY_PRESS, key::INSERT),
					event_id(event_id::KEY_PRESS, key::DELETEKEY),
					event_id(),
					event_id(event_id::KEY_PRESS, key::BACKSPACE),	//8
					event_id(),
					event_id(),
					event_id(),
					event_id(),
					event_id(event_id::KEY_PRESS, key::ENTER),	//13
					event_id(event_id::KEY_PRESS, key::UP),
					event_id(event_id::KEY_PRESS, key::DOWN),
					event_id(event_id::KEY_PRESS, key::PGUP),
					event_id(event_id::KEY_PRESS, key::PGDN),
					event_id(event_id::KEY_PRESS, key::TAB),
					// 32-126 folows ASCII*/
				};

				// Execute appropriate actions

				// actions can delete THIS through execute_frame_tags()
				for (int i = 0; i < def->m_button_actions.size(); i++)
				{
					int keycode = (def->m_button_actions[i].m_conditions & 0xFE00) >> 9;
					event_id key_event = keycode < 32 ? s_key[keycode] : event_id(event_id::KEY_PRESS, (key::code) keycode);
					if (key_event == id)
					{
						parent->do_actions(def->m_button_actions[i].m_actions);
						called = true;
					}
				}
			}
			else
			{
				// Set our mouse state (so we know how to render).
				switch (id.m_id)
				{
				case event_id::ROLL_OUT:
				case event_id::RELEASE_OUTSIDE:
					m_mouse_state = UP;
					break;

				case event_id::RELEASE:
				case event_id::ROLL_OVER:
				case event_id::DRAG_OUT:
					m_mouse_state = OVER;
					break;

				case event_id::PRESS:
				case event_id::DRAG_OVER:
					m_mouse_state = DOWN;
					break;

				default:
					return false;	// unhandled event, like setfocus, ...
				};

				// Button transition sounds.
				if (def->m_sound != NULL)
				{
					int bi; // button sound array index [0..3]
					sound_handler* s = get_sound_handler();

					// Check if there is a sound handler
					if (s != NULL) {
						switch (id.m_id)
						{
						case event_id::ROLL_OUT:
							bi = 0;
							break;
						case event_id::ROLL_OVER:
							bi = 1;
							break;
						case event_id::PRESS:
							bi = 2;
							break;
						case event_id::RELEASE:
							bi = 3;
							break;
						default:
							bi = -1;
							break;
						}
						if (bi >= 0)
						{
							button_character_definition::button_sound_info& bs = def->m_sound->m_button_sounds[bi];
							// character zero is considered as null character
							if (bs.m_sound_id > 0)
							{
								assert(def->m_sound->m_button_sounds[bi].m_sam != NULL);
								if (bs.m_sound_style.m_stop_playback)
								{
									s->stop_sound(bs.m_sam->m_sound_handler_id);
								}
								else
								{
									s->play_sound(NULL, bs.m_sam->m_sound_handler_id, bs.m_sound_style.m_loop_count);
								}
							}
						}
					}
				}

				// @@ eh, should just be a lookup table.
				int	c = 0;
				if (id.m_id == event_id::ROLL_OVER) c |= (button_action::IDLE_TO_OVER_UP);
				else if (id.m_id == event_id::ROLL_OUT) c |= (button_action::OVER_UP_TO_IDLE);
				else if (id.m_id == event_id::PRESS) c |= (button_action::OVER_UP_TO_OVER_DOWN);
				else if (id.m_id == event_id::RELEASE) c |= (button_action::OVER_DOWN_TO_OVER_UP);
				else if (id.m_id == event_id::DRAG_OUT) c |= (button_action::OVER_DOWN_TO_OUT_DOWN);
				else if (id.m_id == event_id::DRAG_OVER) c |= (button_action::OUT_DOWN_TO_OVER_DOWN);
				else if (id.m_id == event_id::RELEASE_OUTSIDE) c |= (button_action::OUT_DOWN_TO_IDLE);
						
				//IDLE_TO_OVER_DOWN = 1 << 7,
				//OVER_DOWN_TO_IDLE = 1 << 8,

				// Execute appropriate actions

				// actions can delete THIS & PARENT through execute_frame_tags()
				for (int i = 0; i < def->m_button_actions.size(); i++)
				{
					if (def->m_button_actions[i].m_conditions & c)
					{
						parent->do_actions(def->m_button_actions[i].m_actions);
						called = true;
					}
				}
			}
			return called;
		}

		virtual void	get_mouse_state(int* x, int* y, int* buttons)
		{
			get_parent()->get_mouse_state(x, y, buttons);
		}

		//
		// ActionScript overrides
		//

		virtual bool	set_member(const tu_stringi& name, const as_value& val)
		{
			as_standard_member	std_member = get_standard_member(name);
			switch (std_member)
			{
				default:
					break;

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

		virtual bool	get_member(const tu_stringi& name, as_value* val)
		{
			// first try character members
			as_standard_member	std_member = get_standard_member(name);
			switch (std_member)
			{
				default:
					break;

				case M_ENABLED:
				{
					val->set_bool( is_enabled() );
					return true;
				}
			}
			return character::get_member(name, val);
		}

		virtual void	get_bound(rect* bound)
		{
			int n = m_def->m_button_records.size();
			if (n == 0)
			{
				return;
			}

			bound->m_x_min = FLT_MAX;
			bound->m_x_max = - FLT_MAX;
			bound->m_y_min = FLT_MAX;
			bound->m_y_max = - FLT_MAX;

			matrix m = get_matrix();
			for (int i = 0; i < n; i++)
			{
				button_record&	rec = m_def->m_button_records[i];
				if (m_record_character[i] == NULL)
				{
					continue;
				}
				if ((m_mouse_state == UP && rec.m_up)
					|| (m_mouse_state == DOWN && rec.m_down)
					|| (m_mouse_state == OVER && rec.m_over))
				{

					rect ch_bound;
					m_record_character[i]->get_bound(&ch_bound);

					m.transform(&ch_bound);

					bound->expand_to_rect(ch_bound);
				}
			}
		}

		virtual bool can_handle_mouse_event()
		{
			return is_enabled();
		}

		virtual	bool is_enabled() const
		{
			return m_enabled;
		}
		
		virtual character_def* get_character_def() { return m_def.get_ptr();	}

	};


	//
	// button_record
	//

	bool	button_record::read(stream* in, int tag_type, movie_definition_sub* m)
	// Return true if we read a record; false if this is a null record.
	{
		int	flags = in->read_u8();
		if (flags == 0)
		{
			return false;
		}
		m_has_blend_mode = flags & 32 ? true : false;
		m_has_filter_list = flags & 16 ? true : false;
		m_hit_test = flags & 8 ? true : false;
		m_down = flags & 4 ? true : false;
		m_over = flags & 2 ? true : false;
		m_up = flags & 1 ? true : false;

		m_character_id = in->read_u16();
		m_character_def = NULL;
		m_button_layer = in->read_u16(); 
		m_button_matrix.read(in);

		if (tag_type == 34)
		{
			m_button_cxform.read_rgba(in);

			if(m_has_filter_list)
			{
				read_filter_list(in);
			}

			if( m_has_blend_mode )
			{
				m_blend_mode = in->read_u8();
			}
		}

		return true;
	}


	//
	// button_action
	//


	button_action::~button_action()
	{
		for (int i = 0, n = m_actions.size(); i < n; i++)
		{
			delete m_actions[i];
		}
		m_actions.resize(0);
	}

	void	button_action::read(stream* in, int tag_type)
	{
		// Read condition flags.
		if (tag_type == 7)
		{
			m_conditions = OVER_DOWN_TO_OVER_UP;
		}
		else
		{
			assert(tag_type == 34);
			m_conditions = in->read_u16();
		}

		// Read actions.
		IF_VERBOSE_ACTION(log_msg("-------------- actions in button\n")); // @@ need more info about which actions
		action_buffer*	a = new action_buffer;
		a->read(in);
		m_actions.push_back(a);
	}


	//
	// button_character_definition
	//

	button_character_definition::button_character_definition(player* player) :
		character_def(player),
		m_sound(NULL)
	// Constructor.
	{
	}

	button_character_definition::~button_character_definition()
	{
		delete m_sound;
	}


	void button_character_definition::sound_info::read(stream* in)
	{
		m_in_point = m_out_point = m_loop_count = 0;
		in->read_uint(2);	// skip reserved bits.
		m_stop_playback = in->read_uint(1) ? true : false;
		m_no_multiple = in->read_uint(1) ? true : false;
		m_has_envelope = in->read_uint(1) ? true : false;
		m_has_loops = in->read_uint(1) ? true : false;
		m_has_out_point = in->read_uint(1) ? true : false;
		m_has_in_point = in->read_uint(1) ? true : false;
		if (m_has_in_point) m_in_point = in->read_u32();
		if (m_has_out_point) m_out_point = in->read_u32();
		if (m_has_loops) m_loop_count = in->read_u16();
		if (m_has_envelope)
		{
			int nPoints = in->read_u8();
			m_envelopes.resize(nPoints);
			for (int i=0; i < nPoints; i++)
			{
				m_envelopes[i].m_mark44 = in->read_u32();
				m_envelopes[i].m_level0 = in->read_u16();
				m_envelopes[i].m_level1 = in->read_u16();
			}
		}
		else
		{
			m_envelopes.resize(0);
		}
		IF_VERBOSE_PARSE(
			log_msg("	has_envelope = %d\n", m_has_envelope);
			log_msg("	has_loops = %d\n", m_has_loops);
			log_msg("	has_out_point = %d\n", m_has_out_point);
			log_msg("	has_in_point = %d\n", m_has_in_point);
			log_msg("	in_point = %d\n", m_in_point);
			log_msg("	out_point = %d\n", m_out_point);

			log_msg("	loop_count = %d\n", m_loop_count);
			log_msg("	envelope size = %d\n", m_envelopes.size());
		);
	}



	void	button_character_definition::read(stream* in, int tag_type, movie_definition_sub* m)
	// Initialize from the given stream.
	{
		assert(tag_type == 7 || tag_type == 17 || tag_type == 34);

		if (tag_type == 7)
		{
			// Old button tag.
				
			// Read button character records.
			for (;;)
			{
				button_record	r;
				if (r.read(in, tag_type, m) == false)
				{
					// Null record; marks the end of button records.
					break;
				}
				m_button_records.push_back(r);
			}

			// Read actions.
			m_button_actions.resize(m_button_actions.size() + 1);
			m_button_actions.back().read(in, tag_type);
		}
		else if (tag_type == 17)
		{
			assert(m_sound == NULL);	// redefinition button sound is error
			m_sound = new button_sound_def();
			IF_VERBOSE_PARSE(log_msg("button sound options:\n"));
			for (int i = 0; i < 4; i++)
			{
				button_sound_info& bs = m_sound->m_button_sounds[i];
				bs.m_sound_id = in->read_u16();
				if (bs.m_sound_id > 0)
				{
					bs.m_sam = m->get_sound_sample(bs.m_sound_id);
//					if (bs.m_sam == NULL)
//					{
//						printf("sound tag not found, sound_id=%d, button state #=%i", bs.sound_id, i);
//					}
					IF_VERBOSE_PARSE(log_msg("\n	sound_id = %d\n", bs.m_sound_id));
					bs.m_sound_style.read(in);
				}
			}
		}
		else if (tag_type == 34)
		{
			// Read the menu flag.
			m_menu = in->read_u8() != 0;

			int	button_2_action_offset = in->read_u16();
			int	next_action_pos = in->get_position() + button_2_action_offset - 2;

			// Read button records.
			for (;;)
			{
				button_record	r;
				if (r.read(in, tag_type, m) == false)
				{
					// Null record; marks the end of button records.
					break;
				}
				m_button_records.push_back(r);
			}

			if (button_2_action_offset > 0)
			{
				in->set_position(next_action_pos);

				// Read Button2ActionConditions
				for (;;)
				{
					int	next_action_offset = in->read_u16();
					next_action_pos = in->get_position() + next_action_offset - 2;

					m_button_actions.resize(m_button_actions.size() + 1);
					m_button_actions.back().read(in, tag_type);

					if (next_action_offset == 0
						|| in->get_position() >= in->get_tag_end_position())
					{
						// done.
						break;
					}

					// seek to next action.
					in->set_position(next_action_pos);
				}
			}
		}
	}


	character*	button_character_definition::create_character_instance(character* parent, int id)
	// Create a mutable instance of our definition.
	{
		character*	ch = new button_character_instance(get_player(), this, parent, id);
		// instanciate_registered_class( ch );	//TODO: test it
		return ch;
	}
};


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
