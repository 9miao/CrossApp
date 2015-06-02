// as_mouse_event.cpp	-- Vitaly Alexeev <tishka92@yahoo.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// flash9

#ifndef GAMESWF_AS_EVENT_H
#define GAMESWF_AS_EVENT_H

#include "gameswf/gameswf_action.h"	// for as_object

namespace gameswf
{

	struct as_event;
	as_event* event_init(player* player);

	struct as_event : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_EVENT };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		as_event(player* player);

	};



}	// end namespace gameswf


#endif // GAMESWF_AS_EVENT_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
