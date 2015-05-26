// as_mouse	-- Vitaly Alexeev <tishka92@yahoo.com>	2009

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// 'Mouse' action script class implementation

#include "gameswf/gameswf_render.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_as_classes/as_mouse.h"

namespace gameswf
{

	void	as_mouse_hide(const fn_call& fn)
	{
		as_mouse*	mouse = cast_to<as_mouse>(fn.this_ptr);
		render_handler* rh = get_render_handler();
		if (mouse && rh)
		{
			rh->set_cursor(render_handler::INVISIBLE_CURSOR);
		}
	}

	void	as_mouse_show(const fn_call& fn)
	{
		as_mouse*	mouse = cast_to<as_mouse>(fn.this_ptr);
		render_handler* rh = get_render_handler();
		if (mouse && rh)
		{
			rh->set_cursor(render_handler::VISIBLE_CURSOR);
		}
	}

	void	as_mouse_add_listener(const fn_call& fn)
	{
		as_mouse*	mouse = cast_to<as_mouse>(fn.this_ptr);
		if (mouse && fn.nargs > 0)
		{
			as_object* listener = fn.arg(0).to_object();
			mouse->get_root()->add_mouse_listener(listener);
		}
	}

	void	as_mouse_remove_listener(const fn_call& fn)
	{
		fn.result->set_bool(false);
		as_mouse*	mouse = cast_to<as_mouse>(fn.this_ptr);
		if (mouse && fn.nargs > 0)
		{
			as_object* listener = fn.arg(0).to_object();
			bool rc = mouse->get_root()->remove_mouse_listener(listener);
			fn.result->set_bool(rc);
		}
	}

	as_mouse::as_mouse(player* player) :
		as_object(player)
	{
		builtin_member("hide", as_mouse_hide);
		builtin_member("show", as_mouse_show);
		builtin_member("addListener", as_mouse_add_listener);
		builtin_member("removeListener", as_mouse_remove_listener);
	}

}

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
