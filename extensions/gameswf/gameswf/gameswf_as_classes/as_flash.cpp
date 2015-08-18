// as_flash.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "as_flash.h"
#include "as_geom.h"
#include "as_event.h"

namespace gameswf
{
	//
	// flash object
	//

	as_object* flash_init(player* player)
	{
		// Create built-in flash object.
		as_object*	flash_obj = new as_object(player);

		// constant
		flash_obj->builtin_member("geom", geom_init(player));

		// flash9, events handler
		flash_obj->builtin_member("Events",  event_init(player));

		return flash_obj;
	}

}
