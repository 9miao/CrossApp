// gameswf_dlist.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A list of active characters.


#ifndef GAMESWF_DLIST_H
#define GAMESWF_DLIST_H


#include "../base/container.h"
#include "gameswf_types.h"
#include "gameswf_impl.h"


namespace gameswf
{

	#define ADJUST_DEPTH_VALUE 16384

	// A struct to serve as an entry in the display list.
	struct display_object_info
	{
		gc_ptr<character>	m_character;	// state is held in here

		display_object_info()
		{
		}

		display_object_info(const display_object_info& di)
		{
			*this = di;
		}

		~display_object_info()
		{
		}

		void	operator=(const display_object_info& di)
		{
			m_character = di.m_character;
		}

		void	set_character(character* ch)
		{
			m_character = ch;
		}

		static int compare(const void* _a, const void* _b); // For qsort().
	};


	// A list of active characters.
	struct display_list
	{

		// TODO use better names!
		int	find_display_index(int depth);
		int	get_display_index(int depth);
		
		void	add_display_object( character* ch, int depth, bool replace_if_depth_is_occupied, 
				const cxform& color_xform, const matrix& mat, float ratio, Uint16 clip_depth, Uint8 blend_mode);
		void	move_display_object( int depth, bool use_cxform, const cxform& color_xform, bool use_matrix,
				const matrix& mat, float ratio, Uint16 clip_depth, Uint8 blend_mode);
		void	replace_display_object( character* ch, int depth, bool use_cxform, const cxform& color_xform,
				bool use_matrix, const matrix& mat, float ratio, Uint16 clip_depth, Uint8 blend_mode);


		void	remove_display_object(character* ch);
		void	remove_display_object(int depth, int id);

		void add_keypress_listener(character* ch);
		void remove_keypress_listener(character* ch);

		// clear the display list.
		void	clear();

		// advance referenced characters.
		void	advance(float delta_time);

		// display the referenced characters.
		void	display();
		void	display(const display_info& di);

		int	size() { return m_display_object_array.size(); }
		character*	get_character(int index) { return m_display_object_array[index].m_character.get_ptr(); }

		// May return NULL.
		character*	get_character_at_depth(int depth);

		// May return NULL.
		// If there are multiples, returns the *first* match only!
		character*	get_character_by_name(const tu_string& name);

		// returns index of ch
		int	get_character_by_ptr(const character* ch);

		// May return NULL.
		// If there are multiples, returns the *first* match only!
		character*	get_character_by_name_i(const tu_stringi& name);

		inline const display_object_info&	get_display_object(int idx) const
		// get the display object at the given position.
		{
			return m_display_object_array[idx];
		}

		void clear_unaffected(swf_array<int>& affected_depths);
		void swap_characters(character* ch, character* ch2);
		void change_character_depth(character* ch, int depth);
		int get_highest_depth();

		void clear_refs(gameswf::hash<as_object*, bool>* visited_objects, as_object* this_ptr);
		void dump(tu_string& tabs);

	private:

		void remove(int index);
		swf_array<display_object_info> m_display_object_array;
	};


}


#endif // GAMESWF_DLIST_H



// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
