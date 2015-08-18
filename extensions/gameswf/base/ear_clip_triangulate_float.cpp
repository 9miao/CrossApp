// ear_clip_triangulate_float.cpp	-- Thatcher Ulrich 2006

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Triangulation, specialized for floating-point coords.


#include "ear_clip_triangulate_impl.h"


namespace ear_clip_triangulate {
	void compute(
		swf_array<float>* results,
		int path_count,
		const swf_array<float> paths[],
		int debug_halt_step,
		swf_array<float>* debug_edges)
	{
		ear_clip_triangulate::compute_triangulation<float>(results, path_count, paths, debug_halt_step, debug_edges);
	}
}

