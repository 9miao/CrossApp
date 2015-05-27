// as_geom.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "as_geom.h"
#include "as_point.h"
#include "as_matrix.h"
#include "as_transform.h"
#include "as_color_transform.h"

namespace gameswf
{
	//
	// geom object
	//

	as_object* geom_init(player* player)
	{
		// Create built-in geom object.
		as_object*	geom_obj = new as_object(player);

		// constant
		geom_obj->builtin_member("Point", as_global_point_ctor);
		geom_obj->builtin_member("Matrix", as_global_matrix_ctor);
		geom_obj->builtin_member("Transform", as_global_transform_ctor);
		geom_obj->builtin_member("ColorTransform", as_global_color_transform_ctor);

		return geom_obj;
	}

}
