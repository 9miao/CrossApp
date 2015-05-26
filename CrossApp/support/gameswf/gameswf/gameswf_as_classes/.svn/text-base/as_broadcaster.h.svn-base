// as_broadcaster.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script AsBroadcaster implementation code for the gameswf SWF player library.


#ifndef GAMESWF_AS_BROADCASTER_H
#define GAMESWF_AS_BROADCASTER_H

#include "gameswf/gameswf_action.h"	// for as_object
#include "gameswf/gameswf_listener.h"
#include "base/tu_queue.h"

namespace gameswf
{

	// Create built-in broadcaster object.
	as_object* broadcaster_init(player* player);

	struct as_listener : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_LISTENER };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		as_listener(player* player);
		virtual bool	get_member(const tu_stringi& name, as_value* val);
		void add(as_object* listener);
		void remove(as_object* listener);
		int size() const;
		virtual void enumerate(as_environment* env);
		void	broadcast(const fn_call& fn);
	
		private :

		listener m_listeners;
		bool m_reentrance;
		tu_queue< array<as_value> > m_suspended_event;
	};

}	// end namespace gameswf


#endif // GAMESWF_AS_BROADCASTER_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
