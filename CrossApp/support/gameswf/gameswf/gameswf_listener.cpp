// gameswf_listener.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf_listener.h"

namespace gameswf
{

	// for Key, keyPress, ...
	void listener::notify(const event_id& ev)
	{
		// event handler may affects m_listeners using addListener & removeListener
		// iterate through a copy of it
		clone_listener listeners(m_listeners);
		for (int i = 0, n = listeners.size(); i < n; i++)
		{
			listeners[i]->on_event(ev);
		}
	}

	// for asBroadcaster, ...
	void listener::notify(const tu_string& event_name, const fn_call& fn)
	{
		// may be called from multithread plugin ==>
		// we should check current root
		if (fn.get_player()->get_root() == NULL)
		{
			return;
		}

		// event handler may affects m_listeners using addListener & removeListener
		// iterate through a copy of it
		clone_listener listeners(m_listeners);

		// iterate through a copy of environment
		int nargs = fn.nargs;
		as_environment env(fn.get_player());
		for (int i = 0; i < nargs; i++)
		{
			env.push(fn.env->top(nargs - i - 1));
		}

		for (int i = 0, n = listeners.size(); i < n; i++)
		{
			as_value function;
			if (listeners[i]->get_member(event_name, &function))
			{
				call_method(function, &env, listeners[i].get_ptr(),
					nargs, env.get_top_index());
			}
		}
	}

	// for video, timer, ...
	void	listener::advance(float delta_time)
	{
		// event handler may affects m_listeners using addListener & removeListener
		// iterate through a copy of it
		clone_listener listeners(m_listeners);
		for (int i = 0, n = listeners.size(); i < n; i++)
		{
			listeners[i]->advance(delta_time);
		}
	}

	void listener::add(as_object* listener) 
	{
		// sanity check
		assert(m_listeners.size() < 1000);
		//printf("m_listeners size=%d\n", m_listeners.size());

		if (listener)
		{
			int free_item = -1;
			for (int i = 0, n = m_listeners.size(); i < n; i++)
			{
				if (m_listeners[i] == listener)
				{
					return;
				}
				if (m_listeners[i] == NULL)
				{
					free_item = i;
				}
			}

			if (free_item >= 0)
			{
				m_listeners[free_item] = listener;
			}
			else
			{
				m_listeners.push_back(listener);
			}
		}
	} 

	bool listener::remove(as_object* listener) 
	{
		bool is_removed = false;

		// null out a item
		for (int i = 0, n = m_listeners.size(); i < n; i++)
		{
			if (m_listeners[i] == listener)
			{
				m_listeners[i] = NULL;
				is_removed = true;
			}
		}
		return is_removed;
	} 

	as_object*	listener::operator[](const tu_stringi& name) const
	{
		return operator[](atoi(name.c_str()));
	}
	
	as_object*	listener::operator[](int index) const
	{
		if (index >= 0 && index < m_listeners.size())
		{
			int nonzero = 0;
			for (int i = 0, n = m_listeners.size(); i < n; i++)
			{
				if (m_listeners[i] != NULL)
				{
					if (nonzero == index)
					{
						return m_listeners[i].get_ptr();
					}
					nonzero++;
				}
			}
		}
		return NULL;
	}

	int	listener::size() const
	{
		int nonzero = 0;
		for (int i = 0, n = m_listeners.size(); i < n; i++)
		{
			if (m_listeners[i] != NULL)
			{
				nonzero++;
			}
		}
		return nonzero;
	}

	void listener::enumerate(as_environment* env) const
	{
		int nonzero = 0;
		for (int i = 0, n = m_listeners.size(); i < n; i++)
		{
			if (m_listeners[i] != NULL)
			{
				env->push(nonzero);
				nonzero++;
			}
		}
	}

}
