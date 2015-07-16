// as_key.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script Key implementation code for the gameswf SWF player library.


#ifndef GAMESWF_AS_KEY_H
#define GAMESWF_AS_KEY_H

#include "../gameswf_action.h"	// for as_object
#include "../gameswf_root.h"	// for listener

namespace gameswf
{
	void	key_add_listener(const fn_call& fn);
	// Add a listener (first arg is object reference) to our list.
	// Listeners will have "onKeyDown" and "onKeyUp" methods
	// called on them when a key changes state.

	void	key_get_ascii(const fn_call& fn);
	// Return the ascii value of the last key pressed.

	void	key_get_code(const fn_call& fn);
	// Returns the keycode of the last key pressed.

	void	key_is_down(const fn_call& fn);
	// Return true if the specified (first arg keycode) key is pressed.

	void	key_is_toggled(const fn_call& fn);
	// Given the keycode of NUM_LOCK or CAPSLOCK, returns true if
	// the associated state is on.

	void	key_remove_listener(const fn_call& fn);
	// Remove a previously-added listener.

	struct as_key : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_KEY };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		Uint8	m_keymap[key::KEYCOUNT / 8 + 1];	// bit-array
		int	m_last_key_pressed;
		listener m_listeners;

		as_key(player* player);

		bool	is_key_down(int code);
		void	set_key_down(int code);
		void	set_key_up(int code);
		int	get_last_key_pressed() const;
	};

	// creates 'Key' object
	as_key* key_init(player* player);

}	// namespace gameswf

#endif	// GAMESWF_AS_KEY_H
