// gameswf_listener.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.


#ifndef GAMESWF_LISTENER_H
#define GAMESWF_LISTENER_H

#include "gameswf.h"
#include "gameswf_function.h"
#include "../base/container.h"
#include "../base/utility.h"
#include "../base/weak_ptr.h"

namespace gameswf
{

	struct listener : public ref_counted
	{

		// misc
		struct clone_listener : public swf_array<gc_ptr<as_object> >
		{
			clone_listener(const swf_array< gameswf::weak_ptr<as_object> >& listeners)
			{
				for (int i = 0, n = listeners.size(); i < n; i++)
				{
					if (listeners[i] != NULL)
					{
						push_back(listeners[i].get_ptr());
					}
				}
			}
		};

		exported_module void add(as_object* listener);
		exported_module bool remove(as_object* listener);

		exported_module void notify(const event_id& ev);
		exported_module void notify(const tu_string& event_name, const fn_call& fn);
		exported_module void advance(float delta_time);

		exported_module void clear() { m_listeners.clear(); }
		exported_module as_object*	operator[](const tu_stringi& name) const;
		exported_module as_object*	operator[](int index) const;
		exported_module int	size() const;
		exported_module void enumerate(as_environment* env) const;

	private:

		swf_array< gameswf::weak_ptr<as_object> > m_listeners;

	};

}

#endif
