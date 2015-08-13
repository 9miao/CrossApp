// as_color.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "as_selection.h"
#include "../gameswf_character.h"

namespace gameswf
{

	void	as_selection_setfocus(const fn_call& fn)
	{
		as_selection* obj = cast_to<as_selection>(fn.this_ptr);
		if (obj)
		{
			if (fn.nargs > 0)
			{
				character* target = cast_to<character>(fn.env->find_target(fn.arg(0)));
				if (target)
				{
					target->on_event(event_id::SETFOCUS);
					fn.result->set_bool(true);
				}
			}
		}
		fn.result->set_bool(false);
	}

	as_object* selection_init(player* player)
	{
		as_object* sel = new as_selection(player);

		// methods
		sel->builtin_member("setFocus", as_selection_setfocus);

// TODO
//		sel->set_member("getFocus", as_selection_setfocus);
//		sel->set_member("addListener", as_selection_setfocus);
//		sel->set_member("removeListener", as_selection_setfocus);
		// ...
		return sel;
	}

	as_selection::as_selection(player* player) :
		as_object(player)
	{
	}
};
