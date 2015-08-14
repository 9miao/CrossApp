// ear_clip_triangulate.h	-- Thatcher Ulrich 2006

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code to triangulate 2D polygonal regions.
//
// see ear_clip_triangulate.cpp for more details.


#ifndef EAR_CLIP_TRIANGULATE_H
#define EAR_CLIP_TRIANGULATE_H

#include "container.h"
#include "tu_types.h"

namespace ear_clip_triangulate
{
	// Triangulators that take input arrays and output trilist
	// coordinate arrays.

	// The triangulator for each coord type is implemented in
	// different .cpp files, to avoid code bloat if you only need
	// to link to one version.

	// coord type sint16
	void compute(
		array<sint16>* results,
		int path_count,
		const array<sint16> paths[],
		int debug_halt_step = -1,
		array<sint16>* debug_remaining_loop = NULL);

	// coord type float
	void compute(
		array<float>* results,
		int path_count,
		const array<float> paths[],
		int debug_halt_step = -1,
		array<float>* debug_remaining_loop = NULL);
}


#endif // EAR_CLIP_TRIANGULATE_H
