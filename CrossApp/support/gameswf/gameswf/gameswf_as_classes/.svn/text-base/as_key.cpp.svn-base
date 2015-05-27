// as_key.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf/gameswf_as_classes/as_key.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_root.h"

namespace gameswf
{

	void	key_add_listener(const fn_call& fn)
	// Add a listener (first arg is object reference) to our list.
	// Listeners will have "onKeyDown" and "onKeyUp" methods
	// called on them when a key changes state.
	{
		as_key*	ko = cast_to<as_key>(fn.this_ptr);
		assert(ko);

		if (fn.nargs < 1)
		{
			log_error("key_add_listener needs one argument (the listener object)\n");
			return;
		}

		as_object*	listener = fn.arg(0).to_object();
		if (listener == NULL)
		{
			log_error("key_add_listener passed a NULL object; ignored\n");
			return;
		}

		ko->m_listeners.add(listener);
	}

	void	key_remove_listener(const fn_call& fn)
	// Remove a previously-added listener.
	{
		as_key*	ko = cast_to<as_key>(fn.this_ptr);
		assert(ko);

		if (fn.nargs < 1)
		{
			log_error("key_remove_listener needs one argument (the listener object)\n");
			return;
		}

		ko->m_listeners.remove(fn.arg(0).to_object());
	}

	void	key_get_ascii(const fn_call& fn)
	// Return the ascii value of the last key pressed.
	{
		as_key*	ko = cast_to<as_key>(fn.this_ptr);
		assert(ko);

		fn.result->set_undefined();

		int	code = ko->get_last_key_pressed();
		if (code > 0)
		{
			// @@ Crude for now; just jamming the key code in a string, as a character.
			// Need to apply shift/capslock/numlock, etc...
			char	buf[2];
			buf[0] = (char) code;
			buf[1] = 0;

			fn.result->set_string(buf);
		}
	}

	void	key_get_code(const fn_call& fn)
	// Returns the keycode of the last key pressed.
	{
		as_key*	ko = cast_to<as_key>(fn.this_ptr);
		assert(ko);

		fn.result->set_int(ko->get_last_key_pressed());
	}

	void	key_is_down(const fn_call& fn)
	// Return true if the specified (first arg keycode) key is pressed.
	{
		if (fn.nargs < 1)
		{
			log_error("key_is_down needs one argument (the key code)\n");
			return;
		}

		int	code = fn.arg(0).to_int();

		as_key*	ko = cast_to<as_key>(fn.this_ptr);
		assert(ko);

		fn.result->set_bool(ko->is_key_down(code));
	}

	void	key_is_toggled(const fn_call& fn)
	// Given the keycode of NUM_LOCK or CAPSLOCK, returns true if
	// the associated state is on.
	{
		// @@ TODO
		fn.result->set_bool(false);
	}

	as_key* key_init(player* player)
	{
		// Create built-in key object.
		as_key*	key_obj = new as_key(player);

		// constants
	#define KEY_CONST(k) key_obj->set_member(#k, key::k)
		KEY_CONST(BACKSPACE);
		KEY_CONST(CAPSLOCK);
		KEY_CONST(CONTROL);
		KEY_CONST(DELETEKEY);
		KEY_CONST(DOWN);
		KEY_CONST(END);
		KEY_CONST(ENTER);
		KEY_CONST(ESCAPE);
		KEY_CONST(HOME);
		KEY_CONST(INSERT);
		KEY_CONST(LEFT);
		KEY_CONST(PGDN);
		KEY_CONST(PGUP);
		KEY_CONST(RIGHT);
		KEY_CONST(SHIFT);
		KEY_CONST(SPACE);
		KEY_CONST(TAB);
		KEY_CONST(UP);

		// methods
		key_obj->builtin_member("addListener", key_add_listener);
		key_obj->builtin_member("getAscii", key_get_ascii);
		key_obj->builtin_member("getCode", key_get_code);
		key_obj->builtin_member("isDown", key_is_down);
		key_obj->builtin_member("isToggled", key_is_toggled);
		key_obj->builtin_member("removeListener", key_remove_listener);

		return key_obj;
	}

	as_key::as_key(player* player) :
		as_object(player),
		m_last_key_pressed(0)
	{
		memset(m_keymap, 0, sizeof(m_keymap));
	}

	bool	as_key::is_key_down(int code)
	{
		if (code < 0 || code >= key::KEYCOUNT) return false;

		int	byte_index = code >> 3;
		int	bit_index = code - (byte_index << 3);
		int	mask = 1 << bit_index;

		assert(byte_index >= 0 && byte_index < int(sizeof(m_keymap)/sizeof(m_keymap[0])));

		if (m_keymap[byte_index] & mask)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void	as_key::set_key_down(int code)
	{
		if (code < 0 || code >= key::KEYCOUNT) return;

		m_last_key_pressed = code;

		int	byte_index = code >> 3;
		int	bit_index = code - (byte_index << 3);
		int	mask = 1 << bit_index;

		assert(byte_index >= 0 && byte_index < int(sizeof(m_keymap)/sizeof(m_keymap[0])));
		m_keymap[byte_index] |= mask;
		m_listeners.notify(event_id(event_id::KEY_DOWN));
	}

	void	as_key::set_key_up(int code)
	{
		if (code < 0 || code >= key::KEYCOUNT) return;

		m_last_key_pressed = code;

		int	byte_index = code >> 3;
		int	bit_index = code - (byte_index << 3);
		int	mask = 1 << bit_index;

		assert(byte_index >= 0 && byte_index < int(sizeof(m_keymap)/sizeof(m_keymap[0])));
		m_keymap[byte_index] &= ~mask;

		m_listeners.notify(event_id(event_id::KEY_UP));
	}

	int	as_key::get_last_key_pressed() const
	{
		return m_last_key_pressed; 
	}

}
