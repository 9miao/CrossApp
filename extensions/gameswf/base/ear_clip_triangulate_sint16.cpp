// ear_clip_triangulate_sint16.cpp	-- Thatcher Ulrich 2006

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Triangulation, specialized for floating-point coords.


#include "ear_clip_triangulate_impl.h"


namespace ear_clip_triangulate {
	void compute(
		swf_array<sint16>* results,
		int path_count,
		const swf_array<sint16> paths[],
		int debug_halt_step,
		swf_array<sint16>* debug_edges)
	{
		ear_clip_triangulate::compute_triangulation<sint16>(results, path_count, paths, debug_halt_step, debug_edges);
	}
}

