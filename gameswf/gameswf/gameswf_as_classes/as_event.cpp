// as_mouse_event.cpp	-- Vitaly Alexeev <tishka92@yahoo.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// flash9

#include "as_event.h"
#include "as_mouse_event.h"
#include "../gameswf_listener.h"

namespace gameswf
{

	// TODO

	as_event* event_init(player* player)
	{
		as_event*	obj = new as_event(player);

		// packages
		obj->builtin_member("MouseEvent", mouse_event_init(player));

		return obj;
	}

	as_event::as_event(player* player) :
		as_object(player)
	{
		listener m_listeners;

	}


};
