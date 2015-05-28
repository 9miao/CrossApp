// as_broadcaster.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Provides event notification and listener management capabilities that
// you can add to user-defined objects. This class is intended for advanced 
// users who want to create custom event handling mechanisms. 
// You can use this class to make any object an event broadcaster and 
// to create one or more listener objects that receive notification anytime 
// the broadcasting object calls the broadcastMessage() method. 

#include "gameswf/gameswf_as_classes/as_broadcaster.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include "gameswf/gameswf_function.h"

namespace gameswf
{
	// (listenerObj:Object) : Boolean
	void	as_broadcast_addlistener(const fn_call& fn)
	// Registers an object to receive event notification messages.
	// This method is called on the broadcasting object and
	// the listener object is sent as an argument.
	{
		assert(fn.this_ptr);
		as_value val;
		if (fn.this_ptr->get_member("_listeners", &val))
		{
			as_listener* asl = cast_to<as_listener>(val.to_object());
			if (asl)
			{
				as_object* listener = fn.arg(0).to_object();
				if (listener)
				{
					asl->add(listener);
				}
			}
		}
	}

	// (listenerObj:Object) : Boolean
	void	as_broadcast_removelistener(const fn_call& fn)
	// Removes an object from the list of objects that receive event notification messages. 
	{
		assert(fn.this_ptr);
		as_value val;
		if (fn.this_ptr->get_member("_listeners", &val))
		{
			as_listener* asl = cast_to<as_listener>(val.to_object());
			if (asl)
			{
				asl->remove(fn.arg(0).to_object());
			}
		}
	}

	// (eventName:String, ...) : Void
	void	as_broadcast_sendmessage(const fn_call& fn)
	// Sends an event message to each object in the list of listeners.
	// When the message is received by the listening object,
	// gameswf attempts to invoke a function of the same name on the listening object. 
	{
		assert(fn.this_ptr);
		as_value val;
		if (fn.this_ptr->get_member("_listeners", &val))
		{
			as_listener* asl = cast_to<as_listener>(val.to_object());
			if (asl)
			{
				asl->broadcast(fn);
			}
		}
	}

	// public static initialize(obj:Object) : Void
	// Adds event notification and listener management functionality to a given object.
	// This is a static method; it must be called by using the AsBroadcaster class
	void	as_broadcaster_initialize(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			as_object* obj = fn.arg(0).to_object();
			if (obj)
			{
				obj->set_member("_listeners", new as_listener(fn.get_player()));
				obj->set_member("addListener", as_broadcast_addlistener);
				obj->set_member("removeListener", as_broadcast_removelistener);
				obj->set_member("broadcastMessage", as_broadcast_sendmessage);
			}
		}
	}
		
	as_object* broadcaster_init(player* player)
	{
		as_object* bc = new as_object(player);
		bc->builtin_member("initialize", as_broadcaster_initialize);
		return bc;
	}

	as_listener::as_listener(player* player) :
		as_object(player),
		m_reentrance(false)
	{
	}

	bool	as_listener::get_member(const tu_stringi& name, as_value* val)
	{
		if (name == "length")
		{
			val->set_int(m_listeners.size());
		}
		else
		{
			as_object* listener = m_listeners[name];
			val->set_as_object(listener);
		}
		return true;
	}

	void as_listener::add(as_object* listener)
	{
		m_listeners.add(listener);
	}

	void as_listener::remove(as_object* listener)
	{
		m_listeners.remove(listener);
	}

	int as_listener::size() const
	{
		return m_listeners.size();
	}

	void as_listener::enumerate(as_environment* env)
	// retrieves members & pushes them into env
	{
		m_listeners.enumerate(env);
	}

	void	as_listener::broadcast(const fn_call& fn)
	{
		assert(fn.env);

		if (m_reentrance)
		{
			// keep call args
			// we must process one event completely then another
			array<as_value> arg;
			for (int i = 0; i < fn.nargs; i++)
			{
				arg.push_back(fn.arg(i));
			}
			m_suspended_event.push(arg);
			return;
		}
		m_reentrance = true;

		// event handler may affects 'fn.arg' using broadcastMessage
		// so we iterate through the copy of args
		tu_string event_name = fn.arg(0).to_tu_string();
		for (int j = fn.nargs - 1; j > 0; j--)
		{
			fn.env->push(fn.arg(j));
		}
			
		m_listeners.notify(event_name, 
			fn_call(NULL, 0, fn.env, fn.nargs - 1, fn.env->get_top_index()));

		fn.env->drop(fn.nargs - 1);

		// check reentrances
		while (m_suspended_event.size() > 0)
		{
			// event handler may affects m_suspended_event using broadcastMessage
			// so we iterate through the copy of args
			array<as_value>& arg = m_suspended_event.front();
			tu_string event_name = arg[0].to_tu_string();
			for (int j = arg.size() - 1; j > 0; j--)
			{
				fn.env->push(arg[j]);
			}
				
			m_listeners.notify(event_name, 
				fn_call(NULL, 0, fn.env, arg.size() - 1, fn.env->get_top_index()));

			fn.env->drop(fn.nargs - 1);
			m_suspended_event.pop();
		}

		m_reentrance = false;
	}

};
