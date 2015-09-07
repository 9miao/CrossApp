// gameswf_types.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some basic types for gameswf.

#include "gameswf_types.h"

#include "gameswf_log.h"
#include "gameswf_stream.h"
#include "gameswf_render.h"
#include "gameswf.h"
#include <string.h>


namespace gameswf
{

	//
	// point
	//


	bool	point::bitwise_equal(const point& p) const
	// Bitwise comparison; return true if *this is bitwise
	// identical to p.
	{
		return memcmp(this, &p, sizeof(p)) == 0;
	}
	
	float point::get_length() const
	{
		return sqrt( m_x * m_x + m_y * m_y );
	}

	void point::twips_to_pixels()
	{
		m_x = TWIPS_TO_PIXELS(m_x);
		m_y = TWIPS_TO_PIXELS(m_y);
	}

	void point::pixels_to_twips()
	{
		m_x = PIXELS_TO_TWIPS(m_x);
		m_y = PIXELS_TO_TWIPS(m_y);
	}

	//
	// matrix
	//

	matrix	matrix::identity;

	matrix::matrix()
	{
		// Default to identity.
		set_identity();
	}


//	bool	matrix::is_valid() const
//	{
//		return isfinite(m_[0][0])
//			&& isfinite(m_[0][1])
//			&& isfinite(m_[0][2])
//			&& isfinite(m_[1][0])
//			&& isfinite(m_[1][1])
//			&& isfinite(m_[1][2]);
//	}


	void	matrix::set_identity()
	// Set the matrix to identity.
	{
		memset(&m_[0], 0, sizeof(m_));
		m_[0][0] = 1;
		m_[1][1] = 1;
	}


	void	matrix::concatenate(const matrix& m)
	// Concatenate m's transform onto ours.  When
	// transforming points, m happens first, then our
	// original xform.
	{
		matrix	t;
		t.m_[0][0] = m_[0][0] * m.m_[0][0] + m_[0][1] * m.m_[1][0];
		t.m_[1][0] = m_[1][0] * m.m_[0][0] + m_[1][1] * m.m_[1][0];
		t.m_[0][1] = m_[0][0] * m.m_[0][1] + m_[0][1] * m.m_[1][1];
		t.m_[1][1] = m_[1][0] * m.m_[0][1] + m_[1][1] * m.m_[1][1];
		t.m_[0][2] = m_[0][0] * m.m_[0][2] + m_[0][1] * m.m_[1][2] + m_[0][2];
		t.m_[1][2] = m_[1][0] * m.m_[0][2] + m_[1][1] * m.m_[1][2] + m_[1][2];

		*this = t;
	}


	void	matrix::concatenate_translation(float tx, float ty)
	// Concatenate a translation onto the front of our
	// matrix.  When transforming points, the translation
	// happens first, then our original xform.
	{
		m_[0][2] += m_[0][0] * tx + m_[0][1] * ty;
		m_[1][2] += m_[1][0] * tx + m_[1][1] * ty;
	}


	void	matrix::concatenate_scale(float scale)
	// Concatenate a uniform scale onto the front of our
	// matrix.  When transforming points, the scale
	// happens first, then our original xform.
	{
		m_[0][0] *= scale;
		m_[0][1] *= scale;
		m_[1][0] *= scale;
		m_[1][1] *= scale;
	}


	void	matrix::set_lerp(const matrix& m1, const matrix& m2, float t)
	// Set this matrix to a blend of m1 and m2, parameterized by t.
	{
		m_[0][0] = flerp(m1.m_[0][0], m2.m_[0][0], t);
		m_[1][0] = flerp(m1.m_[1][0], m2.m_[1][0], t);
		m_[0][1] = flerp(m1.m_[0][1], m2.m_[0][1], t);
		m_[1][1] = flerp(m1.m_[1][1], m2.m_[1][1], t);
		m_[0][2] = flerp(m1.m_[0][2], m2.m_[0][2], t);
		m_[1][2] = flerp(m1.m_[1][2], m2.m_[1][2], t);
	}

	
	void	matrix::set_scale_rotation(float x_scale, float y_scale, float angle)
	// Set the scale & rotation part of the matrix.
	// angle in radians.
	{
		float	cos_angle = cosf(angle);
		float	sin_angle = sinf(angle);
		m_[0][0] = x_scale * cos_angle;
		m_[0][1] = y_scale * -sin_angle;
		m_[1][0] = x_scale * sin_angle;
		m_[1][1] = y_scale * cos_angle;
	}


	void	matrix::read(stream* in)
	// Initialize from the stream.
	{
		in->align();

		set_identity();

		int	has_scale = in->read_uint(1);
		if (has_scale)
		{
			int	scale_nbits = in->read_uint(5);
			m_[0][0] = in->read_sint(scale_nbits) / 65536.0f;
			m_[1][1] = in->read_sint(scale_nbits) / 65536.0f;
		}
		int	has_rotate = in->read_uint(1);
		if (has_rotate)
		{
			int	rotate_nbits = in->read_uint(5);
			m_[1][0] = in->read_sint(rotate_nbits) / 65536.0f;
			m_[0][1] = in->read_sint(rotate_nbits) / 65536.0f;
		}

		int	translate_nbits = in->read_uint(5);
		if (translate_nbits > 0)
		{
			m_[0][2] = (float) in->read_sint(translate_nbits);
			m_[1][2] = (float) in->read_sint(translate_nbits);
		}

		//IF_VERBOSE_PARSE(log_msg("  mat: has_scale = %d, has_rotate = %d\n", has_scale, has_rotate));
	}


	void	matrix::print() const
	// Debug log.
	{
		log_msg("| %4.4f %4.4f %4.4f |\n", float(m_[0][0]), float(m_[0][1]), TWIPS_TO_PIXELS(m_[0][2]));
		log_msg("| %4.4f %4.4f %4.4f |\n", float(m_[1][0]), float(m_[1][1]), TWIPS_TO_PIXELS(m_[1][2]));
	}

	void	matrix::transform(point* result, const point& p) const
	// Transform point 'p' by our matrix.  Put the result in
	// *result.
	{
		assert(result);
		assert(&p != result);

		result->m_x = m_[0][0] * p.m_x + m_[0][1] * p.m_y + m_[0][2];
		result->m_y = m_[1][0] * p.m_x + m_[1][1] * p.m_y + m_[1][2];
	}

	void	matrix::transform(rect* bound) const
	// Transform bound our matrix.
	{
		// get corners of transformed bound
		point p[4];
		transform(p + 0, point(bound->get_corner(0)));
		transform(p + 1, point(bound->get_corner(1)));
		transform(p + 2, point(bound->get_corner(2)));
		transform(p + 3, point(bound->get_corner(3)));

		// Build bound that covers transformed bound
		bound->set_to_point(p[0]);
		bound->expand_to_point(p[1]);
		bound->expand_to_point(p[2]);
		bound->expand_to_point(p[3]);
	}
	
	void	matrix::transform_vector(point* result, const point& v) const
	// Transform vector 'v' by our matrix. Doesn't apply translation.
	// Put the result in *result.
	{
		assert(result);
		assert(&v != result);

		result->m_x = m_[0][0] * v.m_x + m_[0][1] * v.m_y;
		result->m_y = m_[1][0] * v.m_x + m_[1][1] * v.m_y;
	}

	void	matrix::transform_by_inverse(point* result, const point& p) const
	// Transform point 'p' by the inverse of our matrix.  Put result in *result.
	{
		// @@ TODO optimize this!
		matrix	m;
		m.set_inverse(*this);
		m.transform(result, p);
	}

	void	matrix::transform_by_inverse(rect* bound) const
	// Transform point 'p' by the inverse of our matrix.  Put result in *result.
	{
		// @@ TODO optimize this!
		matrix	m;
		m.set_inverse(*this);
		m.transform(bound);
	}

	void	matrix::set_inverse(const matrix& m)
	// Set this matrix to the inverse of the given matrix.
	{
		assert(this != &m);

		// Invert the rotation part.
		float	det = m.m_[0][0] * m.m_[1][1] - m.m_[0][1] * m.m_[1][0];
		if (det == 0.0f)
		{
			// Not invertible.
			//assert(0);	// castano: this happens sometimes! (ie. sample6.swf)

			// Arbitrary fallback.
			set_identity();
			m_[0][2] = -m.m_[0][2];
			m_[1][2] = -m.m_[1][2];
		}
		else
		{
			float	inv_det = 1.0f / det;
			m_[0][0] = m.m_[1][1] * inv_det;
			m_[1][1] = m.m_[0][0] * inv_det;
			m_[0][1] = -m.m_[0][1] * inv_det;
			m_[1][0] = -m.m_[1][0] * inv_det;

			m_[0][2] = -(m_[0][0] * m.m_[0][2] + m_[0][1] * m.m_[1][2]);
			m_[1][2] = -(m_[1][0] * m.m_[0][2] + m_[1][1] * m.m_[1][2]);
		}
	}


	bool	matrix::does_flip() const
	// Return true if this matrix reverses handedness.
	{
		float	det = m_[0][0] * m_[1][1] - m_[0][1] * m_[1][0];

		return det < 0.f;
	}


	float	matrix::get_determinant() const
	// Return the determinant of the 2x2 rotation/scale part only.
	{
		return m_[0][0] * m_[1][1] - m_[1][0] * m_[0][1];
	}


	float	matrix::get_max_scale() const
	// Return the maximum scale factor that this transform
	// applies.  For assessing scale, when determining acceptable
	// errors in tesselation.
	{
		// @@ not 100% sure what the heck I'm doing here.  I
		// think this is roughly what I want; take the max
		// length of the two basis vectors.
		float	basis0_length2 = m_[0][0] * m_[0][0] + m_[0][1] * m_[0][1];
		float	basis1_length2 = m_[1][0] * m_[1][0] + m_[1][1] * m_[1][1];
		float	max_length2 = fmax_swf(basis0_length2, basis1_length2);
		return sqrtf(max_length2);
	}

	float	matrix::get_x_scale() const
	{
		float scale = sqrtf(m_[0][0] * m_[0][0] + m_[1][0] * m_[1][0]);

		// Are we turned inside out?
		if (get_determinant() < 0.f)
		{
			scale = -scale;
		}

		return scale;
	}

	float	matrix::get_y_scale() const
	{
		return sqrtf(m_[1][1] * m_[1][1] + m_[0][1] * m_[0][1]);
	}

	float	matrix::get_rotation() const
	{
		if (get_determinant() < 0.f)
		{
			// We're turned inside out; negate the
			// x-component used to compute rotation.
			//
			// Matches get_x_scale().
			//
			// @@ this may not be how Macromedia does it!  Test this!
			return atan2f(m_[1][0], -m_[0][0]);
		}
		else
		{
			return atan2f(m_[1][0], m_[0][0]);
		}
	}


	//
	// cxform
	//


	cxform	cxform::identity;


	cxform::cxform()
	// Initialize to identity transform.
	{
		m_[0][0] = 1;
		m_[1][0] = 1;
		m_[2][0] = 1;
		m_[3][0] = 1;
		m_[0][1] = 0;
		m_[1][1] = 0;
		m_[2][1] = 0;
		m_[3][1] = 0;
	}

	void	cxform::concatenate(const cxform& c)
	// Concatenate c's transform onto ours.  When
	// transforming colors, c's transform is applied
	// first, then ours.
	{
		m_[0][1] += m_[0][0] * c.m_[0][1];
		m_[1][1] += m_[1][0] * c.m_[1][1];
		m_[2][1] += m_[2][0] * c.m_[2][1];
		m_[3][1] += m_[3][0] * c.m_[3][1];

		m_[0][0] *= c.m_[0][0];
		m_[1][0] *= c.m_[1][0];
		m_[2][0] *= c.m_[2][0];
		m_[3][0] *= c.m_[3][0];
	}

	
	rgba	cxform::transform(const rgba in) const
	// Apply our transform to the given color; return the result.
	{
		rgba	result;

		result.m_r = (Uint8) fclamp(in.m_r * m_[0][0] + m_[0][1], 0, 255);
		result.m_g = (Uint8) fclamp(in.m_g * m_[1][0] + m_[1][1], 0, 255);
		result.m_b = (Uint8) fclamp(in.m_b * m_[2][0] + m_[2][1], 0, 255);
		result.m_a = (Uint8) fclamp(in.m_a * m_[3][0] + m_[3][1], 0, 255);

		return result;
	}


	void	cxform::read_rgb(stream* in)
	{
		in->align();

		int	has_add = in->read_uint(1);
		int	has_mult = in->read_uint(1);
		int	nbits = in->read_uint(4);

		if (has_mult) {
			m_[0][0] = in->read_sint(nbits) / 255.0f;
			m_[1][0] = in->read_sint(nbits) / 255.0f;
			m_[2][0] = in->read_sint(nbits) / 255.0f;
			m_[3][0] = 1;
		}
		else {
			for (int i = 0; i < 4; i++) { m_[i][0] = 1; }
		}
		if (has_add) {
			m_[0][1] = (float) in->read_sint(nbits);
			m_[1][1] = (float) in->read_sint(nbits);
			m_[2][1] = (float) in->read_sint(nbits);
			m_[3][1] = 1;
		}
		else {
			for (int i = 0; i < 4; i++) { m_[i][1] = 0; }
		}
	}

	void	cxform::read_rgba(stream* in)
	{
		in->align();

		int	has_add = in->read_uint(1);
		int	has_mult = in->read_uint(1);
		int	nbits = in->read_uint(4);

		if (has_mult) {
			m_[0][0] = in->read_sint(nbits) / 256.0f;
			m_[1][0] = in->read_sint(nbits) / 256.0f;
			m_[2][0] = in->read_sint(nbits) / 256.0f;
			m_[3][0] = in->read_sint(nbits) / 256.0f;
		}
		else {
			for (int i = 0; i < 4; i++) { m_[i][0] = 1; }
		}
		if (has_add) {
			m_[0][1] = (float) in->read_sint(nbits);
			m_[1][1] = (float) in->read_sint(nbits);
			m_[2][1] = (float) in->read_sint(nbits);
			m_[3][1] = (float) in->read_sint(nbits);
		}
		else {
			for (int i = 0; i < 4; i++) { m_[i][1] = 0; }
		}
	}

	void cxform::clamp()
	// Force component values to be in legal range.
	{
		m_[0][0] = fclamp(m_[0][0], 0, 1);
		m_[1][0] = fclamp(m_[1][0], 0, 1);
		m_[2][0] = fclamp(m_[2][0], 0, 1);
		m_[3][0] = fclamp(m_[3][0], 0, 1);

		m_[0][1] = fclamp(m_[0][1], -255.0f, 255.0f);
		m_[1][1] = fclamp(m_[1][1], -255.0f, 255.0f);
		m_[2][1] = fclamp(m_[2][1], -255.0f, 255.0f);
		m_[3][1] = fclamp(m_[3][1], -255.0f, 255.0f);
	}


	void	cxform::print() const
	// Debug log.
	{
		log_msg("    *         +\n");
		log_msg("| %4.4f %4.4f|\n", float(m_[0][0]), float(m_[0][1]));
		log_msg("| %4.4f %4.4f|\n", float(m_[1][0]), float(m_[1][1]));
		log_msg("| %4.4f %4.4f|\n", float(m_[2][0]), float(m_[2][1]));
		log_msg("| %4.4f %4.4f|\n", float(m_[3][0]), float(m_[3][1]));
	}


	//
	// rgba
	//

	void	rgba::read(stream* in, int tag_type)
	{
		if (tag_type <= 22)
		{
			read_rgb(in);
		}
		else
		{
			read_rgba(in);
		}
	}

	void	rgba::read_rgba(stream* in)
	{
		read_rgb(in);
		m_a = in->read_u8();
	}

	void	rgba::read_rgb(stream* in)
	{
		m_r = in->read_u8();
		m_g = in->read_u8();
		m_b = in->read_u8();
		m_a = 0x0FF;
	}

	void	rgba::print()
	// For debugging.
	{
		log_msg("rgba: %d %d %d %d\n", m_r, m_g, m_b, m_a);
	}

	
	void	rgba::set_lerp(const rgba& a, const rgba& b, float f)
	{
		m_r = (Uint8) frnd(flerp(a.m_r, b.m_r, f));
		m_g = (Uint8) frnd(flerp(a.m_g, b.m_g, f));
		m_b = (Uint8) frnd(flerp(a.m_b, b.m_b, f));
		m_a = (Uint8) frnd(flerp(a.m_a, b.m_a, f));
	}


	//
	// rect
	//


	void	rect::read(stream* in)
	{
		in->align();
		int	nbits = in->read_uint(5);
		m_x_min = (float) in->read_sint(nbits);
		m_x_max = (float) in->read_sint(nbits);
		m_y_min = (float) in->read_sint(nbits);
		m_y_max = (float) in->read_sint(nbits);

//		IF_DEBUG(log_msg("rect::read() nbits = %d\n", nbits));
	}

	void	rect::print() const
	// Debug spew.
	{
		log_msg("xmin = %g, ymin = %g, xmax = %g, ymax = %g\n",
			TWIPS_TO_PIXELS(m_x_min),
			TWIPS_TO_PIXELS(m_y_min),
			TWIPS_TO_PIXELS(m_x_max),
			TWIPS_TO_PIXELS(m_y_max));
	}

	
	bool	rect::point_test(float x, float y) const
	// Return true if the specified point is inside this rect.
	{
		if (x < m_x_min
		    || x > m_x_max
		    || y < m_y_min
		    || y > m_y_max)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	bool	rect::bound_test(const rect& bound) const
	// Return true if the specified rect intersect this rect
	{
		if (m_x_max < bound.m_x_min
			|| m_x_min > bound.m_x_max 
			|| m_y_min > bound.m_y_max 
			|| m_y_max < bound.m_y_min)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void rect::set_to_point(float x, float y)
	{
		m_x_min = m_x_max = x;
		m_y_min = m_y_max = y;
	}

	void rect::set_to_point(const point& p)
	{
		set_to_point(p.m_x, p.m_y);
	}

	void	rect::expand_to_point(float x, float y)
	// Expand this rectangle to enclose the given point.
	{
		m_x_min = fmin_swf(m_x_min, x);
		m_y_min = fmin_swf(m_y_min, y);
		m_x_max = fmax_swf(m_x_max, x);
		m_y_max = fmax_swf(m_y_max, y);
	}

	void	rect::expand_to_point(const point& p)
	{
		expand_to_point(p.m_x, p.m_y);
	}

	void rect::expand_to_rect(const rect& r)
	// Expand to enclose the given rectangle.
	{
		expand_to_point(r.m_x_min, r.m_y_min);
		expand_to_point(r.m_x_max, r.m_y_max);
	}

	point	rect::get_corner(int i) const
	// Get one of the rect verts.
	{
		assert(i >= 0 && i < 4);
		return point(
			(i == 0 || i == 3) ? m_x_min : m_x_max,
			(i < 2) ? m_y_min : m_y_max);
	}

	void	rect::enclose_transformed_rect(const matrix& m, const rect& r)
	// Set ourself to bound a rectangle that has been transformed
	// by m.  This is an axial bound of an oriented (and/or
	// sheared, scaled, etc) box.
	{
		// Get the transformed bounding box.
		point	p0, p1, p2, p3;
		m.transform(&p0, r.get_corner(0));
		m.transform(&p1, r.get_corner(1));
		m.transform(&p2, r.get_corner(2));
		m.transform(&p3, r.get_corner(3));

		// Vitaly: It is necessary to test the case when p0 is not min(p1, p2, p3)
		m_x_min = m_x_max = p0.m_x;
		m_y_min = m_y_max = p0.m_y;
		expand_to_point(p1.m_x, p1.m_y);
		expand_to_point(p2.m_x, p2.m_y);
		expand_to_point(p3.m_x, p3.m_y);
	}


	void	rect::set_lerp(const rect& a, const rect& b, float t)
	// Set this to the lerp of a and b.
	{
		m_x_min = flerp(a.m_x_min, b.m_x_min, t);
		m_y_min = flerp(a.m_y_min, b.m_y_min, t);
		m_x_max = flerp(a.m_x_max, b.m_x_max, t);
		m_y_max = flerp(a.m_y_max, b.m_y_max, t);
	}

	void rect::twips_to_pixels()
	{
		m_x_min = TWIPS_TO_PIXELS(m_x_min);
		m_y_min = TWIPS_TO_PIXELS(m_y_min);
		m_x_max = TWIPS_TO_PIXELS(m_x_max);
		m_y_max = TWIPS_TO_PIXELS(m_y_max);
	}

	void rect::pixels_to_twips()
	{
		m_x_min = PIXELS_TO_TWIPS(m_x_min);
		m_y_min = PIXELS_TO_TWIPS(m_y_min);
		m_x_max = PIXELS_TO_TWIPS(m_x_max);
		m_y_max = PIXELS_TO_TWIPS(m_y_max);
	}


};	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
