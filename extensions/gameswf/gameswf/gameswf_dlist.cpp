// gameswf_dlist.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A list of active characters.



#include "gameswf_dlist.h"
#include "gameswf_log.h"
#include "gameswf_render.h"
#include "gameswf.h"
#include "gameswf_root.h"
#include <typeinfo>



namespace gameswf
{
	/*static*/ int	display_object_info::compare(const void* _a, const void* _b)
	{
		display_object_info*	a = (display_object_info*) _a;
		display_object_info*	b = (display_object_info*) _b;

		if (a->m_character->get_depth() < b->m_character->get_depth())
		{
			return -1;
		}
		else if (a->m_character->get_depth() == b->m_character->get_depth())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	
	int	display_list::find_display_index(int depth)
	// Find the index in the display list of the first object
	// matching the given depth.  Failing that, return the index
	// of the first object with a larger depth.
	{
		int	size = m_display_object_array.size();
		if (size == 0)
		{
			return 0;
		}
		
		// Binary search.
		int	jump = size >> 1;
		int	index = jump;
		for (;;)
		{
			jump >>= 1;
			if (jump < 1) jump = 1;
			
			if (depth > m_display_object_array[index].m_character->get_depth())
			{
				if (index == size - 1)
				{
					index = size;
					break;
				}
				index += jump;
			}
			else if (depth < m_display_object_array[index].m_character->get_depth())
			{
				if (index == 0
				    || depth > m_display_object_array[index - 1].m_character->get_depth())
				{
					break;
				}
				index -= jump;
			}
			else
			{
				// match -- scan backward to make sure this is the first entry with this depth.
				//
				// Linear.  But multiple objects with the same depth are only allowed for
				// very old SWF's, so we don't really care.
				for (;;)
				{
					if (index == 0
					    || m_display_object_array[index - 1].m_character->get_depth() < depth)
					{
						break;
					}
					index--;
				}
				assert(m_display_object_array[index].m_character->get_depth() == depth);
				assert(index == 0 || m_display_object_array[index - 1].m_character->get_depth() < depth);
				break;
			}
		}
		
		assert(index >= 0 && index <= size);
		
		return index;
	}
	

	int	display_list::get_display_index(int depth)
		// Like the above, but looks for an exact match, and returns -1 if failed.
	{
		int	index = find_display_index(depth);
		if (index >= m_display_object_array.size()
			|| get_display_object(index).m_character->get_depth() != depth)
		{
			// No object at that depth.
			return -1;
		}
		return index;
	}


	character*	display_list::get_character_at_depth(int depth)
	{
		int	index = get_display_index(depth);
		if (index != -1)
		{
			character*	ch = m_display_object_array[index].m_character.get_ptr();
			assert(ch->get_depth() == depth);

			return ch;
		}

		return NULL;
	}


	character*	display_list::get_character_by_name(const tu_string& name)
	{
		// See if we have a match on the display list.
		for (int i = 0, n = size(); i < n; i++)
		{
			character*	ch = get_character(i);
			if (ch->get_name() == name.c_str())
			{
				// Found it.
				return ch;
			}
		}
		return NULL;
	}

	int	display_list::get_character_by_ptr(const character* ch_search)
	{
		// See if we have a match on the display list.
		for (int i = 0, n = size(); i < n; i++)
		{
			character*	ch = get_character(i);
			if (ch == ch_search)
			{
				// Found it.
				return i;
			}
		}
		return -1;
	}

	character*	display_list::get_character_by_name_i(const tu_stringi& name)
	// Return first character with matching (case insensitive) name, if any.
	{
		// Search through dlist for a match.
		for (int i = 0, n = size(); i < n; i++)
		{
			character*	ch = get_character(i);
			if (name == ch->get_name().c_str())
			{
				// Found it.
				return ch;
			}
		}

		return NULL;
	}

	void display_list::remove(int index)
	// Removing the character at get_display_object(index).
	{
		display_object_info&	di = m_display_object_array[index];

		// indirect call onUnload & killFocus of children
		di.m_character->clear_display_objects();

		di.m_character->on_event(event_id::KILLFOCUS);
		di.m_character->on_event(event_id::UNLOAD);
		di.m_character->set_depth(0);	// is't alive
		
		// remove this character from listener
		remove_keypress_listener(di.m_character.get_ptr());

		di.set_character(NULL);
		m_display_object_array.remove(index);
	}

	void	display_list::add_display_object( character* ch,  int depth, bool replace_if_depth_is_occupied,
			 const cxform& color_xform,  const matrix& mat,  float ratio, Uint16 clip_depth, Uint8 blend_mode)
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::add(%d, '%s')\n", depth, ch->get_name()));//xxxxx

		assert(ch);

		int	size = m_display_object_array.size();
		int index = find_display_index(depth);

		if (replace_if_depth_is_occupied)
		{
			// Eliminate an existing object if it's in the way.
			if (index >= 0 && index < size)
			{
				display_object_info & dobj = m_display_object_array[index];

				if (dobj.m_character->get_depth() == depth)
				{
					remove(index);
				}
			}
		}
		else
		{
			// Caller wants us to allow multiple objects
			// with the same depth.  find_display_index()
			// returns the first matching depth, if there
			// are any, so the new character will get
			// inserted before all the others with the
			// same depth.  This matches the semantics
			// described by Alexi's SWF ref.  (This is all
			// for legacy SWF compatibility anyway.)
		}

		ch->set_depth(depth);

		display_object_info	di;
		di.set_character(ch);
		di.m_character->set_depth(depth);
		di.m_character->set_cxform(color_xform);
		di.m_character->set_matrix(mat);
		di.m_character->set_ratio(ratio);
		di.m_character->set_clip_depth(clip_depth);
		di.m_character->set_blend_mode(blend_mode);

		// Insert into the display list...
		assert(index == find_display_index(depth));
		
		m_display_object_array.insert(index, di);

		ch->execute_frame_tags(0);
		add_keypress_listener(ch);
	}
	
	void	display_list::move_display_object( int depth, bool use_cxform, const cxform& color_xform,
					 bool use_matrix, const matrix& mat, float ratio, Uint16 clip_depth, Uint8 blend_mode)
	// Updates the transform properties of the object at
	// the specified depth.
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::move(%d)\n", depth));//xxxxx

		int	size = m_display_object_array.size();
		if (size <= 0)
		{
			// error.
			log_error("error: move_display_object() -- no objects on display list\n");
			//			assert(0);
			return;
		}
		
		int	index = find_display_index(depth);
		if (index < 0 || index >= size)
		{
			// error.
			// log_error("error: move_display_object() -- can't find object at depth %d\n", depth);
			//			assert(0);
			return;
		}
		
		display_object_info&	di = m_display_object_array[index];
		character*	ch = di.m_character.get_ptr();
		if (ch->get_depth() != depth)
		{
			// error
			log_error("error: move_display_object() -- no object at depth %d\n", depth);
			return;
		}

		if (ch->get_accept_anim_moves() == false)
		{
			// This character is rejecting anim moves.  This happens after it
			// has been manipulated by ActionScript.
			return;
		}

		if (use_cxform)
		{
			ch->set_cxform(color_xform);
		}
		if (use_matrix)
		{
			ch->set_matrix(mat);
		}
		ch->set_ratio(ratio);
		// move_display_object apparently does not change clip depth!  Thanks to Alexeev Vitaly.
		// ch->set_clip_depth(clip_depth);
		ch->set_blend_mode(blend_mode);
	}
	
	
	void	display_list::replace_display_object( character* ch, int depth, bool use_cxform, const cxform& color_xform,
			 bool use_matrix, const matrix& mat, float ratio, Uint16 clip_depth, Uint8 blend_mode)
	// Puts a new character at the specified depth, replacing any
	// existing character.  If use_cxform or use_matrix are false,
	// then keep those respective properties from the existing
	// character.
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::replace(%d)\n", depth));//xxxxx

		int	size = m_display_object_array.size();
		int	index = find_display_index(depth);
		if (index < 0 || index >= size)
		{
			add_display_object(ch, depth, true, color_xform, mat, ratio, clip_depth, blend_mode);
			return;
		}
		
		display_object_info&	di = m_display_object_array[index];
		gc_ptr<character>	old_ch = di.m_character;

		// Put the new character in its place.

		assert(ch);

		cxform cx = color_xform;
		matrix m = mat;

		// Set the display properties.
		if (use_cxform == false)
		{
			// Use the cxform from the old character.
			cx = old_ch->get_cxform();
		}

		if (use_matrix == false)
		{
			// Use the matrix from the old character.
			m = old_ch->get_matrix();
		}

		add_display_object(ch, depth, true, cx, m, ratio, clip_depth, blend_mode);
	}
	

	void	display_list::remove_display_object(character* ch)
	// Removes the object with the specified pointer.
	{
		// find ch
		int i = get_character_by_ptr(ch);
		if (i >= 0) 
		{ 
			remove(i);
		} 
	}

	void	display_list::remove_display_object(int depth, int id)
	// Removes the object at the specified depth.
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::remove(%d)\n", depth));//xxxxx

		int	size = m_display_object_array.size();
		if (size <= 0)
		{
			// it's not error
			//log_error("remove_display_object: no characters in display list\n");
			return;
		}
		
		int	index = find_display_index(depth);
		if (index < 0
		    || index >= size
		    || get_character(index)->get_depth() != depth)
		{
			// it's not error
			// log_error("remove_display_object: no character at depth %d\n", depth);
			return;
		}

		assert(get_character(index)->get_depth() == depth);
		
		if (id != -1)
		{
			// Caller specifies a specific id; scan forward til we find it.
			for (;;)
			{
				if (get_character(index)->get_id() == id)
				{
					break;
				}
				if (index + 1 >= size || get_character(index + 1)->get_depth() != depth)
				{
					// Didn't find a match!
//					log_error("remove_display_object: no character at depth %d with id %d\n", depth, id);
					return;
				}
				index++;
			}
			assert(index < size);
			assert(get_character(index)->get_depth() == depth);
			assert(get_character(index)->get_id() == id);
		}

		remove(index);
	}

	void display_list::add_keypress_listener(character* ch)
	{
		// has character keypress event ?
		if (ch->has_keypress_event())
		{
			if (ch->get_root())
			{
				ch->get_root()->m_keypress_listener.add(ch);
			}
		}
	}
	
	void display_list::remove_keypress_listener(character* ch)
	{
		if (ch)
		{
			if (ch->get_root())
			{
				ch->get_root()->m_keypress_listener.remove(ch);
			}
		}
	}
	
	void	display_list::clear()
	// clear the display list.
	{
		while (m_display_object_array.size() > 0)
		{
			remove(0);
		}
	}
	
	void	display_list::advance(float delta_time)
	// advance referenced characters.
	{

		// That there was no crash because of actions script
		// we iterate through the copy
		swf_array<display_object_info> tmp_list = m_display_object_array;
		int n = tmp_list.size();
		for (int i = 0; i < n; i++)
		{
			character*	ch = tmp_list[i].m_character.get_ptr();
			if (ch)
			{
				ch->advance(delta_time);
			}
		}
	}
	
	
	void	display_list::display()
	// Display the referenced characters. Lower depths
	// are obscured by higher depths.
	{
		bool masked = false;
		int highest_masked_layer = 0;
		
		//log_msg("number of objects to be drawn %i\n", m_display_object_array.size());
		
		for (int i = 0; i < m_display_object_array.size(); i++)
		{
			display_object_info&	dobj = m_display_object_array[i];

			character*	ch = dobj.m_character.get_ptr();
			assert(ch);

			if (ch->get_visible() == false)
			{
				// Don't display.
				continue;
			}

			// check whether a previous mask should be disabled
			if (masked)
			{
				if (ch->get_depth() > highest_masked_layer)
				{
					masked = false;
	
					// turn off mask
					render::disable_mask();
				}
			}

			// check whether this object should become mask
			if (ch->get_clip_depth() > 0)
			{
				render::begin_submit_mask();

				// keep the bound of mask in static 
				// TODO: fix for nested mask
//				rect bound;
//				ch->get_bound(&bound);
//				render::set_mask_bound(bound);
			}

			// If 'ch' sit under a mask and 'ch' does not have intersection with a mask
			// then we should skip ch->display(). Thanks to Julien Hamaide
			// TODO: fix for nested mask
/*			if (masked)
			{
				rect bound;
				ch->get_bound(&bound);
				
				if (bound.bound_test(render::get_mask_bound()))
				{
					ch->display();
				}
			}
			else
			{
				ch->display();
			}*/

			ch->display();

			// if this object should have become a mask,
			// inform the renderer that it now has all
			// information about it
			if (ch->get_clip_depth() > 0)
			{
				//log_msg("end submit mask\n");
				render::end_submit_mask();
				highest_masked_layer = ch->get_clip_depth();
				masked = true;
			}
		}
		
		if (masked)
		{
			// If a mask masks the scene all the way up to the highest
			// layer, it will not be disabled at the end of drawing
			// the display list, so disable it manually.
			render::disable_mask();
		}
	}

	void display_list::clear_unaffected(swf_array<int>& affected_depths) 
	{ 
		for (int i = 0; i < m_display_object_array.size(); )
		{
			display_object_info&	di = m_display_object_array[i];

			int di_depth = di.m_character->get_depth(); 
			bool is_affected = false; 

			for (int j = 0; j < affected_depths.size(); j++) 
			{ 
				if (affected_depths[j] != di_depth) 
				{ 
					continue; 
				} 
				is_affected = true; 
				break; 
			} 

			if (is_affected == false) 
			{ 
				remove(i);
				continue; 
			} 
			i++; 
		} 
	} 

	void display_list::swap_characters(character* ch1, character* ch2) 
	{ 
		// find ch1 & ch2
		int i1 = get_character_by_ptr(ch1);
		int i2 = get_character_by_ptr(ch2);
		if (i1 >=0 && i2 >= 0) 
		{ 
			display_object_info tmp = m_display_object_array[i2];
			m_display_object_array[i2] = m_display_object_array[i1];
			m_display_object_array[i1] = tmp;
		} 
	} 


	void display_list::change_character_depth(character* ch, int depth)
	{
		int ch_index = get_character_by_ptr( ch );
		assert( get_display_index( depth ) == -1 );

		ch->set_depth(depth);

		display_object_info	di;
		di.set_character(ch);

		m_display_object_array.remove( ch_index );

		// Insert into the display list...
		int new_index = find_display_index(depth);

		m_display_object_array.insert(new_index, di);

	}

	// return next FREE depth number
	int display_list::get_highest_depth() 
	{ 
		int depth = ADJUST_DEPTH_VALUE - 1;
		for (int i = 0, n = size(); i < n; i++)
		{
			character*	ch = get_character(i);
			assert(ch);
			if (ch->get_depth() > depth)
			{
				depth = ch->get_depth();
			}
		}
		return depth + 1;
	}

	void	display_list::clear_refs(gameswf::hash<as_object*, bool>* visited_objects, as_object* this_ptr)
	{
		for (int i = 0, n = size(); i < n; i++)
		{
			character*	ch = get_character(i);
			if (ch)
			{
				ch->clear_refs(visited_objects, this_ptr);
			}
		}
	}

	void display_list::dump(tu_string& tabs)
	{
		tabs += "  ";
		printf("%s*** displaylist ***\n", tabs.c_str());
		for (int i = 0, n = size(); i < n; i++)
		{
			character*	ch = get_character(i);
			if (ch->get_name().size() > 0)
			{
				printf("%s%s\n", tabs.c_str(), ch->get_name().c_str());
			}
			else
			{
				printf("%s<noname>\n", tabs.c_str());
			}
		}
		tabs.resize(tabs.size() - 2);
	}
}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
