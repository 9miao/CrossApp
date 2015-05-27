// gameswf_styles.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Fill and line style types.


#include "gameswf_styles.h"
#include "gameswf_impl.h"
#include "gameswf_log.h"
#include "gameswf_render.h"
#include "gameswf_stream.h"
#include "gameswf_movie_def.h"


namespace gameswf
{
	//
	// gradient_record
	//

	gradient_record::gradient_record() :
		m_ratio(0)
	{
	}


	void	gradient_record::read(stream* in, int tag_type)
	{
		m_ratio = in->read_u8();
		m_color.read(in, tag_type);
	}


	//
	// fill_style
	//


	fill_style::fill_style() :
		m_type(0),
		m_gradient_bitmap_info(0),
		m_bitmap_character(0)
	{
		assert(m_gradients.size() == 0);
	}


	fill_style::~fill_style()
	{
	}

	void	fill_style::read(stream* in, int tag_type, movie_definition_sub* md)
	{
		m_type = in->read_u8();

		IF_VERBOSE_PARSE(log_msg("  fill_style read type = 0x%X\n", m_type));

		if (m_type == 0x00)
		{
			// 0x00: solid fill
			if (tag_type <= 22) {
				m_color.read_rgb(in);
			} else {
				m_color.read_rgba(in);
			}

			IF_VERBOSE_PARSE(log_msg("  color: ");
					 m_color.print());
		}
		else if (m_type == 0x10 || m_type == 0x12)
		{
			// 0x10: linear gradient fill
			// 0x12: radial gradient fill

			matrix	input_matrix;
			input_matrix.read(in);

			if (m_type == 0x10)
			{
				m_gradient_matrix.set_identity();
				m_gradient_matrix.concatenate_translation(128.f, 0.f);
				m_gradient_matrix.concatenate_scale(1.0f / 128.0f);
			}
			else 
			{
				m_gradient_matrix.set_identity();
				m_gradient_matrix.concatenate_translation(32.f, 32.f);
				m_gradient_matrix.concatenate_scale(1.0f / 512.0f);
			}


			matrix	m;
			m.set_inverse(input_matrix);
			m_gradient_matrix.concatenate(m);
				
			// GRADIENT

			// 'spread_mode' and 'interpolation_mode' are not used for now
			// and so they are commented out
//			in->align();
//			int spread_mode = in->read_uint(2);
//			int interpolation_mode = in->read_uint(2);
//			int num_gradients = in->read_uint(4);

			// SWF 8 and later supports up to 15 gradient control points
			int num_gradients = in->read_u8() & 0x0F;

			m_gradients.resize(num_gradients);
			for (int i = 0; i < num_gradients; i++)
			{
				m_gradients[i].read(in, tag_type);
			}

			IF_VERBOSE_PARSE(log_msg("  gradients: num_gradients = %d\n", num_gradients));

			// @@ hack.
			if (num_gradients > 0)
			{
				m_color = m_gradients[0].m_color;
			}

			if (md->get_create_bitmaps() == DO_LOAD_BITMAPS)
			{
				m_gradient_bitmap_info = create_gradient_bitmap();
			}
			else
			{
				m_gradient_bitmap_info = render::create_bitmap_info_empty();
			}

			// Make sure our movie_def_impl knows about this bitmap.
			md->add_bitmap_info(m_gradient_bitmap_info.get_ptr());
		}
		else
		if (m_type == 0x13)
		{
			
			assert( tag_type == 83 );
			// focal gradient fill, Flash 8
			// parsed but not implemented yet

			matrix	input_matrix;
			input_matrix.read(in);

			// 'spread_mode' and 'interpolation_mode' are not used for now
			// and so they are commented out
//			in->align();
//			int spread_mode = in->read_uint(2);
//			int interpolation_mode = in->read_uint(2);
//			int gradient_count = in->read_uint(4);

			int num_gradients = in->read_u8() & 0x0F;

			for (int i = 0; i < num_gradients; i++)
			{
				int ratio = in->read_u8();
				UNUSED(ratio);
				rgba color;
				color.read_rgba(in);
			}

			in->read_u16(); //Fixed 8
		}
		else
		if (m_type >= 0x40 && m_type <= 0x43)
		{
			// 0x40: tiled bitmap fill
			// 0x41: clipped bitmap fill
			// 0x42: non-smoothed repeating bitmap, Flash 8, TODO
			// 0x43: non-smoothed clipped bitmap, Flash 8, TODO

			int	bitmap_char_id = in->read_u16();
			IF_VERBOSE_PARSE(log_msg("  bitmap_char = %d\n", bitmap_char_id));

			// Look up the bitmap character.
			m_bitmap_character = md->get_bitmap_character(bitmap_char_id);

			matrix	m;
			m.read(in);

			// For some reason, it looks like they store the inverse of the
			// TWIPS-to-texcoords matrix.
			m_bitmap_matrix.set_inverse(m);
			IF_VERBOSE_PARSE(m_bitmap_matrix.print());
		}
		else
		{
			assert(0);
		}
	}


	rgba	fill_style::sample_gradient(int ratio) const
	// Return the color at the specified ratio into our gradient.
	// Ratio is in [0, 255].
	{
		assert(ratio >= 0 && ratio <= 255);
		assert(m_type == 0x10 || m_type == 0x12);
		assert(m_gradients.size() > 0);

		if (ratio < m_gradients[0].m_ratio)
			return m_gradients[0].m_color;		
		
		for (int i = 1; i < m_gradients.size(); i++)
		{
			if (m_gradients[i].m_ratio >= ratio)
			{
				const gradient_record& gr0 = m_gradients[i - 1];
				const gradient_record& gr1 = m_gradients[i];
				float	f = 0.0f;
				if (gr0.m_ratio != gr1.m_ratio)
				{
					f = (ratio - gr0.m_ratio) / float(gr1.m_ratio - gr0.m_ratio);
				}

				rgba	result;
				result.set_lerp(m_gradients[i - 1].m_color, m_gradients[i].m_color, f);
				return result;
			}
		}
		return m_gradients.back().m_color;
	}

	bitmap_info*	fill_style::create_gradient_bitmap() const
	// Make a bitmap_info* corresponding to our gradient.
	// We can use this to set the gradient fill style.
	{
		assert(m_type == 0x10 || m_type == 0x12);

		image::rgba*	im = NULL;

		if (m_type == 0x10)
		{
			// Linear gradient.
			im = image::create_rgba(256, 1);

			for (int i = 0; i < im->m_width; i++)
			{
				rgba	sample = sample_gradient(i);
				im->set_pixel(i, 0, sample.m_r, sample.m_g, sample.m_b, sample.m_a);
			}
		}
		else if (m_type == 0x12)
		{
			// Radial gradient.
			im = image::create_rgba(64, 64);

			for (int j = 0; j < im->m_height; j++)
			{
				for (int i = 0; i < im->m_width; i++)
				{
					float	radius = (im->m_height - 1) / 2.0f;
					float	y = (j - radius) / radius;
					float	x = (i - radius) / radius;
					int	ratio = (int) floorf(255.5f * sqrt(x * x + y * y));
					if (ratio > 255)
					{
						ratio = 255;
					}
					rgba	sample = sample_gradient( ratio );
					im->set_pixel(i, j, sample.m_r, sample.m_g, sample.m_b, sample.m_a);
				}
			}
		}

		bitmap_info*	bi = render::create_bitmap_info_rgba(im);
		delete im;

		return bi;
	}


	void	fill_style::apply(int fill_side, float ratio, render_handler::bitmap_blend_mode bm) const
	// Push our style parameters into the renderer.
	{
		UNUSED(ratio);
		if (m_type == 0x00)
		{
			// 0x00: solid fill
			render::fill_style_color(fill_side, m_color);
		}
		else if (m_type == 0x10 || m_type == 0x12)
		{
			// 0x10: linear gradient fill
			// 0x12: radial gradient fill

			if (m_gradient_bitmap_info == NULL)
			{
				// This can happen when morphing gradient styles.
				// assert(morphing???);
				// log an error?
				fill_style*	this_non_const = const_cast<fill_style*>(this);
				this_non_const->m_gradient_bitmap_info = create_gradient_bitmap();
			}

			if (m_gradient_bitmap_info != NULL)
			{
				render::fill_style_bitmap( fill_side, m_gradient_bitmap_info.get_ptr(), m_gradient_matrix,
					render_handler::WRAP_CLAMP, bm);
			}
		}
		else if (m_type >= 0x40 || m_type <= 0x43)
		{
			// bitmap fill (either tiled or clipped)
			bitmap_info*	bi = NULL;
			if (m_bitmap_character != NULL)
			{
				bi = m_bitmap_character->get_bitmap_info();
	
				if (bi != NULL)
				{
					switch (m_type)
					{
						case 0x40 :
						case 0x42 :
							render::fill_style_bitmap(fill_side, bi, m_bitmap_matrix,  render_handler::WRAP_REPEAT, bm);
							break;

						case 0x41 :
						case 0x43 :
							render::fill_style_bitmap(fill_side, bi, m_bitmap_matrix, render_handler::WRAP_CLAMP, bm);
							break;

						default:
							assert(0);
					}
				}
			}
		}
		else
		{
			assert(0);
		}
	}


	void	fill_style::set_lerp(const fill_style& a, const fill_style& b, float t)
	// Sets this style to a blend of a and b.  t = [0,1]
	{
		assert(t >= 0 && t <= 1);

		// fill style type
		m_type = a.get_type();
		assert(m_type == b.get_type());

		// fill style color
		m_color.set_lerp(a.get_color(), b.get_color(), t);

		// fill style gradient matrix
		//
		// @@ TODO morphed gradients don't come out exactly
		// right; they shift around some.  Not sure where the
		// problem is.
		m_gradient_matrix.set_lerp(a.m_gradient_matrix, b.m_gradient_matrix, t);

		// fill style gradients
		assert(m_gradients.size() == a.m_gradients.size());
		assert(m_gradients.size() == b.m_gradients.size());
		for (int j=0; j < m_gradients.size(); j++)
		{
			m_gradients[j].m_ratio =
				(Uint8) frnd(
					flerp(a.m_gradients[j].m_ratio, b.m_gradients[j].m_ratio, t)
					);
			m_gradients[j].m_color.set_lerp(a.m_gradients[j].m_color, b.m_gradients[j].m_color, t);
		}
		m_gradient_bitmap_info = NULL;

		// fill style bitmap ID
		m_bitmap_character = a.m_bitmap_character;
		assert(m_bitmap_character == b.m_bitmap_character);

		// fill style bitmap matrix
		m_bitmap_matrix.set_lerp(a.m_bitmap_matrix, b.m_bitmap_matrix, t);
	}


	//
	// line_style
	//

	
	line_style::line_style()
		:
		m_width(0),
		m_start_capstyle(0),
		m_joinstyle(0),
		m_has_fill_flag(false),
		m_no_hscale_flag(false),
		m_no_vscale_flag(false),
		m_pixelhinting_flag(false),
		m_noclose(false),
		m_end_capstyle(0),
		m_miter_limit_factor(0)
	{
	}


	void	line_style::read(stream* in, int tag_type, movie_definition_sub* m)
	{
		m_width = in->read_u16();
		if (tag_type == 83)	// SHAPE 4
		{
			// 0 = Round cap
			// 1 = No cap
			// 2 = Square cap
			m_start_capstyle = in->read_uint(2);

			// 0 = Round join
			// 1 = Bevel join
			// 2 = Miter join
			m_joinstyle = in->read_uint(2);

			// If 0, uses Color field.
			m_has_fill_flag  = in->read_uint(1) == 1 ? true : false;

			// If 1, stroke thickness will not scale if the object is scaled horizontally.
			m_no_hscale_flag = in->read_uint(1) == 1 ? true : false;

			// If 1, stroke thickness will not scale if the object is scaled vertically.
			m_no_vscale_flag = in->read_uint(1) == 1 ? true : false;

			// If 1, all anchors will be aligned to full pixels.
			m_pixelhinting_flag = in->read_uint(1) == 1 ? true : false;

			in->read_uint(5); // Reserved UB[5] Must be 0.

			// If 1, stroke will not be closed if the stroke’s last point
			// matches its first point. Flash Player will apply caps instead of a join.
			m_noclose = in->read_uint(1) == 1 ? true : false;

			// 0 = Round cap
			// 1 = No cap
			// 2 = Square cap
			m_end_capstyle = in->read_uint(2);

			if (m_joinstyle == 2)
			{
				// Miter limit factor is an 8.8 fixed-point value
				m_miter_limit_factor = in->read_u16();
			}

			if (m_has_fill_flag)
			{
				m_fill_style.read(in, tag_type, m);
			}
			else
			{
				m_color.read(in, tag_type);
			}
		}
		else
		{
			m_color.read(in, tag_type);
		}
	}


	void	line_style::apply(float ratio) const
	{
		UNUSED(ratio);
		render::line_style_color(m_color);
		render::line_style_width(m_width);
	}

}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
