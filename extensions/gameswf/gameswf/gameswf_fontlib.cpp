// gameswf_fontlib.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A module to take care of all of gameswf's loaded fonts.


#include "../base/container.h"
#include "../base/tu_file.h"
#include "gameswf.h"
#include "gameswf_font.h"
#include "gameswf_impl.h"
#include "gameswf_log.h"
#include "gameswf_render.h"
#include "gameswf_shape.h"
#include "gameswf_styles.h"
#include "gameswf_tesselate.h"
#include "gameswf_render.h"
#include "gameswf_movie_def.h"
#include "gameswf_fontlib.h"

namespace gameswf
{

	// Size (in TWIPS) of the box that the glyph should stay within.

	static const int	OVERSAMPLE_BITS = 1;
	static const int	OVERSAMPLE_FACTOR = (1 << OVERSAMPLE_BITS);

	// The nominal size of the final antialiased glyphs stored in
	// the texture.  This parameter controls how large the very
	// largest glyphs will be in the texture; most glyphs will be
	// considerably smaller.  This is also the parameter that
	// controls the tradeoff between texture RAM usage and
	// sharpness of large text.
	static int	s_glyph_nominal_size = 96;

	static int	s_rendering_box = OVERSAMPLE_FACTOR * s_glyph_nominal_size;

	// The dimensions of the textures that the glyphs get packed into.
	static const int	GLYPH_CACHE_TEXTURE_SIZE = 128;

	// How much space to leave around the individual glyph image.
	// This should be at least 1.  The bigger it is, the smoother
	// the boundaries of minified text will be, but the more
	// texture space is wasted.
	const int PAD_PIXELS = 3;

	// The raw non-antialiased render size for glyphs.
	static int	s_glyph_render_size = s_glyph_nominal_size << OVERSAMPLE_BITS;

	//
	// State for the glyph packer.
	//

	static Uint8*	s_render_buffer = NULL;
	static Uint8*	s_image_buffer = NULL;	// antialiased image from s_render_buffer
	static matrix	s_render_matrix;

	// Integer-bounded 2D rectangle.
	struct recti
	{
		int	m_x_min, m_x_max, m_y_min, m_y_max;

		recti(int x0 = 0, int x1 = 0, int y0 = 0, int y1 = 0)
			:
			m_x_min(x0),
			m_x_max(x1),
			m_y_min(y0),
			m_y_max(y1)
		{
		}

		bool	is_valid() const
		{
			return m_x_min <= m_x_max
				&& m_y_min <= m_y_max;
		}

		bool	intersects(const recti& r) const
		// Return true if r touches *this.
		{
			if (m_x_min >= r.m_x_max
			    || m_x_max <= r.m_x_min
			    || m_y_min >= r.m_y_max
			    || m_y_max <= r.m_y_min)
			{
				// disjoint.
				return false;
			}
			return true;
		}

		bool	contains(int x, int y) const
		// Return true if (x,y) is inside *this.
		{
			return x >= m_x_min
				&& x < m_x_max
				&& y >= m_y_min
				&& y < m_y_max;
		}
	};

	// Rects already on the texture.
	static swf_array<recti>	s_covered_rects;

	// 2d integer point.
	struct pointi
	{
		int	m_x, m_y;

		pointi(int x = 0, int y = 0)
			:
			m_x(x),
			m_y(y)
		{
		}

		bool	operator<(const pointi& p) const
		// For sorting anchor points.
		{
			return imin_swf(m_x, m_y) < imin_swf(p.m_x, p.m_y);
		}
	};

	// Candidates for upper-left corner of a new rectangle.  Use
	// lower-left and upper-right of previously placed rects.
	static swf_array<pointi>	s_anchor_points;

	bool	is_rect_available(const recti& r)
	// Return true if the given rect can be packed into the
	// currently active texture.
	{
		assert(r.is_valid());
		assert(r.m_x_min >= 0);
		assert(r.m_y_min >= 0);

		if (r.m_x_max > GLYPH_CACHE_TEXTURE_SIZE
		    || r.m_y_max > GLYPH_CACHE_TEXTURE_SIZE)
		{
			// Rect overflows the texture bounds.
			return false;
		}

		// Check against existing rects.
		for (int i = 0, n = s_covered_rects.size(); i < n; i++)
		{
			if (r.intersects(s_covered_rects[i]))
			{
				return false;
			}
		}

		// Spot appears to be open.
		return true;
	}


	void	add_cover_rect(const recti& r)
	// Add the given rect to our list.  Eliminate any anchor
	// points that are disqualified by this new rect.
	{
		s_covered_rects.push_back(r);

		for (int i = 0; i < s_anchor_points.size(); i++)
		{
			const pointi&	p = s_anchor_points[i];
			if (r.contains(p.m_x, p.m_y))
			{
				// Eliminate this point from consideration.
				s_anchor_points.remove(i);
				i--;
			}
		}
	}


	void	add_anchor_point(const pointi& p)
	// Add point to our list of anchors.  Keep the list sorted.
	{
		// Add it to end, since we expect new points to be
		// relatively greater than existing points.
		s_anchor_points.push_back(p);

		// Insertion sort -- bubble down into correct spot.
		for (int i = s_anchor_points.size() - 2; i >= 0; i--)
		{
			if (s_anchor_points[i + 1] < s_anchor_points[i])
			{
				tu_swap(&(s_anchor_points[i]), &(s_anchor_points[i + 1]));
			}
			else
			{
				// Done bubbling down.
				break;
			}
		}
	}


	bool	pack_rectangle(int* px, int* py, int width, int height)
	// Find a spot for the rectangle in the current cache image.
	// Return true if there's a spot; false if there's no room.
	{
		// Nice algo, due to JARE:
		//
		// * keep a list of "candidate points"; initialize it with {0,0}
		//
		// * each time we add a rect, add its lower-left and
		// upper-right as candidate points.
		//
		// * search the candidate points only, when looking
		// for a good spot.  If we find a good one, also try
		// scanning left or up as well; sometimes this can
		// close some open space.
		//
		// * when we use a candidate point, remove it from the list.

		// Consider candidate spots.
		for (int i = 0, n = s_anchor_points.size(); i < n; i++)
		{
			const pointi&	p = s_anchor_points[i];
			recti	r(p.m_x, p.m_x + width, p.m_y, p.m_y + height);

			// Is this spot any good?
			if (is_rect_available(r))
			{
				// Good spot.  Scan left to see if we can tighten it up.
				while (r.m_x_min > 0)
				{
					recti	r2(r.m_x_min - 1, r.m_x_min - 1 + width, r.m_y_min, r.m_y_min + height);
					if (is_rect_available(r2))
					{
						// Shift left.
						r = r2;
					}
					else
					{
						// Not clear; stop scanning.
						break;
					}
				}

				// Mark our covered rect; remove newly covered anchors.
				add_cover_rect(r);

				// Found our desired spot.  Add new
				// candidate points to the anchor list.
				add_anchor_point(pointi(r.m_x_min, r.m_y_max));	// lower-left
				add_anchor_point(pointi(r.m_x_max, r.m_y_min));	// upper-right

				*px = r.m_x_min;
				*py = r.m_y_min;

				return true;
			}
		}

		// Couldn't find a good spot.
		return false;
	}

	static void	software_trapezoid(
		float y0, float y1,
		float xl0, float xl1,
		float xr0, float xr1)
	// Fill the specified trapezoid in the software output buffer.
	{
		assert(s_render_buffer);

		int	iy0 = (int) ceilf(y0);
		int	iy1 = (int) ceilf(y1);
		float	dy = y1 - y0;

		for (int y = iy0; y < iy1; y++)
		{
			if (y < 0) 
			{
				continue;
			}
			if (y >= s_rendering_box)
			{
				return;
			}

			float	f = (y - y0) / dy;
			int	xl = (int) ceilf(flerp(xl0, xl1, f));
			int	xr = (int) ceilf(flerp(xr0, xr1, f));
			
			xl = iclamp(xl, 0, s_rendering_box - 1);
			xr = iclamp(xr, 0, s_rendering_box - 1);

			if (xr > xl)
			{
				memset(s_render_buffer + y * s_rendering_box + xl, 255, xr - xl);
			}
		}
	}

	struct draw_into_software_buffer : tesselate::trapezoid_accepter
	// A trapezoid accepter that does B&W rendering into our
	// software buffer.
	{
		// Overrides from trapezoid_accepter
		virtual void	accept_trapezoid(int style, const tesselate::trapezoid& tr)
		{
			// Transform the coords.
			float	x_scale = s_render_matrix.m_[0][0];
			float	y_scale = s_render_matrix.m_[1][1];
			float	x_offset = s_render_matrix.m_[0][2];
			float	y_offset = s_render_matrix.m_[1][2];

			float	y0 = tr.m_y0 * y_scale + y_offset;
			float	y1 = tr.m_y1 * y_scale + y_offset;
			float	lx0 = tr.m_lx0 * x_scale + x_offset;
			float	lx1 = tr.m_lx1 * x_scale + x_offset;
			float	rx0 = tr.m_rx0 * x_scale + x_offset;
			float	rx1 = tr.m_rx1 * x_scale + x_offset;

			// Draw into the software buffer.
			software_trapezoid(y0, y1, lx0, lx1, rx0, rx1);
		}

		virtual void	accept_line_strip(int style, const point coords[], int coord_count)
		{
			assert(0);	// Shape glyphs should not contain lines.
		}

		virtual void end_shape() {}
	};

	// for debugging
	static void print_glyph(Uint8* data, int w, int h)
	{
		static int s_image_sequence = 0;
		char fi[256];
		sprintf(fi, "/image%d.ppm", s_image_sequence++);
		FILE*	fp = fopen(fi, "wb");
		if (fp)
		{
			fprintf(fp, "P6\n%d %d\n255\n", w, h);
			for (int i = 0; i < w * h; i++)
			{
				fputc(data[i], fp);
				fputc(data[i], fp);
				fputc(data[i], fp);
			}
			fclose(fp);
		}
	}

	static void antialias()
	{
		memset(s_image_buffer, 0, GLYPH_CACHE_TEXTURE_SIZE * GLYPH_CACHE_TEXTURE_SIZE);

		// Resample.  Simple average 2x2 --> 1
		int new_h = s_rendering_box >> 1;
		int new_w = s_rendering_box >> 1;
		for (int j = 0; j < new_h; j++) 
		{
			Uint8*	out = ((Uint8*) s_image_buffer) + j * GLYPH_CACHE_TEXTURE_SIZE;
			Uint8*	in = ((Uint8*) s_render_buffer) + (j << 1) * s_rendering_box;
			for (int i = 0; i < new_w; i++)
			{
				int	sum;
				sum = (*in + *(in + 1) + *(in + s_rendering_box) + *(in + 1 + s_rendering_box));
				*out = sum >> 2;
				out++;
				in += 2;
			}
		}
	}

	static void	render_glyph(glyph_entity* ge, const shape_character_def* sh, int fontsize)
	// Render the given outline shape into a antialiased bitmap
	{
		assert(ge);
		assert(sh);
		assert(s_render_buffer);

		//
		// Tesselate and render the shape into a software buffer.
		//

		// Clear the render output to 0.
		memset(s_render_buffer, 0, s_rendering_box * s_rendering_box);
		int version = sh->get_player()->get_root()->get_movie_version();

		float scale = OVERSAMPLE_FACTOR * fontsize / 1024.0f;		// the EM square is 1024 x 1024
 
		// Look at glyph bounds; adjust origin to make sure
		// the shape will fit in our output in left-top corner.
		rect	glyph_bounds;
		sh->compute_bound(&glyph_bounds);
		float offset_x = - glyph_bounds.m_x_min;
		float offset_y = - glyph_bounds.m_y_min;

		s_render_matrix.set_identity();

		s_render_matrix.concatenate_scale(version > 7 ? scale / 20.0f : scale);
		s_render_matrix.concatenate_translation(offset_x, offset_y);

		// Tesselate & draw the shape.
		draw_into_software_buffer	accepter;
		sh->tesselate( (float) s_rendering_box / (float) s_glyph_render_size * 0.5f, &accepter);

		antialias();

	//	print_glyph(s_render_buffer, s_rendering_box, s_rendering_box);
	//	print_glyph(s_image_buffer, GLYPH_CACHE_TEXTURE_SIZE, GLYPH_CACHE_TEXTURE_SIZE);

		ge->m_bi = render::create_bitmap_info_alpha(GLYPH_CACHE_TEXTURE_SIZE, GLYPH_CACHE_TEXTURE_SIZE, s_image_buffer);

		// Fill in rendered_glyph_info.
		scale = version > 7 ? 1024 * 20 : 1024;
		scale /= fontsize / (float) GLYPH_CACHE_TEXTURE_SIZE;
		ge->m_bounds.m_x_min = offset_x / scale;
		ge->m_bounds.m_x_max = 1;
		ge->m_bounds.m_y_min = offset_y / scale;
		ge->m_bounds.m_y_max = 1;
	}

	glyph_provider_tu::glyph_provider_tu()
	{
		s_render_buffer = new Uint8[s_rendering_box * s_rendering_box];
		s_image_buffer = new Uint8[GLYPH_CACHE_TEXTURE_SIZE * GLYPH_CACHE_TEXTURE_SIZE];
	}

	glyph_provider_tu::~glyph_provider_tu()
	{
		delete s_render_buffer;
		delete s_image_buffer;

		for (stringi_hash<glyph_array*>::iterator it = m_glyph.begin(); it != m_glyph.end(); ++it)
		{
			delete it->second;
		}
	}

	bitmap_info* glyph_provider_tu::get_char_image(character_def* shape_glyph, Uint16 xcode, 
			const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance)
	{
		shape_character_def*	sh = cast_to<shape_character_def>(shape_glyph);
		if (sh == NULL)
		{
			return NULL;
		}

		int flags = is_bold ? 2 : 0;
		flags |= is_italic ? 1 : 0;
		int glyph_code = (Uint8) fontsize << 24 | (Uint8) flags << 16 | xcode;

		glyph_array* ga;
		glyph_entity ge;
		if (m_glyph.get(fontname, &ga))
		{
			if (ga->get(glyph_code, &ge))
			{
				if (bounds)
				{
					*bounds = ge.m_bounds;
				}
				if (advance)
				{
					*advance = ge.m_advance;
				}

			//	printf("glyph_provider_tu: finded %08X: %s\n", glyph_code, fontname.c_str());
				return ge.m_bi;
			}
		}
		else
		{
			// add new font
		//	printf("glyph_provider_tu: added font %s\n", fontname.c_str());
			ga = new glyph_array();
			m_glyph[fontname] = ga;
		}

		render_glyph(&ge, sh, fontsize);

		// store glyph
		ge.m_advance = 0;
		ga->add(glyph_code, ge);

		if (bounds)
		{
			*bounds = ge.m_bounds;
		}

		if (advance)
		{
			*advance = ge.m_advance;
		}

	//	printf("glyph_provider_tu: added %08X: %s\n", glyph_code, fontname.c_str());
		return ge.m_bi;
	}

	glyph_provider*	create_glyph_provider_tu()
	{
		return new glyph_provider_tu();
	}

}	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
