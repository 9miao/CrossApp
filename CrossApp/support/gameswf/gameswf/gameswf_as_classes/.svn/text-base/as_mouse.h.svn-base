// as_mouse	-- Vitaly Alexeev <tishka92@yahoo.com>	2009

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// 'Mouse' action script class implementation

#ifndef GAMESWF_AS_MOUSE_H
#define GAMESWF_AS_MOUSE_H

#include "gameswf/gameswf_action.h"	// for as_object
#include "gameswf/gameswf_listener.h"

namespace gameswf
{
	struct as_mouse : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_MOUSE };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		as_mouse(player* player);
	};

}

#endif //GAMESWF_AS_MOUSE_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
