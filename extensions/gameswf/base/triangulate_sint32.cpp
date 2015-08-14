// triangulate_sint32.cpp	-- Thatcher Ulrich 2004

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code to triangulate arbitrary 2D polygonal regions.
//
// Instantiate our templated algo from triangulate_inst.h

#include "triangulate_impl.h"


namespace triangulate
{
	// Version using sint32 coords
	void	compute(
		swf_array<sint32>* result,	// trilist
		int path_count,
		const swf_array<sint32> paths[],
		int debug_halt_step /* = -1 */,
		swf_array<sint32>* debug_remaining_loop /* = NULL */)
	{
		compute_triangulation<sint32>(result, path_count, paths, debug_halt_step, debug_remaining_loop);
	}
}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
