// vert_types.h  -- Thatcher Ulrich 2006

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some basic geometric vertex types.


#ifndef VERT_TYPES_H
#define VERT_TYPES_H


// convenience struct; this could use some public vec2 type, but often
// it's nicer for users if the external interface is smaller and more
// c-like, since they probably have their own vec2 that they prefer.
template<class coord_t>
struct vec2
{
	vec2() : x(0), y(0) {}
	vec2(coord_t _x, coord_t _y) : x(_x), y(_y) {}

	bool	operator==(const vec2<coord_t>& v) const
	{
		return x == v.x && y == v.y;
	}
	bool operator!=(const vec2<coord_t>& v) const
	{
		return !operator==(v);
	}
	bool operator<(const vec2<coord_t>& v) const
	// For sorting verts lexicographically.
	{
		if (x < v.x) {
			return true;
		} else if (x == v.x) {
			if (y < v.y) {
				return true;
			}
		}
		return false;
	}
	
//data:
	coord_t	x, y;
};


inline double	determinant_float(const vec2<float>& a, const vec2<float>& b, const vec2<float>& c)
{
	return (double(b.x) - double(a.x)) * (double(c.y) - double(a.y))
		- (double(b.y) - double(a.y)) * (double(c.x) - double(a.x));
}


// This is not really valid!  double has only 52 mantissa bits, but we
// need about 65 for worst-case 32-bit determinant.
inline double	determinant_sint32(const vec2<sint32>& a, const vec2<sint32>& b, const vec2<sint32>& c)
{
	return (double(b.x) - double(a.x)) * (double(c.y) - double(a.y))
		- (double(b.y) - double(a.y)) * (double(c.x) - double(a.x));
}


// double would be OK, and possibly faster on PC-like architectures,
// but I'm curious to see if I can get away w/ no floating point for
// triangulating meshes w/ 16-bit coords.
inline sint64	determinant_sint16(const vec2<sint16>& a, const vec2<sint16>& b, const vec2<sint16>& c)
{
	return (sint64(b.x) - sint64(a.x)) * (sint64(c.y) - sint64(a.y))
		- (sint64(b.y) - sint64(a.y)) * (sint64(c.x) - sint64(a.x));
}


// Return {-1,0,1} if c is {to the right, on, to the left} of the
// directed edge defined by a->b.
template<class coord_t>
inline int	vertex_left_test(const vec2<coord_t>& a, const vec2<coord_t>& b, const vec2<coord_t>& c)
{
	//compiler_assert(0);	// must specialize
	return -1;
}


template<>
inline int	vertex_left_test(const vec2<float>& a, const vec2<float>& b, const vec2<float>& c)
// Specialize for vec2<float>
{
	double	det = determinant_float(a, b, c);
	if (det > 0) return 1;
	else if (det < 0) return -1;
	else return 0;
}


template<>
inline int	vertex_left_test(const vec2<sint32>& a, const vec2<sint32>& b, const vec2<sint32>& c)
// Specialize for vec2<sint32>
{
	double	det = determinant_sint32(a, b, c);
	if (det > 0) return 1;
	else if (det < 0) return -1;
	else return 0;
}


template<>
inline int	vertex_left_test(const vec2<sint16>& a, const vec2<sint16>& b, const vec2<sint16>& c)
// Specialize for vec2<sint16>
{
	sint64	det = determinant_sint16(a, b, c);
	if (det > 0) return 1;
	else if (det < 0) return -1;
	else return 0;
}


// TODO: these don't really have to be inline.


inline void edges_intersect_sub(int* e0_vs_e1, int* e1_vs_e0,
				const vec2<sint16>& e0v0, const vec2<sint16>& e0v1,
				const vec2<sint16>& e1v0, const vec2<sint16>& e1v1)
// Return {-1,0,1} for edge A {crossing, vert-touching, not-crossing}
// the line of edge B.
//
// Specialized for sint16
{
	// If e1v0,e1v1 are on opposite sides of e0, and e0v0,e0v1 are
	// on opposite sides of e1, then the segments cross.  These
	// are all determinant checks.

	// The main degenerate case we need to watch out for is if
	// both segments are zero-length.
	//
	// If only one is degenerate, our tests are still OK.

	if (e0v0.x == e0v1.x && e0v0.y == e0v1.y)
	{
		// e0 is zero length.
		if (e1v0.x == e1v1.x && e1v0.y == e1v1.y)
		{
			if (e1v0.x == e0v0.x && e1v0.y == e0v0.y) {
				// Coincident.
				*e0_vs_e1 = 0;
				*e1_vs_e0 = 0;
				return;
			}
		}
	}

	// See if e1 crosses line of e0.
	sint64	det10 = determinant_sint16(e0v0, e0v1, e1v0);
	sint64	det11 = determinant_sint16(e0v0, e0v1, e1v1);

	// Note: we do > 0, which means a vertex on a line counts as
	// intersecting.  In general, if one vert is on the other
	// segment, we have to go searching along the path in either
	// direction to see if it crosses or not, and it gets
	// complicated.  Better to treat it as intersection.

	int det1sign = 0;
	if (det11 < 0) det1sign = -1;
	else if (det11 > 0) det1sign = 1;
	if (det10 < 0) det1sign = -det1sign;
	else if (det10 == 0) det1sign = 0;

	if (det1sign > 0) {
		// e1 doesn't cross the line of e0.
		*e1_vs_e0 = 1;
	} else if (det1sign < 0) {
		// e1 does cross the line of e0.
		*e1_vs_e0 = -1;
	} else {
		// One (or both) of the endpoints of e1 are on the
		// line of e0.
		*e1_vs_e0 = 0;
	}

	// See if e0 crosses line of e1.
	sint64	det00 = determinant_sint16(e1v0, e1v1, e0v0);
	sint64	det01 = determinant_sint16(e1v0, e1v1, e0v1);

	int det0sign = 0;
	if (det01 < 0) det0sign = -1;
	else if (det01 > 0) det0sign = 1;
	if (det00 < 0) det0sign = -det0sign;
	else if (det00 == 0) det0sign = 0;

	if (det0sign > 0) {
		// e0 doesn't cross the line of e1.
		*e0_vs_e1 = 1;
	} else if (det0sign < 0) {
		// e0 crosses line of e1
		*e0_vs_e1 = -1;
	} else {
		// One (or both) of the endpoints of e0 are on the
		// line of e1.
		*e0_vs_e1 = 0;
	}
}


inline void edges_intersect_sub(int* e0_vs_e1, int* e1_vs_e0,
				const vec2<float>& e0v0, const vec2<float>& e0v1,
				const vec2<float>& e1v0, const vec2<float>& e1v1)
// Return {-1,0,1} for edge A {crossing, vert-touching, not-crossing}
// the line of edge B.
//
// Specialized for float
{
	// If e1v0,e1v1 are on opposite sides of e0, and e0v0,e0v1 are
	// on opposite sides of e1, then the segments cross.  These
	// are all determinant checks.

	// The main degenerate case we need to watch out for is if
	// both segments are zero-length.
	//
	// If only one is degenerate, our tests are still OK.

	if (e0v0.x == e0v1.x && e0v0.y == e0v1.y)
	{
		// e0 is zero length.
		if (e1v0.x == e1v1.x && e1v0.y == e1v1.y)
		{
			if (e1v0.x == e0v0.x && e1v0.y == e0v0.y) {
				// Coincident.
				*e0_vs_e1 = 0;
				*e1_vs_e0 = 0;
				return;
			}
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

	int det1sign = 0;
	if (det11 < 0) det1sign = -1;
	else if (det11 > 0) det1sign = 1;
	if (det10 < 0) det1sign = -det1sign;
	else if (det10 == 0) det1sign = 0;

	if (det1sign > 0) {
		// e1 doesn't cross the line of e0.
		*e1_vs_e0 = 1;
	} else if (det1sign < 0) {
		// e1 does cross the line of e0.
		*e1_vs_e0 = -1;
	} else {
		// One (or both) of the endpoints of e1 are on the
		// line of e0.
		*e1_vs_e0 = 0;
	}

	// See if e0 crosses line of e1.
	double	det00 = determinant_float(e1v0, e1v1, e0v0);
	double	det01 = determinant_float(e1v0, e1v1, e0v1);

	int det0sign = 0;
	if (det01 < 0) det0sign = -1;
	else if (det01 > 0) det0sign = 1;
	if (det00 < 0) det0sign = -det0sign;
	else if (det00 == 0) det0sign = 0;

	if (det0sign > 0) {
		// e0 doesn't cross the line of e1.
		*e0_vs_e1 = 1;
	} else if (det0sign < 0) {
		// e0 crosses line of e1
		*e0_vs_e1 = -1;
	} else {
		// One (or both) of the endpoints of e0 are on the
		// line of e1.
		*e0_vs_e1 = 0;
	}
}


inline bool vert_in_triangle(const vec2<sint16>& v, const vec2<sint16>& v0, const vec2<sint16>& v1, const vec2<sint16>& v2)
// Return true if v touches the boundary or the interior of triangle (v0, v1, v2).
//
// Specialized for sint16
{
	sint64 det0 = determinant_sint16(v0, v1, v);
	if (det0 >= 0) {
		sint64 det1 = determinant_sint16(v1, v2, v);
		if (det1 >= 0) {
			sint64 det2 = determinant_sint16(v2, v0, v);
			if (det2 >= 0) {
				// Point touches the triangle.
				return true;
			}
		}
	}
	return false;
}


inline bool vert_in_triangle(const vec2<float>& v, const vec2<float>& v0, const vec2<float>& v1, const vec2<float>& v2)
// Return true if v touches the boundary or the interior of triangle (v0, v1, v2).
//
// Specialized for sint16
{
	double det0 = determinant_float(v0, v1, v);
	if (det0 >= 0) {
		double det1 = determinant_float(v1, v2, v);
		if (det1 >= 0) {
			double det2 = determinant_float(v2, v0, v);
			if (det2 >= 0) {
				// Point touches the triangle.
				return true;
			}
		}
	}
	return false;
}







#endif // VERT_TYPES_H

