// triangulate_impl.h	-- Thatcher Ulrich 2004

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code to triangulate arbitrary 2D polygonal regions.
//
// Use the basic robust ear-clipping algorithm from "FIST: Fast
// Industrial-Strength Triangulation of Polygons" by Martin Held.
//
// NOTE: This code is based on the algorithm described in the FIST
// paper, but this is NOT the official FIST code written by Martin
// Held!  This code may not be as robust or as fast as FIST, and is
// certainly not as well tested.  Also, it deviates in some places
// from the algorithm as described in the FIST paper; I have tried to
// document those places in the code, along with my reasoning, but
// this code is not warranted in any way.
//
// In particular, the recovery_process is currently not as good as
// official FIST or even what's in the FIST paper.  This routine may
// do some ugly stuff with self-intersecting input.
//
// For information on obtaining the offical industrial-strength FIST
// code, see the FIST web page at:
// http://www.cosy.sbg.ac.at/~held/projects/triang/triang.html


#include "utility.h"
#include "triangulate.h"
#include "tu_random.h"
#include "grid_index.h"
#include "vert_types.h"

#define PROFILE_TRIANGULATE
#ifdef PROFILE_TRIANGULATE
#include "tu_timer.h"
#endif // PROFILE_TRIANGULATE


// Template this whole thing on coord_t, so sint16 and float versions
// can easily reuse the code.
//
// These templates are instantiated in triangulate_<type>.cpp files.
// They're in separate cpp files so the linker will discard code for
// unused types.

template<class coord_t>
bool	vertex_in_ear(const vec2<coord_t>& v, const vec2<coord_t>& a, const vec2<coord_t>& b, const vec2<coord_t>& c)
// Return true if v is on or inside the ear (a,b,c).
// (a,b,c) must be in ccw order!
{
	assert(vertex_left_test(b, a, c) <= 0);	// check ccw order

	if (v == a || v == c)
	{
		// Special case; we don't care if v is coincident with a or c.
		return false;
	}

	// Include the triangle boundary in our test.
	bool	ab_in = vertex_left_test(a, b, v) >= 0;
	bool	bc_in = vertex_left_test(b, c, v) >= 0;
	bool	ca_in = vertex_left_test(c, a, v) >= 0;

	return ab_in && bc_in && ca_in;
}


template<class coord_t> struct poly;



inline int	remap_index_for_duped_verts(int index, int duped_v0, int duped_v1)
// Helper.  Return the new value of index, for the case of verts [duped_v0]
// and [duped_v1] being duplicated and subsequent verts being shifted up.
{
	assert(duped_v0 < duped_v1);
	if (index <= duped_v0)
	{
		// No shift.
		return index;
	}
	else if (index <= duped_v1)
	{
		// Shift up one.
		return index + 1;
	}
	else
	{
		// Shift up two.
		return index + 2;
	}
}


template<class coord_t>
struct poly_vert
{
	poly_vert() {}
	poly_vert(coord_t x, coord_t y, poly<coord_t>* owner, int my_index)
		:
		m_v(x, y),
		m_my_index(my_index),
		m_next(-1),
		m_prev(-1),
		m_convex_result(0),	// 1 (convex), 0 (colinear), -1 (reflex)
		m_is_ear(false),
		m_poly_owner(owner)
	{
	}

	void	remap(const array<int>& remap_table)
	{
		m_my_index = remap_table[m_my_index];
		m_next = remap_table[m_next];
		m_prev = remap_table[m_prev];
	}

	index_point<coord_t>	get_index_point() const { return index_point<coord_t>(m_v.x, m_v.y); }

//data:
	vec2<coord_t>	m_v;
	int	m_my_index;	// my index into sorted_verts array
	int	m_next;
	int	m_prev;
	int	m_convex_result;	// (@@ only need 2 bits)
	bool	m_is_ear;
	poly<coord_t>*	m_poly_owner;	 // needed?
};


template<class coord_t>
int	compare_vertices(const void* a, const void* b)
// For qsort.  Sort by x, then by y.
{
	const poly_vert<coord_t>* vert_a = (const poly_vert<coord_t>*) a;
	const poly_vert<coord_t>* vert_b = (const poly_vert<coord_t>*) b;

	if (vert_a->m_v.x < vert_b->m_v.x)
		return -1;
	else if (vert_a->m_v.x > vert_b->m_v.x)
		return 1;
	else
	{
		if (vert_a->m_v.y < vert_b->m_v.y)
			return -1;
		else if (vert_a->m_v.y > vert_b->m_v.y)
			return 1;
	}

	return 0;
}


template<class coord_t>
inline bool	edges_intersect_sub(const array<poly_vert<coord_t> >& sorted_verts, int e0v0, int e0v1, int e1v0, int e1v1)
// Return true if edge (e0v0,e0v1) intersects (e1v0,e1v1).
{
	// Need to specialize this on coord_t, in order to get it
	// correct and avoid overflow.
	//compiler_assert(0);
	return -1;
}


template<>
inline bool	edges_intersect_sub(const array<poly_vert<float> >& sorted_verts, int e0v0i, int e0v1i, int e1v0i, int e1v1i)
// Return true if edge (e0v0,e0v1) intersects (e1v0,e1v1).
//
// Specialized for float.
{
	// If e1v0,e1v1 are on opposite sides of e0, and e0v0,e0v1 are
	// on opposite sides of e1, then the segments cross.  These
	// are all determinant checks.

	// The main degenerate case we need to watch out for is if
	// both segments are zero-length.
	//
	// If only one is degenerate, our tests are still OK.

	const vec2<float>&	e0v0 = sorted_verts[e0v0i].m_v;
	const vec2<float>&	e0v1 = sorted_verts[e0v1i].m_v;
	const vec2<float>&	e1v0 = sorted_verts[e1v0i].m_v;
	const vec2<float>&	e1v1 = sorted_verts[e1v1i].m_v;

	// Note: exact equality here.  I think the reason to use
	// epsilons would be underflow in case of very small
	// determinants.  Our determinants are doubles, so I think
	// we're good.
	if (e0v0.x == e0v1.x && e0v0.y == e0v1.y)
	{
		// e0 is zero length.
		if (e1v0.x == e1v1.x && e1v0.y == e1v1.y)
		{
			// Both edges are zero length.
			// They intersect only if they're coincident.
			return e0v0.x == e1v0.x && e0v0.y == e1v0.y;
		}
	}

	// See if e1 crosses line of e0.
	double	det10 = determinant_float(e0v0, e0v1, e1v0);
	double	det11 = determinant_float(e0v0, e0v1, e1v1);

	// Note: we do > 0, which means a vertex on a line counts as
	// intersecting.  In general, if one vert is on the other
	// segment, we have to go searching along the path in either
	// direction to see if it crosses or not, and it gets
	// complicated.  Better to treat it as intersection.

	if (det10 * det11 > 0)
	{
		// e1 doesn't cross the line of e0.
		return false;
	}

	// See if e0 crosses line of e1.
	double	det00 = determinant_float(e1v0, e1v1, e0v0);
	double	det01 = determinant_float(e1v0, e1v1, e0v1);

	if (det00 * det01 > 0)
	{
		// e0 doesn't cross the line of e1.
		return false;
	}

	// They both cross each other; the segments intersect.
	return true;
}


template<>
inline bool	edges_intersect_sub(const array<poly_vert<sint32> >& sorted_verts, int e0v0i, int e0v1i, int e1v0i, int e1v1i)
// Return true if edge (e0v0,e0v1) intersects (e1v0,e1v1).
//
// Specialized for sint32
{
	// If e1v0,e1v1 are on opposite sides of e0, and e0v0,e0v1 are
	// on opposite sides of e1, then the segments cross.  These
	// are all determinant checks.

	// The main degenerate case we need to watch out for is if
	// both segments are zero-length.
	//
	// If only one is degenerate, our tests are still OK.

	const vec2<sint32>&	e0v0 = sorted_verts[e0v0i].m_v;
	const vec2<sint32>&	e0v1 = sorted_verts[e0v1i].m_v;
	const vec2<sint32>&	e1v0 = sorted_verts[e1v0i].m_v;
	const vec2<sint32>&	e1v1 = sorted_verts[e1v1i].m_v;

	if (e0v0.x == e0v1.x && e0v0.y == e0v1.y)
	{
		// e0 is zero length.
		if (e1v0.x == e1v1.x && e1v0.y == e1v1.y)
		{
			// Both edges are zero length.  Treat them as
			// non-coincident (even if they're all on the
			// same point).
			return false;
		}
	}

	// See if e1 crosses line of e0.
	double	det10 = determinant_sint32(e0v0, e0v1, e1v0);
	double	det11 = determinant_sint32(e0v0, e0v1, e1v1);

	// Note: we do > 0, which means a vertex on a line counts as
	// intersecting.  In general, if one vert is on the other
	// segment, we have to go searching along the path in either
	// direction to see if it crosses or not, and it gets
	// complicated.  Better to treat it as intersection.

	if (det10 * det11 > 0)
	{
		// e1 doesn't cross the line of e0.
		return false;
	}

	// See if e0 crosses line of e1.
	double det00 = determinant_sint32(e1v0, e1v1, e0v0);
	double det01 = determinant_sint32(e1v0, e1v1, e0v1);

	if (det00 * det01 > 0)
	{
		// e0 doesn't cross the line of e1.
		return false;
	}

	// They both cross each other; the segments intersect.
	return true;
}


template<class coord_t>
bool	edges_intersect(const array<poly_vert<coord_t> >& sorted_verts, int e0v0, int e0v1, int e1v0, int e1v1)
// Return true if edge (e0v0,e0v1) intersects (e1v0,e1v1).
{
	// Deal with special case: edges that share exactly one vert.
	// We treat these as no intersection, even though technically
	// they share one point.
	//
	// We're not just comparing indices, because duped verts (for
	// bridges) might have different indices.
	//
	// @@ this needs review -- might be wrong.
	bool	coincident[2][2];
	coincident[0][0] = (sorted_verts[e0v0].m_v == sorted_verts[e1v0].m_v);
	coincident[0][1] = (sorted_verts[e0v0].m_v == sorted_verts[e1v1].m_v);
	coincident[1][0] = (sorted_verts[e0v1].m_v == sorted_verts[e1v0].m_v);
	coincident[1][1] = (sorted_verts[e0v1].m_v == sorted_verts[e1v1].m_v);
	if (coincident[0][0] && !coincident[1][1]) return false;
	if (coincident[1][0] && !coincident[0][1]) return false;
	if (coincident[0][1] && !coincident[1][0]) return false;
	if (coincident[1][1] && !coincident[0][0]) return false;

// @@ eh, I think we really want this to be an intersection
#if 0
	// Both verts identical: early out.
	//
	// Note: treat this as no intersection!  This is mainly useful
	// for things like coincident vertical bridge edges.
	if (coincident[0][0] && coincident[1][1]) return false;
	if (coincident[1][0] && coincident[0][1]) return false;
#endif // 0

	// Check for intersection.
	return edges_intersect_sub(sorted_verts, e0v0, e0v1, e1v0, e1v1);
}



template<class coord_t>
bool	is_convex_vert(const array<poly_vert<coord_t> >& sorted_verts, int vi)
// Return true if vert vi is convex.
{
	const poly_vert<coord_t>*	pvi = &(sorted_verts[vi]);
	const poly_vert<coord_t>*	pv_prev = &(sorted_verts[pvi->m_prev]);
	const poly_vert<coord_t>*	pv_next = &(sorted_verts[pvi->m_next]);

	return vertex_left_test<coord_t>(pv_prev->m_v, pvi->m_v, pv_next->m_v) > 0;
}


template<class coord_t>
struct poly
{
	typedef poly_vert<coord_t> vert_t;

	poly(/*@@ TODO array<vert_t>* sorted_verts*/)
		:
		// @@ TODO m_sorted_verts(sorted_verts),
		m_loop(-1),
		m_leftmost_vert(-1),
		m_vertex_count(0),
		m_ear_count(0),
		m_edge_index(NULL),
		m_reflex_point_index(NULL)
	{
	}

	~poly()
	{
		delete m_edge_index;
		m_edge_index = NULL;

		delete m_reflex_point_index;
		m_reflex_point_index = NULL;
	}

	bool	is_valid(const array<vert_t>& sorted_verts, bool check_consecutive_dupes = true) const;

	// init/prep
	void	append_vert(array<vert_t>* sorted_verts, int vert_index);
	void	remap(const array<int>& remap_table);
	void	remap_for_duped_verts(const array<vert_t>& sorted_verts, int v0, int v1);

	void	init_edge_index(const array<vert_t>& sorted_verts, index_box<coord_t>& bound_of_all_verts);
	int	find_valid_bridge_vert(const array<vert_t>& sorted_verts, int v1);
	void	update_connected_sub_poly(array<vert_t>* sorted_verts, int v_start, int v_stop);
	void	init_for_ear_clipping(array<vert_t>* sorted_verts);

	// Edges are indexed using their first vert (i.e. edge (v1,v2) is indexed using v1)
	void	add_edge(const array<vert_t>& sorted_verts, int vi);
	void	remove_edge(const array<vert_t>& sorted_verts, int vi);

	// tests/queries
	bool	any_edge_intersection(const array<vert_t>& sorted_verts, int external_vert, int v2);
	bool	vert_can_see_cone_a(const array<vert_t>& sorted_verts, int v, int cone_a_vert, int cone_b_vert);
	bool	vert_in_cone(const array<vert_t>& sorted_verts, int vert, int cone_v0, int cone_v1, int cone_v2);
	bool	vert_is_duplicated(const array<vert_t>& sorted_verts, int v0);
	bool	ear_contains_reflex_vertex(const array<vert_t>& sorted_verts, int v0, int v1, int v2);

	void	classify_vert(array<vert_t>* sorted_verts, int vi);
	void	dirty_vert(array<vert_t>* sorted_verts, int vi);

	int	get_vertex_count() const { return m_vertex_count; }
	int	get_ear_count() const { return m_ear_count; }
	int	get_next_ear(const array<vert_t>& sorted_verts, tu_random::generator* rg);
	int	remove_degenerate_chain(array<vert_t>* sorted_verts, int vi);
	void	emit_and_remove_ear(array<coord_t>* result, array<vert_t>* sorted_verts, int v0, int v1, int v2);
	bool	build_ear_list(array<vert_t>* sorted_verts, tu_random::generator* rg);

	void	invalidate(const array<vert_t>& sorted_verts);

//data:
//@@ TODO	array<vert_t>*	m_sorted_verts;
	int	m_loop;	// index of first vert
	int	m_leftmost_vert;
	int	m_vertex_count;
	int	m_ear_count;

	// edge search_index (for finding possibly intersecting edges during bridge finding)
	//
	// The payload stored is the index of the first vert in the edge.
	typedef grid_index_box<coord_t, int>	box_index_t;
	typedef typename box_index_t::iterator ib_iterator;
	grid_index_box<coord_t, int>*	m_edge_index;

	// point search index (for finding reflex verts within a potential ear)
	typedef grid_index_point<coord_t, int>	point_index_t;
	typedef typename point_index_t::iterator ip_iterator;
	point_index_t*	m_reflex_point_index;
};


template<class coord_t>
bool	poly<coord_t>::is_valid(const array<vert_t>& sorted_verts, bool check_consecutive_dupes /* = true */) const
// Assert validity.
{
#ifndef NDEBUG

	if (m_loop == -1 && m_leftmost_vert == -1 && m_vertex_count == 0)
	{
		// Empty poly.
		return true;
	}

	assert(m_leftmost_vert == -1 || sorted_verts[m_leftmost_vert].m_poly_owner == this);

	// Check vert count.
	int	first_vert = m_loop;
	int	vi = first_vert;
	int	vert_count = 0;
	int	ear_count = 0;
	bool	found_leftmost = false;
	int	reflex_vert_count = 0;
	do
	{
		const poly_vert<coord_t>*	pvi = &sorted_verts[vi];

		// Check ownership.
		assert(pvi->m_poly_owner == this);

		// Check leftmost vert.
		assert(m_leftmost_vert == -1
		       || compare_vertices<coord_t>(
			       (const void*) &sorted_verts[m_leftmost_vert],
			       (const void*) &sorted_verts[vi]) <= 0);

		// Check link integrity.
		int	v_next = pvi->m_next;
		assert(sorted_verts[v_next].m_prev == vi);

		if (vi == m_leftmost_vert)
		{
			found_leftmost = true;
		}

		if (check_consecutive_dupes && v_next != vi)
		{
			// Subsequent verts are not allowed to be
			// coincident; that causes errors in ear
			// classification.
			assert((pvi->m_v == sorted_verts[v_next].m_v) == false);
		}

		if (pvi->m_convex_result < 0)
		{
			reflex_vert_count++;
		}
		if (pvi->m_is_ear)
		{
			ear_count++;
		}
		vert_count++;
		vi = v_next;
	}
	while (vi != first_vert);

	assert(ear_count == m_ear_count);
	assert(vert_count == m_vertex_count);
	assert(found_leftmost || m_leftmost_vert == -1);

	// Count reflex verts in the grid index.
	if (m_reflex_point_index)
	{
		int	check_count = 0;
		for (ip_iterator it = m_reflex_point_index->begin(m_reflex_point_index->get_bound());
		     ! it.at_end();
		     ++it)
		{
			check_count++;
		}

		assert(check_count == reflex_vert_count);
	}

	// Count edges in the edge index.  There should be exactly one edge per vert.
	if (m_edge_index)
	{
		int	check_count = 0;
		for (ib_iterator it = m_edge_index->begin(m_edge_index->get_bound());
		     ! it.at_end();
		     ++it)
		{
			check_count++;
		}

		assert(check_count == vert_count);
	}

	// Might be nice to check that all verts with (m_poly_owner ==
	// this) are in our loop.

#endif // not NDEBUG

	return true;
}


template<class coord_t>
void	poly<coord_t>::invalidate(const array<vert_t>& sorted_verts)
// Mark as invalid/empty.  Do this after linking into another poly,
// for safety/debugging.
{
	assert(m_loop == -1 || sorted_verts[m_loop].m_poly_owner != this);	// make sure our verts have been stolen already.

	m_loop = -1;
	m_leftmost_vert = -1;
	m_vertex_count = 0;

	assert(is_valid(sorted_verts));
}


template<class coord_t>
int	compare_polys_by_leftmost_vert(const void* a, const void* b)
{
	const poly<coord_t>*	poly_a = * (const poly<coord_t>**) a;
	const poly<coord_t>*	poly_b = * (const poly<coord_t>**) b;

	// Vert indices are sorted, so we just compare the indices,
	// not the actual vert coords.
	if (poly_a->m_leftmost_vert < poly_b->m_leftmost_vert)
	{
		return -1;
	}
	else
	{
		// polys are not allowed to share verts, so the
		// leftmost vert must be different!
		assert(poly_a->m_leftmost_vert > poly_b->m_leftmost_vert);

		return 1;
	}
}


template<class coord_t>
void	poly<coord_t>::append_vert(array<vert_t>* sorted_verts, int vert_index)
// Link the specified vert into our loop.
{
	assert(vert_index >= 0 && vert_index < sorted_verts->size());
	assert(is_valid(*sorted_verts, false /* don't check for consecutive dupes, poly is not finished */));

	m_vertex_count++;

	if (m_loop == -1)
	{
		// First vert.
		assert(m_vertex_count == 1);
		m_loop = vert_index;
		poly_vert<coord_t>*	pv = &(*sorted_verts)[vert_index];
		pv->m_next = vert_index;
		pv->m_prev = vert_index;
		pv->m_poly_owner = this;

		m_leftmost_vert = vert_index;
	}
	else
	{
		// We have a loop.  Link the new vert in, behind the
		// first vert.
		poly_vert<coord_t>*	pv0 = &(*sorted_verts)[m_loop];
		poly_vert<coord_t>*	pv = &(*sorted_verts)[vert_index];
		pv->m_next = m_loop;
		pv->m_prev = pv0->m_prev;
		pv->m_poly_owner = this;
		(*sorted_verts)[pv0->m_prev].m_next = vert_index;
		pv0->m_prev = vert_index;

		// Update m_leftmost_vert
		poly_vert<coord_t>*	pvl = &(*sorted_verts)[m_leftmost_vert];
		if (compare_vertices<coord_t>(pv, pvl) < 0)
		{
			// v is to the left of vl; it's the new leftmost vert
			m_leftmost_vert = vert_index;
		}
	}

	assert(is_valid(*sorted_verts, false /* don't check for consecutive dupes, poly is not finished */));
}


template<class coord_t>
int	poly<coord_t>::find_valid_bridge_vert(const array<vert_t>& sorted_verts, int v1)
// Find a vert v, in this poly, such that v is to the left of v1, and
// the edge (v,v1) doesn't intersect any edges in this poly.
{
	assert(is_valid(sorted_verts));

	const poly_vert<coord_t>*	pv1 = &(sorted_verts[v1]);
	assert(pv1->m_poly_owner != this);	// v1 must not be part of this poly already

	// Held recommends searching verts near v1 first.  And for
	// correctness, we may only consider verts to the left of v1.
	// A fast & easy way to implement this is to walk backwards in
	// our vert array, starting with v1-1.

	// Walk forward to include all coincident but later verts!
	int	vi = v1;
	while ((vi + 1) < sorted_verts.size() && sorted_verts[vi + 1].m_v == pv1->m_v)
	{
		vi++;
	}

	// Now scan backwards for the vert to bridge onto.
	for ( ; vi >= 0; vi--)
	{
		const poly_vert<coord_t>*	pvi = &sorted_verts[vi];

		assert(compare_vertices<coord_t>((void*) pvi, (void*) pv1) <= 0);

		if (pvi->m_poly_owner == this)
		{
			// Candidate is to the left of pv1, so it
			// might be valid.  We don't consider verts to
			// the right of v1, because of possible
			// intersection with other polys.  Due to the
			// poly sorting, we know that the edge
			// (pvi,pv1) can only intersect this poly.

			if (any_edge_intersection(sorted_verts, v1, vi) == false)
			{
				return vi;
			}
		}
	}

	// Ugh!  No valid bridge vert.  Shouldn't happen with valid
	// data.  For invalid data, just pick something and live with
	// the intersection.
	fprintf(stderr, "can't find bridge for vert %d!\n", v1);//xxxxxxxxx

	return m_leftmost_vert;
}


template<class coord_t>
void	poly<coord_t>::remap(const array<int>& remap_table)
{
	assert(m_loop > -1);
	assert(m_leftmost_vert > -1);

	m_loop = remap_table[m_loop];
	m_leftmost_vert = remap_table[m_leftmost_vert];
}


template<class coord_t>
void	poly<coord_t>::remap_for_duped_verts(const array<vert_t>& sorted_verts, int v0, int v1)
// Remap for the case of v0 and v1 being duplicated, and subsequent
// verts being shifted up.
{
	assert(m_loop > -1);
	assert(m_leftmost_vert > -1);

	m_loop = remap_index_for_duped_verts(m_loop, v0, v1);
	m_leftmost_vert = remap_index_for_duped_verts(m_leftmost_vert, v0, v1);

	// Remap the vert indices stored in the edge index.
	if (m_edge_index)
	{
		// Optimization: we don't need to remap anything
		// that's wholly to the left of v0.  Towards the end
		// of bridge building, this could be the vast majority
		// of edges.
		assert(v0 < v1);
		index_box<coord_t>	bound = m_edge_index->get_bound();
		bound.min.x = sorted_verts[v0].m_v.x;

		for (ib_iterator it = m_edge_index->begin(bound);
		     ! it.at_end();
		     ++it)
		{
			it->value = remap_index_for_duped_verts(it->value, v0, v1);
		}
	}

	// We shouldn't have a point index right now.
	assert(m_reflex_point_index == NULL);
}


template<class coord_t>
void	poly<coord_t>::classify_vert(array<vert_t>* sorted_verts, int vi)
// Decide if vi is an ear, and mark its m_is_ear flag & update counts.
{
	poly_vert<coord_t>*	pvi = &((*sorted_verts)[vi]);
	const poly_vert<coord_t>*	pv_prev = &((*sorted_verts)[pvi->m_prev]);
	const poly_vert<coord_t>*	pv_next = &((*sorted_verts)[pvi->m_next]);

	if (pvi->m_convex_result > 0)
	{
		if (vert_in_cone(*sorted_verts, pvi->m_prev, vi, pvi->m_next, pv_next->m_next)
		    && vert_in_cone(*sorted_verts, pvi->m_next, pv_prev->m_prev, pvi->m_prev, vi))
		{
			if (! ear_contains_reflex_vertex(*sorted_verts, pvi->m_prev, vi, pvi->m_next))
			{
				// Valid ear.
				assert(pvi->m_is_ear == false);
				pvi->m_is_ear = true;
				m_ear_count++;
			}
		}
	}
}


template<class coord_t>
void	poly<coord_t>::dirty_vert(array<vert_t>* sorted_verts, int vi)
// Call when an adjacent vert gets clipped.  Recomputes
// m_convex_result and clears m_is_ear for the vert.
{
	poly_vert<coord_t>*	pvi = &((*sorted_verts)[vi]);

	int	new_convex_result =
		vertex_left_test<coord_t>((*sorted_verts)[pvi->m_prev].m_v, pvi->m_v, (*sorted_verts)[pvi->m_next].m_v);
	if (new_convex_result < 0 && pvi->m_convex_result >= 0)
	{
		// Vert is newly reflex.
		// Add to reflex vert index
		assert(m_reflex_point_index);
		m_reflex_point_index->add(index_point<coord_t>(pvi->m_v.x, pvi->m_v.y), vi);
	}
	else if (pvi->m_convex_result < 0 && new_convex_result >= 0)
	{
		// Vert is newly convex/colinear.
		// Remove from reflex vert index.
		assert(m_reflex_point_index);
		ip_iterator	it = m_reflex_point_index->find(index_point<coord_t>(pvi->m_v.x, pvi->m_v.y), vi);
		assert(it.at_end() == false);

		m_reflex_point_index->remove(&(*it));
	}
	pvi->m_convex_result = new_convex_result;

	if (pvi->m_is_ear)
	{
		// Clear its ear flag.
		pvi->m_is_ear = false;
		m_ear_count--;
	}
}


template<class coord_t>
bool	poly<coord_t>::build_ear_list(array<vert_t>* sorted_verts, tu_random::generator* rg)
// Initialize our ear loop with all the ears that can be clipped.
//
// Returns true if we clipped any degenerates while looking for ears.
{
	assert(is_valid(*sorted_verts));
	assert(m_ear_count == 0);

	bool	clipped_any_degenerates = false;

	if (m_vertex_count < 3)
	{
		// Not a real poly, no ears.
		return false;
	}

	// Go around the loop, evaluating the verts.
	int	vi = m_loop;
	int	verts_processed_count = 0;
	for (;;)
	{
		const poly_vert<coord_t>*	pvi = &((*sorted_verts)[vi]);
		const poly_vert<coord_t>*	pv_prev = &((*sorted_verts)[pvi->m_prev]);
		const poly_vert<coord_t>*	pv_next = &((*sorted_verts)[pvi->m_next]);

		// classification of ear, CE2 from FIST paper:
		//
		// v[i-1],v[i],v[i+1] of P form an ear of P iff
		//
		// 1. v[i] is a convex vertex
		//
		// 2. the interior plus boundary of triangle
		// v[i-1],v[i],v[i+1] does not contain any reflex vertex of P
		// (except v[i-1] or v[i+1])
		//
		// 3. v[i-1] is in the cone(v[i],v[i+1],v[i+2]) and v[i+1] is
		// in the cone(v[i-2],v[i-1],v[i]) (not strictly necessary,
		// but used for efficiency and robustness)

		if ((pvi->m_v == pv_next->m_v)
		    || (pvi->m_v == pv_prev->m_v)
		    || (vertex_left_test(pv_prev->m_v, pvi->m_v, pv_next->m_v) == 0
			&& vert_is_duplicated(*sorted_verts, vi) == false))
		{
			// Degenerate case: zero-area triangle.
			//
			// Remove it (and any additional degenerates chained onto this ear).
			vi = remove_degenerate_chain(sorted_verts, vi);

			clipped_any_degenerates = true;

			if (m_vertex_count < 3)
			{
				break;
			}

			continue;
		}
		else
		{
			classify_vert(sorted_verts, vi);
		}

		vi = pvi->m_next;
		verts_processed_count++;

		if (verts_processed_count >= m_vertex_count)
		{
			break;
		}

		// Performance optimization: if we're finding lots of
		// ears, keep our working set local by processing a
		// few ears soon after examining them.
		if (m_ear_count > 5 && verts_processed_count > 10)
		{
			break;
		}
	}

	assert(is_valid(*sorted_verts, true /* do check for dupes */));

	// @@ idea for cheap ear shape control: m_loop = best_ear_found;

	return clipped_any_degenerates;
}


template<class coord_t>
int	poly<coord_t>::get_next_ear(const array<vert_t>& sorted_verts, tu_random::generator* rg)
// Return the next ear to be clipped.
{
	assert(m_ear_count > 0);

	while (sorted_verts[m_loop].m_is_ear == false)
	{
		m_loop = sorted_verts[m_loop].m_next;
	}

	int	next_ear = m_loop;

// define this if you want to randomize the ear selection (should
// improve the average ear shape, at low cost).
//#define RANDOMIZE
#ifdef RANDOMIZE
	// Randomization: skip a random number of ears.
	if (m_ear_count > 6)
	{
		// Decide how many ears to skip.

		// Here's a lot of twiddling to avoid a % op.  Worth it?
		int	random_range = m_ear_count >> 2;
		static const int	MASK_TABLE_SIZE = 8;
		int	random_mask[MASK_TABLE_SIZE] = {
			1, 1, 1, 3, 3, 3, 3, 7	// roughly, the largest (2^N-1) <= index
		};
		if (random_range >= MASK_TABLE_SIZE) random_range = MASK_TABLE_SIZE - 1;
		assert(random_range > 0);

		int	random_skip = rg->next_random() & random_mask[random_range];

		// Do the skipping, by manipulating m_loop.
		while (random_skip > 0)
		{
			if (sorted_verts[m_loop].m_is_ear)
			{
				random_skip--;
			}
			m_loop = sorted_verts[m_loop].m_next;
		}

		assert(is_valid(sorted_verts));
	}
#endif // RANDOMIZE

	assert(sorted_verts[next_ear].m_is_ear == true);

	return next_ear;
}


template<class coord_t>
void	poly<coord_t>::emit_and_remove_ear(
	array<coord_t>* result,
	array<vert_t>* sorted_verts,
	int v0,
	int v1,
	int v2)
// Push the ear triangle into the output; remove the triangle
// (i.e. vertex v1) from this poly.
{
	assert(is_valid(*sorted_verts));
	assert(m_vertex_count >= 3);

	poly_vert<coord_t>*	pv0 = &(*sorted_verts)[v0];
	poly_vert<coord_t>*	pv1 = &(*sorted_verts)[v1];
	poly_vert<coord_t>*	pv2 = &(*sorted_verts)[v2];

	assert((*sorted_verts)[v1].m_is_ear);

	if (m_loop == v1)
	{
		// Change m_loop, since we're about to lose it.
		m_loop = v0;
	}

	// Make sure m_leftmost_vert is dead; we don't need it now.
	m_leftmost_vert = -1;

	if (vertex_left_test(pv0->m_v, pv1->m_v, pv2->m_v) == 0)
	{
		// Degenerate triangle!  Don't emit it.
		assert(0);	// This should have already been removed by remove_degenerate_chain().
	}
	else
	{
		// emit the vertex list for the triangle.
		result->push_back(pv0->m_v.x);
		result->push_back(pv0->m_v.y);
		result->push_back(pv1->m_v.x);
		result->push_back(pv1->m_v.y);
		result->push_back(pv2->m_v.x);
		result->push_back(pv2->m_v.y);
	}

	// Unlink v1.

	if (pv1->m_convex_result < 0)
	{
		// Vert was reflex (can happen due to e.g. recovery).
		// Remove from reflex vert index.
		assert(m_reflex_point_index);
		ip_iterator it = m_reflex_point_index->find(index_point<coord_t>(pv1->m_v.x, pv1->m_v.y), v1);
		assert(it.at_end() == false);

		m_reflex_point_index->remove(&(*it));
	}

	assert(pv0->m_poly_owner == this);
	assert(pv1->m_poly_owner == this);
	assert(pv2->m_poly_owner == this);

	pv0->m_next = v2;
	pv2->m_prev = v0;

	pv1->m_next = -1;
	pv1->m_prev = -1;
	pv1->m_poly_owner = NULL;

	// We lost v1.
	m_vertex_count--;
	m_ear_count--;

	if (pv0->m_v == pv2->m_v)
	{
		// remove_degenerate_chain() should have taken care of
		// this before we got here.
		assert(0);
	}

	// ear status of v0 and v2 could have changed now.
	dirty_vert(sorted_verts, v0);
	dirty_vert(sorted_verts, v2);

	// Big huge performance boost: recheck these local verts now;
	// often we'll clip them right away.
	//
	// @@ what happens if v0 or v2 are now degenerate???
	classify_vert(sorted_verts, v0);	
	classify_vert(sorted_verts, v2);

	assert(is_valid(*sorted_verts));
}


template<class coord_t>
int	poly<coord_t>::remove_degenerate_chain(array<vert_t>* sorted_verts, int vi)
// Remove the degenerate ear at vi, and any degenerate ear formed as
// we remove the previous one.
//
// Return the index of a vertex just prior to the chain we've removed.
{
	assert(m_leftmost_vert == -1);

	int	retval = vi;

	for (;;)
	{
		assert(is_valid(*sorted_verts, false /* don't check for dupes yet */));

		poly_vert<coord_t>*	pv1 = &(*sorted_verts)[vi];
		poly_vert<coord_t>*	pv0 = &(*sorted_verts)[pv1->m_prev];
		poly_vert<coord_t>*	pv2 = &(*sorted_verts)[pv1->m_next];

		if (m_loop == vi)
		{
			// Change m_loop, since we're about to lose it.
			m_loop = pv0->m_my_index;
		}

		// Unlink vi.

		assert(pv0->m_poly_owner == this);
		assert(pv1->m_poly_owner == this);
		assert(pv2->m_poly_owner == this);

		pv0->m_next = pv2->m_my_index;
		pv2->m_prev = pv0->m_my_index;

		pv1->m_next = -1;
		pv1->m_prev = -1;
		pv1->m_poly_owner = NULL;

		if (pv1->m_convex_result < 0)
		{
			// vi was reflex, remove it from index
			assert(m_reflex_point_index);
			ip_iterator	it =
				m_reflex_point_index->find(index_point<coord_t>(pv1->m_v.x, pv1->m_v.y), vi);
			assert(it.at_end() == false);

			m_reflex_point_index->remove(&(*it));
		}

		if (pv1->m_is_ear)
		{
			m_ear_count--;
		}

		// We lost vi.
		m_vertex_count--;

		assert(is_valid(*sorted_verts, false /* don't check for dupes yet */));

		if (m_vertex_count < 3)
		{
			retval = pv0->m_my_index;
			break;
		}

		// If we've created another degenerate, then remove it as well.
		if (pv0->m_v == pv2->m_v)
		{
			// We've created a dupe in the chain, remove it now.
			vi = pv0->m_my_index;
		}
		else if (vertex_left_test((*sorted_verts)[pv0->m_prev].m_v, pv0->m_v, pv2->m_v) == 0)
		{
			// More degenerate.
			vi = pv0->m_my_index;
		}
		else if (vertex_left_test(pv0->m_v, pv2->m_v, (*sorted_verts)[pv2->m_next].m_v) == 0)
		{
			// More degenerate.
			vi = pv2->m_my_index;
		}
		else
		{
			// ear/reflex status of pv0 & pv2 may have changed.
			dirty_vert(sorted_verts, pv0->m_my_index);
			dirty_vert(sorted_verts, pv2->m_my_index);

			retval = pv0->m_my_index;

			break;
		}
	}

	assert(is_valid(*sorted_verts, true /* do check for dupes; there shouldn't be any! */));

	return retval;
}


template<class coord_t>
void	poly<coord_t>::update_connected_sub_poly(array<vert_t>* sorted_verts, int v_first_in_subloop, int v_first_after_subloop)
// Given the beginning and end of a sub-loop that has just been linked
// into our loop, update the verts on the sub-loop to have the correct
// owner, update our m_leftmost_vert and our m_vert_count.
{
	assert(v_first_in_subloop != v_first_after_subloop);

	int	vi = v_first_in_subloop;
	do
	{
		vert_t*	pv = &(*sorted_verts)[vi];

		pv->m_poly_owner = this;
		m_vertex_count++;

		// Update leftmost vert.
		if (pv->m_my_index < m_leftmost_vert)
		{
			m_leftmost_vert = pv->m_my_index;
		}

		// Insert new edge into the edge index.
		add_edge(*sorted_verts, vi);

		vi = pv->m_next;
	}
	while (vi != v_first_after_subloop);

	assert(is_valid(*sorted_verts));
}



template<class coord_t>
void	poly<coord_t>::init_edge_index(const array<vert_t>& sorted_verts, index_box<coord_t>& bound_of_all_verts)
// Initialize our edge-search structure, for quickly finding possible
// intersecting edges (when constructing bridges to join polys).
{
	assert(is_valid(sorted_verts));
	assert(m_edge_index == NULL);

	// Compute grid density.
	int	x_cells = 1;
	int	y_cells = 1;
	if (sorted_verts.size() > 0)
	{
		const float	GRID_SCALE = sqrtf(0.5f);
		coord_t	width = bound_of_all_verts.get_width();
		coord_t	height = bound_of_all_verts.get_height();
		float	area = float(width) * float(height);
		if (area > 0)
		{
			float	sqrt_n = sqrt((float) sorted_verts.size());
			float	w = width * width / area * GRID_SCALE;
			float	h = height * height / area * GRID_SCALE;
			x_cells = int(w * sqrt_n);
			y_cells = int(h * sqrt_n);
		}
		else
		{
			// Zero area.
			if (width > 0)
			{
				x_cells = int(GRID_SCALE * GRID_SCALE * sorted_verts.size());
			}
			else
			{
				y_cells = int(GRID_SCALE * GRID_SCALE * sorted_verts.size());
			}
		}
		x_cells = iclamp(x_cells, 1, 256);
		y_cells = iclamp(y_cells, 1, 256);
	}
	
	m_edge_index = new grid_index_box<coord_t, int>(bound_of_all_verts, x_cells, y_cells);

	// Insert current edges into the index.
	int	vi = m_loop;
	for (;;)
	{
		add_edge(sorted_verts, vi);

		vi = sorted_verts[vi].m_next;
		if (vi == m_loop)
		{
			break;
		}
	}

	assert(is_valid(sorted_verts));
}


template<class coord_t>
void	poly<coord_t>::init_for_ear_clipping(array<vert_t>* sorted_verts)
// Classify all verts for convexity.
//
// Initialize our point-search structure, for quickly finding reflex
// verts within a potential ear.
{
	assert(is_valid(*sorted_verts));

	// Kill m_leftmost_vert; don't need it once all the polys are
	// joined together into one loop.
	m_leftmost_vert = -1;

	// Kill edge index; we don't need it for ear clipping.
	delete m_edge_index;
	m_edge_index = NULL;

	int	reflex_vert_count = 0;

	bool	bound_inited = false;
	index_box<coord_t>	reflex_bound(index_point<coord_t>(0, 0), index_point<coord_t>(0, 0));

	int	vi = m_loop;
	for (;;)
	{
		// Classify vi as reflex/convex.
		vert_t*	pvi = &(*sorted_verts)[vi];
		pvi->m_convex_result =
			vertex_left_test<coord_t>((*sorted_verts)[pvi->m_prev].m_v, pvi->m_v, (*sorted_verts)[pvi->m_next].m_v);
		
		if (pvi->m_convex_result < 0)
		{
			reflex_vert_count++;

			// Update bounds.
			index_point<coord_t>	location(pvi->m_v.x, pvi->m_v.y);
			if (bound_inited == false)
			{
				bound_inited = true;
				reflex_bound = index_box<coord_t>(location, location);
			}
			else
			{
				reflex_bound.expand_to_enclose(location);
			}
		}

		vi = (*sorted_verts)[vi].m_next;
		if (vi == m_loop)
		{
			break;
		}
	}

	// Compute grid density.  FIST recommends w * sqrt(N) * h *
	// sqrt(N), where w*h is between 0.5 and 2.  (N is the reflex
	// vert count.)
	int	x_cells = 1;
	int	y_cells = 1;
	if (reflex_vert_count > 0)
	{
		const float	GRID_SCALE = sqrtf(0.5f);
		coord_t	width = reflex_bound.get_width();
		coord_t	height = reflex_bound.get_height();
		float	area = float(width) * float(height);
		if (area > 0)
		{
			float	sqrt_n = sqrt((float) reflex_vert_count);
			float	w = width * width / area * GRID_SCALE;
			float	h = height * height / area * GRID_SCALE;
			x_cells = int(w * sqrt_n);
			y_cells = int(h * sqrt_n);
		}
		else
		{
			// Zero area.
			if (width > 0)
			{
				x_cells = int(GRID_SCALE * GRID_SCALE * reflex_vert_count);
			}
			else
			{
				y_cells = int(GRID_SCALE * GRID_SCALE * reflex_vert_count);
			}
		}
		x_cells = iclamp(x_cells, 1, 256);
		y_cells = iclamp(y_cells, 1, 256);
	}
	
	m_reflex_point_index = new grid_index_point<coord_t, int>(reflex_bound, x_cells, y_cells);

	// Insert reflex verts into the index.
	vi = m_loop;
	for (;;)
	{
		vert_t*	pvi = &(*sorted_verts)[vi];
		if (pvi->m_convex_result < 0)
		{
			// Reflex.  Insert it.
			m_reflex_point_index->add(index_point<coord_t>(pvi->m_v.x, pvi->m_v.y), vi);
		}

		vi = (*sorted_verts)[vi].m_next;
		if (vi == m_loop)
		{
			break;
		}
	}

	assert(is_valid(*sorted_verts));
}


template<class coord_t>
void	poly<coord_t>::add_edge(const array<vert_t>& sorted_verts, int vi)
// Insert the edge (vi, vi->m_next) into the index.
{
	index_box<coord_t>	ib(sorted_verts[vi].get_index_point());
	ib.expand_to_enclose(sorted_verts[sorted_verts[vi].m_next].get_index_point());

	assert(m_edge_index);

	// Make sure edge isn't already in the index.
	assert(m_edge_index->find_payload_from_point(sorted_verts[vi].get_index_point(), vi) == NULL);

	m_edge_index->add(ib, vi);
}


template<class coord_t>
void	poly<coord_t>::remove_edge(const array<vert_t>& sorted_verts, int vi)
// Remove the edge (vi, vi->m_next) from the index.
{
	assert(m_edge_index);

 	grid_entry_box<coord_t, int>*	entry = m_edge_index->find_payload_from_point(sorted_verts[vi].get_index_point(), vi);
 	assert(entry);

 	m_edge_index->remove(entry);
}


template<class coord_t>
bool	poly<coord_t>::vert_can_see_cone_a(const array<vert_t>& sorted_verts, int v, int cone_a_vert, int cone_b_vert)
// Return true if v can see cone_a_vert, without logically crossing cone_b.
// cone_a_vert and cone_b_vert are coincident.
{
	assert(sorted_verts[cone_a_vert].m_v == sorted_verts[cone_b_vert].m_v);
	
	// @@ Thought: Would it be more robust to know whether v is
	// part of a ccw or cw loop, and then decide based on the
	// relative insideness/outsideness of v w/r/t the cones?

	// Analyze the two cones, to see if the segment
	// (v,cone_a_vert) is blocked by cone_b_vert.  Since
	// cone_a_vert and cone_b_vert are coincident, we need to
	// figure out the relationship among v and the cones.

	// Sort the cones so that they're in convex order.
	const vert_t*	pa = &sorted_verts[cone_a_vert];
	vec2<coord_t>	cone_a[3] = { sorted_verts[pa->m_prev].m_v, pa->m_v, sorted_verts[pa->m_next].m_v };
	if (vertex_left_test(cone_a[0], cone_a[1], cone_a[2]) < 0)
	{
		tu_swap(&cone_a[0], &cone_a[2]);
	}

	const vert_t*	pb = &sorted_verts[cone_b_vert];
	vec2<coord_t>	cone_b[3] = { sorted_verts[pb->m_prev].m_v, pb->m_v, sorted_verts[pb->m_next].m_v };
	if (vertex_left_test(cone_b[0], cone_b[1], cone_b[2]) < 0)
	{
		tu_swap(&cone_b[0], &cone_b[2]);
	}

	// Characterize the cones w/r/t each other.
	int	a_in_b_sum = 0;
	a_in_b_sum += vertex_left_test(cone_b[0], cone_b[1], cone_a[0]);
	a_in_b_sum += vertex_left_test(cone_b[1], cone_b[2], cone_a[0]);
	a_in_b_sum += vertex_left_test(cone_b[0], cone_b[1], cone_a[2]);
	a_in_b_sum += vertex_left_test(cone_b[1], cone_b[2], cone_a[2]);

	int	b_in_a_sum = 0;
	b_in_a_sum += vertex_left_test(cone_a[0], cone_a[1], cone_b[0]);
	b_in_a_sum += vertex_left_test(cone_a[1], cone_a[2], cone_b[0]);
	b_in_a_sum += vertex_left_test(cone_a[0], cone_a[1], cone_b[2]);
	b_in_a_sum += vertex_left_test(cone_a[1], cone_a[2], cone_b[2]);

	// Eeek!  Need a better way of doing this...
	bool	a_in_b = false;
	if (a_in_b_sum >= 4)
	{
		assert(b_in_a_sum <= -2);
		a_in_b = true;
	}
	else if (a_in_b_sum == 3)
	{
		assert(b_in_a_sum <= 3);

		if (b_in_a_sum >= 3)
		{
			// Inconsistent (crossing cones).  No good.
			return false;
		}
		a_in_b = true;
	}
	else if (a_in_b_sum <= -4)
	{
		assert(b_in_a_sum >= 2);
		a_in_b = false;
	}
	else if (a_in_b_sum == -3)
	{
		assert(b_in_a_sum >= -3);

		if (b_in_a_sum <= -3)
		{
			// Inconsistent (crossing cones).  No good.
			return false;
		}
		a_in_b = false;
	}
	else
	{
		if (b_in_a_sum >= 4)
		{
			assert(a_in_b_sum <= -2);
			a_in_b = false;
		}
		else if (b_in_a_sum == 3)
		{
			a_in_b = false;
		}
		else if (b_in_a_sum <= -4)
		{
			assert(a_in_b_sum >= 2);
			a_in_b = true;
		}
		else if (b_in_a_sum == -3)
		{
			a_in_b = true;
		}
		else
		{
			// Inconsistent or coincident.  No good.
			return false;
		}
	}

	if (a_in_b)
	{
		assert(a_in_b);

		bool	v_in_a =
			(vertex_left_test(cone_a[0], cone_a[1], sorted_verts[v].m_v) > 0)
			&& (vertex_left_test(cone_a[1], cone_a[2], sorted_verts[v].m_v) > 0);
		if (v_in_a)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		bool	v_in_b =
			(vertex_left_test(cone_b[0], cone_b[1], sorted_verts[v].m_v) > 0)
			&& (vertex_left_test(cone_b[1], cone_b[2], sorted_verts[v].m_v) > 0);
		if (v_in_b)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}


template<class coord_t>
bool	poly<coord_t>::any_edge_intersection(const array<vert_t>& sorted_verts, int external_vert, int my_vert)
// Return true if edge (external_vert,my_vert) intersects any edge in our poly.
{
	// Check the edge index for potentially overlapping edges.

	const vert_t*	pmv = &sorted_verts[my_vert];
	const vert_t*	pev = &sorted_verts[external_vert];

	assert(m_edge_index);

 	index_box<coord_t>	query_box(pmv->get_index_point());
	query_box.expand_to_enclose(pev->get_index_point());

	for (ib_iterator it = m_edge_index->begin(query_box);
	     ! it.at_end();
	     ++it)
	{
		int	vi = it->value;
		int	v_next = sorted_verts[vi].m_next;

		if (vi != my_vert)
		{
			if (sorted_verts[vi].m_v == sorted_verts[my_vert].m_v)
			{
				// Coincident verts.
				if (vert_can_see_cone_a(sorted_verts, external_vert, my_vert, vi) == false)
				{
					// Logical edge crossing.
					return true;
				}
			}
			else if (edges_intersect(sorted_verts, vi, v_next, external_vert, my_vert))
			{
				return true;
			}
		}
	}

	return false;
}


template<class coord_t>
bool	poly<coord_t>::ear_contains_reflex_vertex(const array<vert_t>& sorted_verts, int v0, int v1, int v2)
// Return true if any of this poly's reflex verts are inside the
// specified ear.  The definition of inside is: a reflex vertex in the
// interior of the triangle (v0,v1,v2), or on the segments [v1,v0) or
// [v1,v2).
{
	// Compute the bounding box of reflex verts we want to check.
	index_box<coord_t>	query_bound(sorted_verts[v0].get_index_point());
	query_bound.expand_to_enclose(index_point<coord_t>(sorted_verts[v1].m_v.x, sorted_verts[v1].m_v.y));
	query_bound.expand_to_enclose(index_point<coord_t>(sorted_verts[v2].m_v.x, sorted_verts[v2].m_v.y));

	for (ip_iterator it = m_reflex_point_index->begin(query_bound);
	     ! it.at_end();
	     ++it)
	{
		int	vk = it->value;

		const vert_t*	pvk = &sorted_verts[vk];
		if (pvk->m_poly_owner != this)
		{
			// Not part of this poly; ignore it.
			continue;
		}

		if (vk != v0 && vk != v1 && vk != v2
		    && query_bound.contains_point(index_point<coord_t>(pvk->m_v.x, pvk->m_v.y)))
		{
#if 0
			//xxxxxx debug hook
			if (v1 == 131 && vk == 132)
			{
				vk = vk;//xxxxx breakpoint here
			}
#endif

			int	v_next = pvk->m_next;
			int	v_prev = pvk->m_prev;

			if (pvk->m_v == sorted_verts[v1].m_v)
			{
				// Tricky case.  See section 4.3 in FIST paper.

				// Note: I'm explicitly considering convex vk in here, unlike FIST.
				// This is to handle the triple dupe case, where a loop validly comes
				// straight through our cone.
				//
				// Note: the triple-dupe case is technically not a valid poly, since
				// it contains a twist.
				//
				// @@ Fix this back to the FIST way?

				int	v_prev_left01 = vertex_left_test(
					sorted_verts[v0].m_v,
					sorted_verts[v1].m_v,
					sorted_verts[v_prev].m_v);
				int	v_next_left01 = vertex_left_test(
					sorted_verts[v0].m_v,
					sorted_verts[v1].m_v,
					sorted_verts[v_next].m_v);
				int	v_prev_left12 = vertex_left_test(
					sorted_verts[v1].m_v,
					sorted_verts[v2].m_v,
					sorted_verts[v_prev].m_v);
				int	v_next_left12 = vertex_left_test(
					sorted_verts[v1].m_v,
					sorted_verts[v2].m_v,
					sorted_verts[v_next].m_v);

				if ((v_prev_left01 > 0 && v_prev_left12 > 0)
				    || (v_next_left01 > 0 && v_next_left12 > 0))
				{
					// Local interior near vk intersects this
					// ear; ear is clearly not valid.
					return true;
				}

				// Check colinear case, where cones of vk and v1 overlap exactly.
				if ((v_prev_left01 == 0 && v_next_left12 == 0)
				    || (v_prev_left12 == 0 && v_next_left01 == 0))
				{
					// @@ TODO: there's a somewhat complex non-local area test that tells us
					// whether vk qualifies as a contained reflex vert.
					//
					// For the moment, deny the ear.
					//
					// The question is, is this test required for correctness?  Because it
					// seems pretty expensive to compute.  If it's not required, I think it's
					// better to always assume the ear is invalidated.

					//xxx
					//fprintf(stderr, "colinear case in ear_contains_reflex_vertex; returning true\n");

					return true;
				}
			}
			else
			{
				assert(pvk->m_convex_result < 0);
				if (vertex_in_ear(
					    pvk->m_v, sorted_verts[v0].m_v, sorted_verts[v1].m_v, sorted_verts[v2].m_v))
				{
					// Found one.
					return true;
				}
			}
		}
	}

	// Didn't find any qualifying verts.
	return false;
}


template<class coord_t>
bool	poly<coord_t>::vert_in_cone(const array<vert_t>& sorted_verts, int vert, int cone_v0, int cone_v1, int cone_v2)
// Returns true if vert is within the cone defined by [v0,v1,v2].
/*
//  (out)  v0
//        /
//    v1 <   (in)
//        \
//         v2
*/
{
	bool	acute_cone = vertex_left_test(sorted_verts[cone_v0].m_v, sorted_verts[cone_v1].m_v, sorted_verts[cone_v2].m_v) > 0;

	// Include boundary in our tests.
	bool	left_of_01 =
		vertex_left_test(sorted_verts[cone_v0].m_v, sorted_verts[cone_v1].m_v, sorted_verts[vert].m_v) >= 0;
	bool	left_of_12 =
		vertex_left_test(sorted_verts[cone_v1].m_v, sorted_verts[cone_v2].m_v, sorted_verts[vert].m_v) >= 0;

	if (acute_cone)
	{
		// Acute cone.  Cone is intersection of half-planes.
		return left_of_01 && left_of_12;
	}
	else
	{
		// Obtuse cone.  Cone is union of half-planes.
		return left_of_01 || left_of_12;
	}
}


template<class coord_t>
bool	poly<coord_t>::vert_is_duplicated(const array<vert_t>& sorted_verts, int vert)
// Return true if there's another vertex in this poly, coincident with vert.
{
	// Scan backwards.
	{for (int vi = vert - 1; vi >= 0; vi--)
	{
		if ((sorted_verts[vi].m_v == sorted_verts[vert].m_v) == false)
		{
			// No more coincident verts scanning backward.
			break;
		}
		if (sorted_verts[vi].m_poly_owner == this)
		{
			// Found a dupe vert.
			return true;
		}
	}}

	// Scan forwards.
	{for (int vi = vert + 1, n = sorted_verts.size(); vi < n; vi++)
	{
		if ((sorted_verts[vi].m_v == sorted_verts[vert].m_v) == false)
		{
			// No more coincident verts scanning forward.
			break;
		}
		if (sorted_verts[vi].m_poly_owner == this)
		{
			// Found a dupe vert.
			return true;
		}
	}}

	// Didn't find a dupe.
	return false;
}


//
// poly_env
//


template<class coord_t>
struct poly_env
// Struct that holds the state of a triangulation.
{
//data:
	array<poly_vert<coord_t> >	m_sorted_verts;
	array<poly<coord_t>*>	m_polys;

	index_box<coord_t>	m_bound;
	int	m_estimated_triangle_count;

//code:
	void	init(int path_count, const array<coord_t> paths[]);
	void	join_paths_into_one_poly();

	int	get_estimated_triangle_count() const { return m_estimated_triangle_count; }

	poly_env()
		:
		m_bound(index_point<coord_t>(0, 0), index_point<coord_t>(0, 0)),
		m_estimated_triangle_count(0)
	{
	}

	~poly_env()
	{
		// delete the polys that got new'd during init()
		for (int i = 0, n = m_polys.size(); i < n; i++)
		{
			delete m_polys[i];
		}
	}

private:
	// Internal helpers.
	void	join_paths_with_bridge(
		poly<coord_t>* main_poly,
		poly<coord_t>* sub_poly,
		int vert_on_main_poly,
		int vert_on_sub_poly);
	void	dupe_two_verts(int v0, int v1);
};


template<class coord_t>
void	poly_env<coord_t>::init(int path_count, const array<coord_t> paths[])
// Initialize our state, from the given set of paths.  Sort vertices
// and component polys.
{
	// Only call this on a fresh poly_env
	assert(m_sorted_verts.size() == 0);
	assert(m_polys.size() == 0);

	// Count total verts.
	int	vert_count = 0;
	for (int i = 0; i < path_count; i++)
	{
		vert_count += paths[i].size();
	}

	// Slight over-estimate; the true number depends on how many
	// of the paths are actually islands.
	m_estimated_triangle_count = vert_count /* - 2 * path_count */;

	// Collect the input verts and create polys for the input paths.
	m_sorted_verts.reserve(vert_count + (path_count - 1) * 2);	// verts, plus two duped verts for each path, for bridges
	m_polys.reserve(path_count);

	for (int i = 0; i < path_count; i++)
	{
		// Create a poly for this path.
		const array<coord_t>&	path = paths[i];

		if (path.size() < 3)
		{
			// Degenerate path, ignore it.
			continue;
		}

		poly<coord_t>*	p = new poly<coord_t>;
		m_polys.push_back(p);

		// Add this path's verts to our list.
		int	path_size = path.size();
		if (path.size() & 1)
		{
			// Bad input, odd number of coords.
			assert(0);
			fprintf(stderr, "path[%d] has odd number of coords (%d), dropping last value\n", i, path.size());//xxxx
			path_size--;
		}
		for (int j = 0; j < path_size; j += 2)	// vertex coords come in pairs.
		{
			int	prev_point = j - 2;
			if (j == 0) prev_point = path_size - 2;

			if (path[j] == path[prev_point] && path[j + 1] == path[prev_point + 1])
			{
				// Duplicate point; drop it.
				continue;
			}

			// Insert point.
			int	vert_index = m_sorted_verts.size();

			poly_vert<coord_t>	vert(path[j], path[j+1], p, vert_index);
			m_sorted_verts.push_back(vert);

			p->append_vert(&m_sorted_verts, vert_index);

			index_point<coord_t>	ip(vert.m_v.x, vert.m_v.y);
			if (vert_index == 0)
			{
				// Initialize the bounding box.
				m_bound.min = ip;
				m_bound.max = ip;
			}
			else
			{
				// Expand the bounding box.
				m_bound.expand_to_enclose(ip);
			}
			assert(m_bound.contains_point(ip));
		}
		assert(p->is_valid(m_sorted_verts));

		if (p->m_vertex_count == 0)
		{
			// This path was degenerate; kill it.
			delete p;
			m_polys.pop_back();
		}
	}

	// Sort the vertices.
	qsort(&m_sorted_verts[0], m_sorted_verts.size(), sizeof(m_sorted_verts[0]), compare_vertices<coord_t>);
	assert(m_sorted_verts.size() <= 1
	       || compare_vertices<coord_t>((void*) &m_sorted_verts[0], (void*) &m_sorted_verts[1]) <= 0);	// check order

	// Remap the vertex indices, so that the polys and the
	// sorted_verts have the correct, sorted, indices.  We can
	// then use vert indices to judge the left/right relationship
	// of two verts.
	array<int>	vert_remap;	// vert_remap[i] == new index of original vert[i]
	vert_remap.resize(m_sorted_verts.size());
	for (int i = 0, n = m_sorted_verts.size(); i < n; i++)
	{
		int	new_index = i;
		int	original_index = m_sorted_verts[new_index].m_my_index;
		vert_remap[original_index] = new_index;
	}
	{for (int i = 0, n = m_sorted_verts.size(); i < n; i++)
	{
		m_sorted_verts[i].remap(vert_remap);
	}}
	{for (int i = 0, n = m_polys.size(); i < n; i++)
	{
		m_polys[i]->remap(vert_remap);
		assert(m_polys[i]->is_valid(m_sorted_verts));
	}}
}


template<class coord_t>
void	poly_env<coord_t>::join_paths_into_one_poly()
// Use zero-area bridges to connect separate polys & islands into one
// big continuous poly.
{
	// Connect separate paths with bridge edges, into one big path.
	//
	// Bridges are zero-area regions that connect a vert on each
	// of two paths.
	if (m_polys.size() > 1)
	{
		// Sort polys in order of each poly's leftmost vert.
		qsort(&m_polys[0], m_polys.size(), sizeof(m_polys[0]), compare_polys_by_leftmost_vert<coord_t>);
		assert(m_polys.size() <= 1
		       || compare_polys_by_leftmost_vert<coord_t>((void*) &m_polys[0], (void*) &m_polys[1]) == -1);

		// assume that the enclosing boundary is the leftmost
		// path; this is true if the regions are valid and
		// don't intersect.
		poly<coord_t>*	full_poly = m_polys[0];

		full_poly->init_edge_index(m_sorted_verts, m_bound);

		// Iterate from left to right
		while (m_polys.size() > 1)
		{
			int	v1 = m_polys[1]->m_leftmost_vert;

			//     find v2 in full_poly, such that:
			//       v2 is to the left of v1,
			//       and v1-v2 seg doesn't intersect any other edges

			//     // (note that since v1 is next-most-leftmost, v1-v2 can't
			//     // hit anything in p, or any paths further down the list,
			//     // it can only hit edges in full_poly) (need to think
			//     // about equality cases)
			//
			int	v2 = full_poly->find_valid_bridge_vert(m_sorted_verts, v1);

			//     once we've found v1 & v2, we use it to make a bridge,
			//     inserting p into full_poly
			//
			assert(m_sorted_verts[v2].m_poly_owner == m_polys[0]);
			assert(m_sorted_verts[v1].m_poly_owner == m_polys[1]);
			join_paths_with_bridge(full_poly, m_polys[1], v2, v1);

			// Drop the joined poly.
			delete m_polys[1];
			m_polys.remove(1);
		}
	}

	m_polys[0]->init_for_ear_clipping(&m_sorted_verts);

	assert(m_polys.size() == 1);
	// assert(all verts in m_sorted_verts have m_polys[0] as their owner);
}


template<class coord_t>
void	poly_env<coord_t>::join_paths_with_bridge(
	poly<coord_t>* main_poly,
	poly<coord_t>* sub_poly,
	int vert_on_main_poly,
	int vert_on_sub_poly)
// Absorb the sub-poly into the main poly, using a zero-area bridge
// between the two given verts.
{
	// TODO: tricky case here, needs to be handled.  What if
	// vert_on_main_poly already has multiple entry/exit edges?
	// We need to be careful to insert the bridge entry/exit in
	// the correct loop(s).
	
	assert(vert_on_main_poly != vert_on_sub_poly);
	assert(main_poly != NULL);
	assert(sub_poly != NULL);
	assert(main_poly != sub_poly);
	assert(main_poly == m_sorted_verts[vert_on_main_poly].m_poly_owner);
	assert(sub_poly == m_sorted_verts[vert_on_sub_poly].m_poly_owner);

	if (m_sorted_verts[vert_on_main_poly].m_v == m_sorted_verts[vert_on_sub_poly].m_v)
	{
		// Special case: verts to join are coincident.  We
		// don't actually need to make a bridge with new
		// verts; we only need to adjust the links and do
		// fixup.
		poly_vert<coord_t>*	pv_main = &m_sorted_verts[vert_on_main_poly];
		poly_vert<coord_t>*	pv_sub = &m_sorted_verts[vert_on_sub_poly];

		int	main_next = pv_main->m_next;

		// Remove the edge we're about to break.
		main_poly->remove_edge(m_sorted_verts, vert_on_main_poly);

		pv_main->m_next = pv_sub->m_next;
		m_sorted_verts[pv_main->m_next].m_prev = vert_on_main_poly;

		pv_sub->m_next = main_next;
		m_sorted_verts[main_next].m_prev = vert_on_sub_poly;

		// Add edge that connects to sub poly.
		main_poly->add_edge(m_sorted_verts, vert_on_main_poly);

		// Fixup sub poly so it's now properly a part of the main poly.
		main_poly->update_connected_sub_poly(&m_sorted_verts, pv_main->m_next, main_next);
		sub_poly->invalidate(m_sorted_verts);

		return;
	}

	// Normal case, need to dupe verts and create zero-area bridge.
	dupe_two_verts(vert_on_main_poly, vert_on_sub_poly);

	// Fixup the old indices to account for the new dupes.
	if (vert_on_sub_poly < vert_on_main_poly)
	{
		vert_on_main_poly++;
	}
	else
	{
		vert_on_sub_poly++;
	}

	poly_vert<coord_t>*	pv_main = &m_sorted_verts[vert_on_main_poly];
	poly_vert<coord_t>*	pv_sub = &m_sorted_verts[vert_on_sub_poly];
	poly_vert<coord_t>*	pv_main2 = &m_sorted_verts[vert_on_main_poly + 1];
	poly_vert<coord_t>*	pv_sub2 = &m_sorted_verts[vert_on_sub_poly + 1];

	// Remove the edge we're about to break.
	main_poly->remove_edge(m_sorted_verts, vert_on_main_poly);

	// Link the loops together.
	pv_main2->m_next = pv_main->m_next;
	pv_main2->m_prev = vert_on_sub_poly + 1;	// (pv_sub2)
	m_sorted_verts[pv_main2->m_next].m_prev = pv_main2->m_my_index;

	pv_sub2->m_prev = pv_sub->m_prev;
	pv_sub2->m_next = vert_on_main_poly + 1;	// (pv_main2)
	m_sorted_verts[pv_sub2->m_prev].m_next = pv_sub2->m_my_index;

	pv_main->m_next = vert_on_sub_poly;		// (pv_sub)
	pv_sub->m_prev = vert_on_main_poly;		// (pv_main)

	// Add edge that connects to sub poly.
	main_poly->add_edge(m_sorted_verts, vert_on_main_poly);

	// Fixup sub poly so it's now properly a part of the main poly.
	main_poly->update_connected_sub_poly(&m_sorted_verts, vert_on_sub_poly, pv_main2->m_next);
	sub_poly->invalidate(m_sorted_verts);

	assert(pv_main->m_poly_owner->is_valid(m_sorted_verts));
}


template<class coord_t>
void	poly_env<coord_t>::dupe_two_verts(int v0, int v1)
// Duplicate the two indexed verts, remapping polys & verts as necessary.
{
	// Order the verts.
	if (v0 > v1)
	{
		tu_swap(&v0, &v1);
	}
	assert(v0 < v1);

	// Duplicate verts.
	poly_vert<coord_t>	v0_copy = m_sorted_verts[v0];
	poly_vert<coord_t>	v1_copy = m_sorted_verts[v1];

	// @@ This stuff can be costly!  E.g. lots of separate little
	// polys that need bridges, with a high total vert count.

	// Slower, clearer code to insert the two new verts.  This
	// ends up moving the data between ((v1+1), end) twice.
	if (0) {
		// Insert v1 first, so v0 doesn't get moved.
		m_sorted_verts.insert(v1 + 1, v1_copy);
		m_sorted_verts.insert(v0 + 1, v0_copy);
	}
	else
	// Faster, more obfuscated code to insert the two new verts.
	//
	// NOTE: I tried doing this in one pass, with a complicated
	// explicit move & remap in the same pass.  It was much
	// slower!  (VC7, Win2K.)  memmove() must be magical?
	{
		// Make room.
		m_sorted_verts.resize(m_sorted_verts.size() + 2);

		// Move the two subsegments.
		memmove(&m_sorted_verts[v1 + 3], &m_sorted_verts[v1 + 1], sizeof(m_sorted_verts[0]) * (m_sorted_verts.size() - 3 - v1));
		memmove(&m_sorted_verts[v0 + 2], &m_sorted_verts[v0 + 1], sizeof(m_sorted_verts[0]) * (v1 - v0));

		// Insert the new duplicate verts.
		m_sorted_verts[v0 + 1] = v0_copy;
		m_sorted_verts[v1 + 2] = v1_copy;
	}

	// Remap the indices within the verts.
	for (int i = 0, n = m_sorted_verts.size(); i < n; i++)
	{
		m_sorted_verts[i].m_my_index = i;
		m_sorted_verts[i].m_next = remap_index_for_duped_verts(m_sorted_verts[i].m_next, v0, v1);
		m_sorted_verts[i].m_prev = remap_index_for_duped_verts(m_sorted_verts[i].m_prev, v0, v1);
	}

	// Remap the polys.
	{for (int i = 0, n = m_polys.size(); i < n; i++)
	{
		m_polys[i]->remap_for_duped_verts(m_sorted_verts, v0, v1);

		assert(m_polys[i]->is_valid(m_sorted_verts));
	}}
}


//
// Helpers.
//


template<class coord_t>
static void	recovery_process(
	array<poly<coord_t>*>* polys,	// polys waiting to be processed
	poly<coord_t>* P,	// current poly
	array<poly_vert<coord_t> >* sorted_verts,
	tu_random::generator* rg);


template<class coord_t>
inline void	debug_emit_poly_loop(
	array<coord_t>* result,
	const array<poly_vert<coord_t> >& sorted_verts,
	poly<coord_t>* P)
// Fill *result with a set of edges representing P.
{
	result->resize(0);	// clear existing junk.

	int	first_vert = P->m_loop;
	int	vi = first_vert;
	do
	{
		result->push_back(sorted_verts[vi].m_v.x);
		result->push_back(sorted_verts[vi].m_v.y);
		vi = sorted_verts[vi].m_next;
		result->push_back(sorted_verts[vi].m_v.x);
		result->push_back(sorted_verts[vi].m_v.y);
	}
	while (vi != first_vert);

	// Loop back to beginning, and pad to a multiple of 3 coords.
	do
	{
		result->push_back(sorted_verts[vi].m_v.x);
		result->push_back(sorted_verts[vi].m_v.y);
	}
	while (result->size() % 6);
}


template<class coord_t>
static void compute_triangulation(
	array<coord_t>* result,
	int path_count,
	const array<coord_t> paths[],
	int debug_halt_step,
	array<coord_t>* debug_edges)
// Compute triangulation.
//
// The debug_ args are optional; they're for terminating early and
// returning the remaining loop to be triangulated.
{
	if (path_count <= 0)
	{
		// Empty paths --> no triangles to emit.
		return;
	}

#ifdef PROFILE_TRIANGULATE
	uint64	start_ticks = tu_timer::get_profile_ticks();
#endif // PROFILE_TRIANGULATE

	// Local generator, for some parts of the algo that need random numbers.
	tu_random::generator	rand_gen;

	// Poly environment; most of the state of the algo.
	poly_env<coord_t>	penv;

	penv.init(path_count, paths);

	penv.join_paths_into_one_poly();

	result->reserve(2 * 3 * penv.get_estimated_triangle_count());

	int	input_vert_count = 0;
	if (penv.m_polys.size() > 0)
	{
		input_vert_count = penv.m_polys[0]->get_vertex_count();
	}

#ifdef PROFILE_TRIANGULATE
	uint64	join_ticks = tu_timer::get_profile_ticks();
	fprintf(stderr, "join poly = %1.6f sec\n", tu_timer::profile_ticks_to_seconds(join_ticks - start_ticks));
#endif // PROFILE_TRIANGULATE

// Debugging only: dump coords of joined poly.
//#define DUMP_JOINED_POLY
#ifdef DUMP_JOINED_POLY
	{
		int	first_vert = penv.m_polys[0]->m_loop;
		int	vi = first_vert;
		do
		{
			printf("%f, %f\n", penv.m_sorted_verts[vi].m_v.x, penv.m_sorted_verts[vi].m_v.y);
			vi = penv.m_sorted_verts[vi].m_next;
		}
		while (vi != first_vert);
	}
#endif

// Debugging only: just emit our joined poly, without triangulating.
//#define EMIT_JOINED_POLY
#ifdef EMIT_JOINED_POLY
	{
		int	first_vert = penv.m_polys[0]->m_loop;
		int	vi = first_vert;
		do
		{
			result->push_back(penv.m_sorted_verts[vi].m_v.x);
			result->push_back(penv.m_sorted_verts[vi].m_v.y);
			vi = penv.m_sorted_verts[vi].m_next;
		}
		while (vi != first_vert);

		// Loop back to beginning, and pad to a multiple of 3 coords.
		do
		{
			result->push_back(penv.m_sorted_verts[vi].m_v.x);
			result->push_back(penv.m_sorted_verts[vi].m_v.y);
		}
		while (result->size() % 6);
	}

	return;
#endif // EMIT_JOINED_POLY

	// ear-clip, adapted from FIST paper:
	//
	//   list<poly> L;
	//   L.insert(full_poly)
	//   while L not empty:
	//     P = L.pop()
	//     Q = priority queue of ears of P
	//     while P.vert_count > 3 do:
	//       if Q not empty:
	//         e = Q.pop
	//         emit e
	//         update P by deleting e
	//       else if an ear was clipped in previous pass then:
	//         Q = priority queue of ears of P (i.e. reexamine P)
	//       else
	//         // we're stuck
	//         recovery_process()	// do something drastic to make the next move
	//     emit last 3 verts of P as the final triangle

	while (penv.m_polys.size())
	{
		poly<coord_t>*	P = penv.m_polys.back();
		penv.m_polys.pop_back();

		P->build_ear_list(&penv.m_sorted_verts, &rand_gen);

		bool	ear_was_clipped = false;
		while (P->get_vertex_count() > 3)
		{
			if (P->get_ear_count() > 0)
			{
				// Clip the next ear from Q.
				int	v1 = P->get_next_ear(penv.m_sorted_verts, &rand_gen);
				int	v0 = penv.m_sorted_verts[v1].m_prev;
				int	v2 = penv.m_sorted_verts[v1].m_next;

				P->emit_and_remove_ear(result, &penv.m_sorted_verts, v0, v1, v2);
				ear_was_clipped = true;

				// For debugging -- terminate early if the debug counter hits zero.
				debug_halt_step--;
				if (debug_halt_step == 0) {
					if (debug_edges) {
						debug_emit_poly_loop(debug_edges, penv.m_sorted_verts, P);
					}
					return;
				}
			}
			else if (ear_was_clipped == true)
			{
				// Re-examine P for new ears.
				ear_was_clipped = P->build_ear_list(&penv.m_sorted_verts, &rand_gen);
			}
			else
			{
				// No valid ears; we're in trouble so try some fallbacks.

#if 1
				// xxx hack for debugging: show the state of P when we hit the recovery process.
				debug_emit_poly_loop(result, penv.m_sorted_verts, P);
				return;
#endif

				//recovery_process(&penv.m_polys, P, &penv.m_sorted_verts, &rand_gen);
				//ear_was_clipped = false;
			}
		}

		if (P->get_vertex_count() == 3)
		{
			// Emit the final triangle.
			if (penv.m_sorted_verts[P->m_loop].m_is_ear == false)
			{
				// Force an arbitrary vert to be an ear.
				penv.m_sorted_verts[P->m_loop].m_is_ear = true;
				P->m_ear_count++;
			}
			P->emit_and_remove_ear(
				result,
				&penv.m_sorted_verts,
				penv.m_sorted_verts[P->m_loop].m_prev,
				P->m_loop,
				penv.m_sorted_verts[P->m_loop].m_next);
		}
		delete P;
	}
	
#ifdef PROFILE_TRIANGULATE
	uint64	clip_ticks = tu_timer::get_profile_ticks();
	fprintf(stderr, "clip poly = %1.6f sec\n", tu_timer::profile_ticks_to_seconds(clip_ticks - join_ticks));
	fprintf(stderr, "total for poly = %1.6f sec\n", tu_timer::profile_ticks_to_seconds(clip_ticks - start_ticks));
	fprintf(stderr, "vert count = %d, verts clipped / sec = %f\n",
		input_vert_count,
		input_vert_count / tu_timer::profile_ticks_to_seconds(clip_ticks - join_ticks));
#endif // PROFILE_TRIANGULATE

	assert(penv.m_polys.size() == 0);
	// assert(for all penv.m_sorted_verts: owning poly == NULL);

	assert((result->size() % 6) == 0);
}


template<class coord_t>
void	recovery_process(
	array<poly<coord_t>*>* polys,
	poly<coord_t>* P,
	array<poly_vert<coord_t> >* sorted_verts,
	tu_random::generator* rg)
{
	// recovery_process:
	//   if two edges in P, e[i-1] and e[i+1] intersect:
	//     insert two tris incident on e[i-1] & e[i+1] as ears into Q
	//   else if P can be split with a valid diagonal:
	//     P = one side
	//     L += the other side
	//     Q = ears of P
	//   else if P has any convex vertex:
	//     pick a random convex vert and add it to Q
	//   else
	//     pick a random vert and add it to Q

	// Case 1: two edges, e[i-1] and e[i+1], intersect; we insert
	// the overlapping ears into Q and resume.
	{for (int vi = (*sorted_verts)[P->m_loop].m_next; vi != P->m_loop; vi = (*sorted_verts)[vi].m_next)
	{
		int	ev0 = vi;
		int	ev1 = (*sorted_verts)[ev0].m_next;
		int	ev2 = (*sorted_verts)[ev1].m_next;
		int	ev3 = (*sorted_verts)[ev2].m_next;
		if (edges_intersect(*sorted_verts, ev0, ev1, ev2, ev3))
		{
			// Insert (1,2,3) as an ear.
			(*sorted_verts)[ev2].m_is_ear = true;
			P->m_ear_count++;

			fprintf(stderr, "recovery_process: self-intersecting sequence, treating %d as an ear\n", ev2);//xxxx

			// Resume regular processing.
			return;
		}
	}}

// Deviation from FIST: Because I'm lazy, I'm skipping this test for
// now...
//
// @@ This seems to be helpful for doing reasonable things in case the
// input is a little bit self-intersecting.  Otherwise, clipping any
// old convex or random vert can create crazy junk in the
// triangulation.  It's probably worth implementing at some point.
#if 0
	// Case 2: P can be split with a valid diagonal.
	//
	// A "valid diagonal" passes these checks, according to FIST:
	//
	// 1. diagonal is locally within poly
	//
	// 2. its relative interior does not intersect any edge of the poly
	//
	// 3. the winding number of the polygon w/r/t the midpoint of
	// the diagonal is one
	//
	{for (int vi = verts[0, end])
	{
		for (int vj = verts[vi->m_next, end])
		{
			if (P->valid_diagonal(vi, vj))
			{
				// Split P, insert leftover piece into polys
				poly*	leftover = P->split(vi, vj);
				polys->push_back(leftover);

				// Resume regular processing.
				return;
			}
		}
	}}
#endif // 0

	// Case 3: P has any convex vert
	int	first_vert = P->m_loop;
	int	vi = first_vert;
	int	vert_count = 0;
	do
	{
		if (is_convex_vert(*sorted_verts, vi))
		{
			// vi is convex; treat it as an ear,
			// regardless of other problems it may have.
			(*sorted_verts)[vi].m_is_ear = true;
			P->m_ear_count++;

			fprintf(stderr, "recovery_process: found convex vert, treating %d as an ear\n", vi);//xxxx

			// Resume regular processing.
			return;
		}

		vert_count++;
		vi = (*sorted_verts)[vi].m_next;
	}
	while (vi != first_vert);

	// Case 4: Pick a random vert and treat it as an ear.
	int	random_vert = rg->next_random() % vert_count;
	for (vi = first_vert; random_vert > 0; random_vert--)
	{
		vi = (*sorted_verts)[vi].m_next;
	}
	(*sorted_verts)[vi].m_is_ear = true;
	P->m_ear_count++;

	fprintf(stderr, "recovery_process: treating random vert %d as an ear\n", vi);//xxxx

	// Resume.
	return;
}


// Local Variables:
// mode: C++
// c-basic-offset: 8
// tab-width: 8
// indent-tabs-mode: t
// End:
