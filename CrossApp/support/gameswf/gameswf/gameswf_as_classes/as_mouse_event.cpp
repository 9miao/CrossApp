// as_mouse_event.cpp	-- Vitaly Alexeev <tishka92@yahoo.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// flash9

#include "as_mouse_event.h"

namespace gameswf
{

	//TODO

	as_mouse_event* mouse_event_init(player* player)
	{
		// Create built-in key object.
		as_mouse_event*	obj = new as_mouse_event(player);

		// methods
		obj->builtin_member("CLICK", "click");


		return obj;
	}

	as_mouse_event::as_mouse_event(player* player) :
		as_event(player)
	{
	}

};
