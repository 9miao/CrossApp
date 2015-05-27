// gameswf_tesselate.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// 2D shape triangulation.  Takes line/curve segments with a style
// index, and outputs trapezoids (old code) or triangles (new code).
//
// Comments on tesselation strategies:
//
// New method (namespace tesselate_new) seems to be pretty reliable,
// and (in theory) produces much better triangulations much more
// quickly.  It's based on ear-clipping, similar to FIST, but
// simplified and modified.  See base/constrianed_triangulate.cpp.
// However, the current integration with gameswf is very hasty and
// does a ton of unnecessary array copying on the input side, as well
// as wasteful tristripping on the output side.  TODO this can all be
// gotten rid of without much difficulty, and then it should fly.
//
// Original method (namespace tesselate) basically works but has some
// problems.  It's slow, it generates lots of triangles where the poly
// edges are curvy, and occasionally it makes a leak around very thin
// shapes.
//
// Shewchuk's constrained delaunay triangulator, "Triangle": not
// designed for rendering; it's big thing is making roundish
// triangles, at the expense of triangle count.  That's the right
// tradeoff for FEA (Shewchuk's main target application), but the
// wrong tradeoff for rendering; we don't have any particular problem
// with skinny triangles.
//
// Actually Shewchuk's code seems to be fast when it's not doing fancy
// extra stuff.  Unfortunately it's not Public Domain-able, but the
// divide-and-conquer algo looks decent.  I don't like how it has to
// break and re-make a bunch of edges when constraining the results,
// but otherwise it has a lot of good properties.
//
// Something like "A Fast Trapezoidation Technique For Planar
// Polygons".  The key improvement there is to only cut from a
// vertex/event to the nearest adjacent edges, instead of slicing
// across the entire set of active edges.  This sounds good w/r/t to
// my crappy old code, but doesn't seem relevant to the better algos.
//
// Or, "FIST: Fast Industrial-Strength Triangulation of Polygons" by
// Martin Held.  This looks good; application is rasterization.  The
// funny thing here is that they explicitly deal with holes by
// inserting zero-area bridge edges to connect the holes with the
// outer boundary, to make one continuous poly.  This seems a little
// hacky and possibly tricky.  [See partial implementation in
// base/triangulate_impl.h -- I never got around to finishing all the
// tricky bridge-building stuff though.]
//
// Variations on line-sweep to find monotone chains, followed by
// triangulating the monotones:
//
// * http://www.mema.ucl.ac.be/~wu/Poly2Tri/poly2tri.html -- looks
// good, source is available, but the license is non-commercial only.
//
// * Narkhede & Manocha's implementation of Seidel's algo; not really
// production quality, non-commercial UNC license.
//
// * Eric Veach's GLU tesselator:
// http://webcvs.freedesktop.org/mesa/Mesa-oldtree/si-glu/libtess/
// This is under a liberal SGI license, and is reportedly very robust.
// If I had known about this code before I delved into the problem, I
// would probably have just adapted it.
//
// * cairographics.org: they do some line-sweep thing.  LGPL though.
// It used to be under the X license I think?  Maybe there's an old
// X-licensed version kicking around.  ... Browsing
// gitweb.freedesktop.org in file cairo-traps.c, it seems that
// tesselator closely resembles my old trapezoider (though theirs
// seems to be implemented much better & more carefully).  So it
// probably produces too many triangles.
//
// Optimize the old tesselator?  I took a quick look at the cairo
// project's trapezoider -- it's very similar to ours but seems to do
// its work in-place without resizing the edge list constantly, so it
// should be pretty quick.  They do some high-precision integer voodoo
// in their intercept calculation that I don't 100% understand.  See
// gitweb.freedesktop.org, in file cairo-traps.c .  It seems this will
// produce meshes with too many verts, much like the old gameswf
// tesselator.
//
// Findings re SWF polygons:
//
// (Based on experiments with Flash MX 2004; old movies seem to obey
// these rules as well, as far as I can tell):
//
// * no intersecting edges -- if you make intersections in the tool
// (even by stretching curve segments around), the tool inserts verts
// automatically.
//
// * left-fill and right-fill seem to be randomly selected.
//
// * individual paths (i.e. segment paths with a consistent set of
// fill styles) are not closed
//
// * there's no odd-even insanity; the tool cleans up the poly if you
// try to make self-intersections and flips and such.
//
// * if you duplicate the paths with two fill styles (so every path
// has just one fill style), and reverse the left-fill paths so they
// make right-fill paths, and then join open verts of the same fill
// style, you will be left with a set of non-intersecting closed polys
// with a consistent winding.  I.e. fairly well-behaved.  They may
// have holes though.
//
// Current outlook: new triangulator looks good so far.  Need to clean
// up & optimize the interface a bit, and look for pathological cases
// where it does something wrong.  The new triangulator needs more
// attention to cases of bad input (intersecting edges and so on) --
// should probably do FIST-like fallback techniques to limit the
// damage.


#include "gameswf_tesselate.h"
#include "gameswf_types.h"
#include "../base/utility.h"
#include "../base/container.h"
#include <stdlib.h>
#include "../base/ear_clip_triangulate.h"


// Useful for debugging.  TODO: make a cleaner interface to this.
// bool gameswf_tesselate_dump_shape = false;//xxxxxxx

namespace gameswf
{
namespace tesselate
{
	// Curve subdivision error tolerance.
	static float	s_tolerance = 1.0f;
	static trapezoid_accepter*	s_accepter = NULL;

	struct fill_segment
	{
		point	m_begin;
		point	m_end;
		int	m_left_style, m_right_style, m_line_style;

		fill_segment() {}

		fill_segment(
			const point& a,
			const point& b,
			int left_style,
			int right_style,
			int line_style)
			:
			m_begin(a),
			m_end(b),
			m_left_style(left_style),
			m_right_style(right_style),
			m_line_style(line_style)
		{
			// For rasterization, we want to ensure that
			// the segment always points towards positive
			// y...
			if (m_begin.m_y > m_end.m_y)
			{
				flip();
			}
		}

		void	flip()
		// Exchange end points, and reverse fill sides.
		{
			tu_swap(&m_begin, &m_end);

			// swap fill styles...
			tu_swap(&m_left_style, &m_right_style);
		}

		float	get_height() const
		// Return segment height.
		{
			assert(m_end.m_y >= m_begin.m_y);

			return m_end.m_y - m_begin.m_y;
		}
	};


	// More Renderer state.
	static array<fill_segment>	s_current_segments;	// @@ should not dynamically resize this thing!
	static array<point>	s_current_path;			// @@ should not dynamically resize this thing!
	static point	s_last_point;
	static int	s_current_left_style;
	static int	s_current_right_style;
	static int	s_current_line_style;
	static bool	s_shape_has_line;	// flag to let us skip the line rendering if no line styles were set when defining the shape.
	static bool	s_shape_has_fill;	// flag to let us skip the fill rendering if no fill styles were set when defining the shape.


	static void	peel_off_and_emit(int i0, int i1, float y0, float y1);


	void	begin_shape(trapezoid_accepter* accepter, float curve_error_tolerance)
	{
		assert(accepter);
		s_accepter = accepter;

		// ensure we're not already in a shape or path.
		// make sure our shape state is cleared out.
		assert(s_current_segments.size() == 0);
		s_current_segments.resize(0);

		assert(s_current_path.size() == 0);
		s_current_path.resize(0);

		assert(curve_error_tolerance > 0);
		if (curve_error_tolerance > 0)
		{
			s_tolerance = curve_error_tolerance;
		}
		else
		{
			s_tolerance = 1.0f;
		}

		s_current_line_style = -1;
		s_current_left_style = -1;
		s_current_right_style = -1;
		s_shape_has_fill = false;
		s_shape_has_line = false;
	}


	static int	compare_segment_y(const void* a, const void* b)
	// For sorting segments by m_begin.m_y, and then by height.
	{
		const fill_segment*	A = (const fill_segment*) a;
		const fill_segment*	B = (const fill_segment*) b;

		const float	ay0 = A->m_begin.m_y;
		const float	by0 = B->m_begin.m_y;

		if (ay0 < by0)
		{
			return -1;
		}
		else if (ay0 == by0)
		{
			const float	ah = A->get_height();
			const float	bh = B->get_height();

			if (ah < bh)
			{
				return -1;
			}
			else if (ah == bh)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}


	static int	compare_segment_x(const void* a, const void* b)
	// For sorting segments by m_begin.m_x, and then by m_end.m_x.
	{
		const fill_segment*	A = (const fill_segment*) a;
		const fill_segment*	B = (const fill_segment*) b;

		const float	ax0 = A->m_begin.m_x;
		const float	bx0 = B->m_begin.m_x;

		if (ax0 < bx0)
		{
			return -1;
		}
		else if (ax0 == bx0)
		{
			const float	ax1 = A->m_end.m_x;
			const float	bx1 = B->m_end.m_x;

			if (ax1 < bx1)
			{
				return -1;
			}
			else if (ax1 == bx1)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}


	void	output_current_segments()
	// Draw our shapes and lines, then clear the segment list.
	{
		if (s_shape_has_fill && s_current_segments.size() > 0)
		{
			//
			// Output the trapezoids making up the filled shape.
			//

			// sort by begining y (smaller first), then by height (shorter first)
			qsort(
				&s_current_segments[0],
				s_current_segments.size(),
				sizeof(s_current_segments[0]),
				compare_segment_y);
		
			int	base = 0;
			while (base < s_current_segments.size())
			{
				float	        ytop = s_current_segments[base].m_begin.m_y;
				int	next_base = base + 1;
				for (;;)
				{
					if (next_base == s_current_segments.size()
					    || s_current_segments[next_base].m_begin.m_y > ytop)
					{
						break;
					}
					next_base++;
				}

				// sort this first part again by y
				qsort(
					&s_current_segments[base],
					next_base - base,
					sizeof(s_current_segments[0]),
					compare_segment_y);

				// s_current_segments[base] through s_current_segments[next_base - 1] is all the segs that start at ytop
				if (next_base >= s_current_segments.size()
				    || s_current_segments[base].m_end.m_y <= s_current_segments[next_base].m_begin.m_y)
				{
					// No segments start between ytop and
					// [base].m_end.m_y, so we can peel
					// off that whole interval and render
					// it right away.
					float	ybottom = s_current_segments[base].m_end.m_y;
					peel_off_and_emit(base, next_base, ytop, ybottom);

					while (base < s_current_segments.size()
					       && s_current_segments[base].m_end.m_y <= ybottom)
					{
						base++;
					}
				}
				else
				{
					float	ybottom = s_current_segments[next_base].m_begin.m_y;
					assert(ybottom > ytop);
					peel_off_and_emit(base, next_base, ytop, ybottom);

					// don't update base; it's still active.
				}
			}
		}
		
		s_current_segments.clear();
	}


	void	peel_off_and_emit(int i0, int i1, float y0, float y1)
	// Clip the interval [y0, y1] off of the segments from
	// s_current_segments[i0 through (i1-1)] and emit the clipped
	// trapezoids.  Modifies the values in s_current_segments.
	{
		assert(i0 < i1);

		if (y0 == y1)
		{
			// Don't bother doing any work...
			return;
		}

		// Peel off first.
		array<fill_segment>	slab;	// @@ make this use static storage
		for (int i = i0; i < i1; i++)
		{
			fill_segment*	f = &s_current_segments[i];
			assert(f->m_begin.m_y == y0);
			assert(f->m_end.m_y >= y1);

			float	dy = f->m_end.m_y - f->m_begin.m_y;
			float	t = 1.0f;
			if (dy > 0)
			{
				t = (y1 - f->m_begin.m_y) / dy;
			}
			point	intersection;
			intersection.m_y = y1;
			intersection.m_x = f->m_begin.m_x + (f->m_end.m_x - f->m_begin.m_x) * t;

			// Peel off.
			slab.push_back(*f);
			slab.back().m_end = intersection;

			// Modify segment.
			s_current_segments[i].m_begin = intersection;
		}

		// Sort by x.
		qsort(&slab[0], slab.size(), sizeof(slab[0]), compare_segment_x);

		// Emit the trapezoids in this slab.
		if (slab.size() > 0
		    && slab[0].m_left_style == -1
		    && slab[0].m_right_style >= 0)
		{
			// Reverse sense of polygon fill!  Right fill style is in charge.
			for (int i = 0; i < slab.size() - 1; i++)
			{
				if (slab[i].m_right_style >= 0)
				{
					trapezoid	tr;
					tr.m_y0 = slab[i].m_begin.m_y;
					tr.m_y1 = slab[i].m_end.m_y;
					tr.m_lx0 = slab[i].m_begin.m_x;
					tr.m_lx1 = slab[i].m_end.m_x;
					tr.m_rx0 = slab[i + 1].m_begin.m_x;
					tr.m_rx1 = slab[i + 1].m_end.m_x;
					s_accepter->accept_trapezoid(slab[i].m_right_style, tr);
				}
			}
		}
		else
		{
			for (int i = 0; i < slab.size() - 1; i++)
			{
				if (slab[i].m_left_style >= 0)
				{
					trapezoid	tr;
					tr.m_y0 = slab[i].m_begin.m_y;
					tr.m_y1 = slab[i].m_end.m_y;
					tr.m_lx0 = slab[i].m_begin.m_x;
					tr.m_lx1 = slab[i].m_end.m_x;
					tr.m_rx0 = slab[i + 1].m_begin.m_x;
					tr.m_rx1 = slab[i + 1].m_end.m_x;
					s_accepter->accept_trapezoid(slab[i].m_left_style, tr);
				}
			}
		}
	}


	void	end_shape()
	{
		output_current_segments();
		s_accepter = NULL;
		s_current_path.clear();
	}


	void	begin_path(int style_left, int style_right, int line_style, float ax, float ay)
	// This call begins recording a sequence of segments, which
	// all share the same fill & line styles.  Add segments to the
	// shape using add_curve_segment() or add_line_segment(), and
	// call end_path() when you're done with this sequence.
	//
	// Pass in -1 for styles that you want to disable.  Otherwise pass in
	// the integral ID of the style for filling, to the left or right.
	{
		s_current_left_style = style_left;
		s_current_right_style = style_right;
		s_current_line_style = line_style;

		s_last_point.m_x = ax;
		s_last_point.m_y = ay;

		assert(s_current_path.size() == 0);
		s_current_path.resize(0);

		s_current_path.push_back(s_last_point);

		if (style_left != -1 || style_right != -1)
		{
			s_shape_has_fill = true;
		}

		if (line_style != -1)
		{
			s_shape_has_line = true;
		}
	}


	void	add_line_segment(float ax, float ay)
	// Add a line running from the previous anchor point to the
	// given new anchor point.
	{
		point	p(ax, ay);

		// s_current_segments is used for filling shapes.
		s_current_segments.push_back(
			fill_segment(
				s_last_point,
				p,
				s_current_left_style,
				s_current_right_style,
				s_current_line_style));

		s_last_point = p;

		s_current_path.push_back(p);
	}


	static void	curve(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y)
	// Recursive routine to generate bezier curve within tolerance.
	{
#ifndef NDEBUG
		static int	recursion_count = 0;
		recursion_count++;
		if (recursion_count > 500)
		{
			assert(0);	// probably a bug!
		}
#endif // not NDEBUG

		// @@ use struct point in here?

		// Midpoint on line between two endpoints.
		float	midx = (p0x + p2x) * 0.5f;
		float	midy = (p0y + p2y) * 0.5f;

		// Midpoint on the curve.
		float	qx = (midx + p1x) * 0.5f;
		float	qy = (midy + p1y) * 0.5f;

		float	dist = fabsf(midx - qx) + fabsf(midy - qy);

		if (dist < s_tolerance)
		{
			// Emit edge.
			add_line_segment(p2x, p2y);
		}
		else
		{
			// Error is too large; subdivide.
			curve(p0x, p0y, (p0x + p1x) * 0.5f, (p0y + p1y) * 0.5f, qx, qy);
			curve(qx, qy, (p1x + p2x) * 0.5f, (p1y + p2y) * 0.5f, p2x, p2y);
		}

#ifndef NDEBUG
		recursion_count--;
#endif // not NDEBUG
	}

	
	void	add_curve_segment(float cx, float cy, float ax, float ay)
	// Add a curve segment to the shape.  The curve segment is a
	// quadratic bezier, running from the previous anchor point to
	// the given new anchor point (ax, ay), with (cx, cy) acting
	// as the control point in between.
	{
		if (cx == ax && cy == ay) {
			// Early-out for degenerate straight segments.
			add_line_segment(ax, ay);
		} else {
		// Subdivide, and add line segments...
		curve(s_last_point.m_x, s_last_point.m_y, cx, cy, ax, ay);
	}
	}


	void	end_path()
	// Mark the end of a set of edges that all use the same styles.
	{
		if (s_current_line_style >= 0 && s_current_path.size() > 1)
		{
			//
			// Emit our line.
			//
			s_accepter->accept_line_strip(s_current_line_style, &s_current_path[0], s_current_path.size());
		}

		s_current_path.resize(0);
	}


}	// end namespace tesselate


namespace tesselate_new
{
	// Curve subdivision error tolerance.
	static float	s_tolerance = 1.0f;
	static mesh_accepter*	s_accepter = NULL;

	struct path_part
	{
		path_part()
			:
			m_left_style(-1),
			m_right_style(-1),
			m_line_style(-1),
			m_closed(false),
			m_processed(false)
		{
		}

		int m_left_style;
		int m_right_style;
		int m_line_style;
		bool m_closed;
		bool m_processed;

		array<point> m_verts;
	};


	static array<path_part>	s_path_parts;
	static point	s_last_point;


	void	begin_shape(mesh_accepter* accepter, float curve_error_tolerance)
	{
		assert(accepter);
		assert(s_accepter == NULL);
		s_accepter = accepter;

		// ensure we're not already in a shape or path.
		// make sure our shape state is cleared out.
		assert(s_path_parts.size() == 0);

		assert(curve_error_tolerance > 0);
		if (curve_error_tolerance > 0)
		{
			s_tolerance = curve_error_tolerance;
		}
		else
		{
			s_tolerance = 1.0f;
		}
	}


	bool try_to_combine_path(int index)
	// Return true if we did any work.
	{
		path_part* pp = &s_path_parts[index];
		if (pp->m_closed || pp->m_right_style == -1 || pp->m_verts.size() <= 0) {
			return false;
		}

		if (pp->m_verts[0] == pp->m_verts.back()) {
			pp->m_closed = true;
			return true;
		}

		// Look for another unclosed path of the same style,
		// which could join our begin or end point.
		int style = pp->m_right_style;
		for (int i = 0; i < s_path_parts.size(); i++) {
			if (i == index) {
				continue;
			}

			path_part* po = &s_path_parts[i];
			if (!po->m_closed && po->m_right_style == style && po->m_verts.size() > 0) {
				// Can we join?
				if (po->m_verts[0] == pp->m_verts.back()) {
					// Yes, po can be appended to pp.
					for (int j = 1; j < po->m_verts.size(); j++) {
						pp->m_verts.push_back(po->m_verts[j]);
					}
					po->m_right_style = -1;
					return true;
				} else if (po->m_verts.back() == pp->m_verts[0]) {
					// Yes, pp can be appended to po.
					for (int j = 1; j < pp->m_verts.size(); j++) {
						po->m_verts.push_back(pp->m_verts[j]);
					}
					pp->m_right_style = -1;
					return true;
				}
			}
		}

		return false;
	}
	
	void copy_points_into_array(array<float>* out, const array<point>& in)
	{
		if (in.size() > 0) {
			// Don't copy the end point, which should be a
			// dupe of the begin point, since the
			// triangulator does that implicitly.
			assert(in[0] == in[in.size() - 1]);
			out->resize((in.size() - 1) * 2);
			for (int i = 0; i < in.size() - 1; i++) {
				(*out)[i * 2] = in[i].m_x;
				(*out)[i * 2 + 1] = in[i].m_y;
			}
		}
	}

	
	void	end_shape()
	{
		// TODO: there's a ton of gratuitous array copying in
		// here! Fix it by being smarter, and by better
		// abstracting the I/O methods for the triangulator.
		
		// Convert left-fill paths into new right-fill paths,
		// so we only have to deal with right-fill below.
		for (int i = 0, n = s_path_parts.size(); i < n; i++) {
			int lstyle = s_path_parts[i].m_left_style;
			int rstyle = s_path_parts[i].m_right_style;

			if (lstyle >= 0)
			{
				if (rstyle == -1)
				{
					s_path_parts[i].m_right_style = s_path_parts[i].m_left_style;
					s_path_parts[i].m_left_style = -1;
					int n = s_path_parts[i].m_verts.size();
					for (int j = 0, k = n >> 1; j < k; j++)
					{
						tu_swap(&s_path_parts[i].m_verts[j], &s_path_parts[i].m_verts[n - j - 1]);
					}
				}
				else
				{
					// Move the data into a new
					// proxy right path.
					s_path_parts.resize(s_path_parts.size() + 1);
					path_part* pold = &s_path_parts[i];
					path_part* pnew = &s_path_parts.back();

					// Copy path, in reverse, into a new right-fill path_part.
					pnew->m_right_style = lstyle;
					pnew->m_verts.reserve(pold->m_verts.size());
					for (int k = pold->m_verts.size() - 1; k >= 0; k--) {
						pnew->m_verts.push_back(pold->m_verts[k]);
					}
					pold->m_left_style = -1;
				}
			}
		}

		// Join path_parts together into closed paths.
		for (;;) {
			bool did_work = false;
			for (int i = 0; i < s_path_parts.size(); i++) {
				did_work = did_work || try_to_combine_path(i);
			}
			if (did_work == false) {
				break;
			}
		}
		
		// Triangulate and emit.
		for (int i = 0; i < s_path_parts.size(); i++) {
			path_part* pp = &s_path_parts[i];
			if (!pp->m_processed && pp->m_right_style != -1 && pp->m_closed && pp->m_verts.size() > 0) {
				pp->m_processed = true;
				int style = pp->m_right_style;
				array<array<float> > paths;
				paths.resize(paths.size() + 1);
				// TODO fix gratuitous array copying
				copy_points_into_array(&paths.back(), pp->m_verts);
				// Grab all the path parts.
				for (int j = i + 1; j < s_path_parts.size(); j++) {
					path_part* pj = &s_path_parts[j];
					if (!pj->m_processed
					    && pj->m_right_style == style
					    && pj->m_closed
					    && pj->m_verts.size() > 0) {
						paths.resize(paths.size() + 1);
						copy_points_into_array(&paths.back(), pj->m_verts);
						pj->m_processed = true;
					}
				}
				array<float> trilist;
				ear_clip_triangulate::compute(&trilist, paths.size(), &paths[0]);
				// TODO: get the results from the
				// triangulator in a more incremental
				// way (without the intermediate
				// trilist buffer!)

				// Give the results to the accepter.
				if (trilist.size() > 0) {
					s_accepter->begin_trilist(style, trilist.size() / 6);
					s_accepter->accept_trilist_batch(
						reinterpret_cast<point*>(&trilist[0]), trilist.size() / 2);
					s_accepter->end_trilist();
				}

// Useful for debugging.  TODO: make a cleaner interface to this.
// 				//xxxxxxxx
// 				if (gameswf_tesselate_dump_shape) {
// 					// Dump paths as a BDM file.
// 					printf("# shape\n");
// 					for (int i = 0; i < paths.size(); i++) {
// 						printf("%d\n", paths[i].size() / 2);
// 						for (j = 0; j < paths[i].size(); j += 2) {
// 							printf("%f %f\n", paths[i][j], paths[i][j + 1]);
// 						}
// 					}
// 				}
// 				//xxxxxxxxx
			}
		}

		s_accepter->end_shape();
		s_accepter = NULL;
		s_path_parts.resize(0);
	}


	void	begin_path(int style_left, int style_right, int line_style, float ax, float ay)
	// This call begins recording a sequence of segments, which
	// all share the same fill & line styles.  Add segments to the
	// shape using add_curve_segment() or add_line_segment(), and
	// call end_path() when you're done with this sequence.
	//
	// Pass in -1 for styles that you want to disable.  Otherwise pass in
	// the integral ID of the style for filling, to the left or right.
	{
		s_path_parts.resize(s_path_parts.size() + 1);
		s_path_parts.back().m_left_style = style_left;
		s_path_parts.back().m_right_style = style_right;
		s_path_parts.back().m_line_style = line_style;

		s_last_point.m_x = ax;
		s_last_point.m_y = ay;

		s_path_parts.back().m_verts.push_back(s_last_point);
	}


	void	add_line_segment(float ax, float ay)
	// Add a line running from the previous anchor point to the
	// given new anchor point.
	{
		s_last_point.m_x = ax;
		s_last_point.m_y = ay;
		s_path_parts.back().m_verts.push_back(s_last_point);
	}


	static void	curve(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y)
	// Recursive routine to generate bezier curve within tolerance.
	{
#ifndef NDEBUG
		static int	recursion_count = 0;
		recursion_count++;
		if (recursion_count > 500)
		{
			assert(0);	// probably a bug!
		}
#endif // not NDEBUG

		// TODO: use struct point in here?

		// Midpoint on line between two endpoints.
		float	midx = (p0x + p2x) * 0.5f;
		float	midy = (p0y + p2y) * 0.5f;

		// Midpoint on the curve.
		float	qx = (midx + p1x) * 0.5f;
		float	qy = (midy + p1y) * 0.5f;

		float	dist = fabsf(midx - qx) + fabsf(midy - qy);

		if (dist < s_tolerance)
		{
			// Emit edge.
			add_line_segment(p2x, p2y);
		}
		else
		{
			// Error is too large; subdivide.
			curve(p0x, p0y, (p0x + p1x) * 0.5f, (p0y + p1y) * 0.5f, qx, qy);
			curve(qx, qy, (p1x + p2x) * 0.5f, (p1y + p2y) * 0.5f, p2x, p2y);
		}

#ifndef NDEBUG
		recursion_count--;
#endif // not NDEBUG
	}

	
	void	add_curve_segment(float cx, float cy, float ax, float ay)
	// Add a curve segment to the shape.  The curve segment is a
	// quadratic bezier, running from the previous anchor point to
	// the given new anchor point (ax, ay), with (cx, cy) acting
	// as the control point in between.
	{
		if (cx == ax && cy == ay) {
			// Early-out for degenerate straight segments.
			add_line_segment(ax, ay);
		} else {
			// Subdivide, and add line segments...
			curve(s_last_point.m_x, s_last_point.m_y, cx, cy, ax, ay);
		}
	}


	void	end_path()
	// Mark the end of a set of edges that all use the same styles.
	{
		if (s_path_parts.back().m_line_style >= 0 && s_path_parts.back().m_verts.size() > 1) {
			// Emit our line.
			s_accepter->accept_line_strip(
				s_path_parts.back().m_line_style,
				&s_path_parts.back().m_verts[0],
				s_path_parts.back().m_verts.size());
		}
	}


}	// end namespace tesselate_new
};	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
