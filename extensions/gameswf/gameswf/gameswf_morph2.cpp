// gameswf_morph2.cpp
// -- Thatcher Ulrich <tu@tulrich.com>, Mike Shaver <shaver@off.net> 2003, Vitalij Alexeev <tishka92@mail.ru> 2004.

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Loading and rendering of morphing shapes using gameswf_shape.

#include "gameswf_morph2.h"
#include "gameswf_stream.h"
#include "gameswf_movie_def.h"


namespace gameswf
{
	morph2_character_def::morph2_character_def(player* player) :
		shape_character_def(player),
		m_shape1(player),
		m_shape2(player),
		m_offset(0),
		m_last_ratio(-1.0f), 
		m_mesh(0)
	{
	}


	morph2_character_def::~morph2_character_def()
	{
	}

	void	morph2_character_def::display(character* inst)
	{
		int i;
		float ratio = inst->m_ratio;

		// bounds
		rect	new_bound;
		new_bound.set_lerp(m_shape1.get_bound_local(), m_shape2.get_bound_local(), ratio);
		set_bound(new_bound);

		// fill styles
		for (i=0; i < m_fill_styles.size(); i++)
		{
			fill_style* fs = &m_fill_styles[i];

			const fill_style& fs1 = m_shape1.get_fill_styles()[i];
			const fill_style& fs2 = m_shape2.get_fill_styles()[i];

			fs->set_lerp(fs1, fs2, ratio);
		}

		// line styles
		for (i = 0; i < m_line_styles.size(); i++)
		{
			line_style& ls = m_line_styles[i];
			const line_style& ls1 = m_shape1.get_line_styles()[i];
			const line_style& ls2 = m_shape2.get_line_styles()[i];
			ls.m_width = (Uint16)frnd(flerp(ls1.get_width(), ls2.get_width(), ratio));
			ls.m_color.set_lerp(ls1.get_color(), ls2.get_color(), ratio);
		}

		// shape
		int k = 0, n = 0;
		for (i = 0; i < m_paths.size(); i++) 
		{
			path& p = m_paths[i];
			const path& p1 = m_shape1.get_paths()[i];

			// Swap fill styles -- for some reason, morph
			// shapes seem to be defined with their fill
			// styles backwards!
			p.m_fill0 = p1.m_fill1;
			p.m_fill1 = p1.m_fill0;

			p.m_line = p1.m_line;

			p.m_ax = flerp(p1.m_ax, m_shape2.get_paths()[n].m_ax, ratio);
			p.m_ay = flerp(p1.m_ay, m_shape2.get_paths()[n].m_ay, ratio);
      
			//  edges;
			int len = p1.m_edges.size();
			p.m_edges.resize(len);

			for (int j = 0; j < p.m_edges.size(); j++)
			{
				p.m_edges[j].m_cx = flerp(p1.m_edges[j].m_cx, m_shape2.get_paths()[n].m_edges[k].m_cx, ratio);
				p.m_edges[j].m_cy = flerp(p1.m_edges[j].m_cy, m_shape2.get_paths()[n].m_edges[k].m_cy, ratio);
				p.m_edges[j].m_ax = flerp(p1.m_edges[j].m_ax, m_shape2.get_paths()[n].m_edges[k].m_ax, ratio);
				p.m_edges[j].m_ay = flerp(p1.m_edges[j].m_ay, m_shape2.get_paths()[n].m_edges[k].m_ay, ratio);
				k++;
				if (m_shape2.get_paths()[n].m_edges.size() <= k)
				{
					k = 0;
					n++;
				}
			}
		}
    
		//  display

		matrix mat = inst->get_world_matrix();
		cxform cx = inst->get_world_cxform();
		float max_error = 20.0f / mat.get_max_scale() /	inst->get_parent()->get_pixel_scale();

		if (ratio != m_last_ratio)
		{
			delete m_mesh;
			m_last_ratio = ratio;
			m_mesh = new mesh_set(this, max_error * 0.75f);
		}
		m_mesh->display(mat, cx, m_fill_styles, m_line_styles, render_handler::BLEND_NORMAL);
	}

  
	void	morph2_character_def::read(stream* in, int tag_type, bool with_style, movie_definition_sub* md)
	{
		UNUSED(with_style);

		rect	bound1, bound2;
		bound1.read(in);
		bound2.read(in);
		m_shape1.set_bound(bound1);
		m_shape2.set_bound(bound2);

		if(tag_type == 84)
		{
			rect edge_bound1, edge_bound2;

			edge_bound1.read(in);
			edge_bound2.read(in);

			int reserved = in->read_uint(6);
			assert(reserved == 0);

			m_uses_nonscaling_strokes = in->read_uint(1) == 1;
			m_uses_scaling_strokes = in->read_uint(1) == 1;
		}

		m_offset = in->read_u32();

		int fill_style_count = in->read_variable_count();
		int i;
		for (i = 0; i < fill_style_count; i++)
		{
			fill_style fs1, fs2;

			fs1.m_type = in->read_u8();
			fs2.m_type = fs1.m_type;

			IF_VERBOSE_PARSE(log_msg("morph fill style type = 0x%X\n", fs1.m_type));

			if (fs1.m_type == 0x00)
			{
				fs1.m_color.read_rgba(in);
				fs2.m_color.read_rgba(in);

				IF_VERBOSE_PARSE(log_msg("morph fill style begin color: "); fs1.m_color.print());
				IF_VERBOSE_PARSE(log_msg("morph fill style end color: "); fs2.m_color.print());
			}
			else
			if (fs1.m_type == 0x10 || fs1.m_type == 0x12)
			{
				matrix	input_matrix1, input_matrix2;

				input_matrix1.read(in);
				input_matrix2.read(in);

				fs1.m_gradient_matrix.set_identity();
				fs2.m_gradient_matrix.set_identity();
				if (fs1.m_type == 0x10)
				{
					fs1.m_gradient_matrix.concatenate_translation(128.f, 0.f);
					fs1.m_gradient_matrix.concatenate_scale(1.0f / 128.0f);
					fs2.m_gradient_matrix.concatenate_translation(128.f, 0.f);
					fs2.m_gradient_matrix.concatenate_scale(1.0f / 128.0f);
				}
				else
				{
					fs1.m_gradient_matrix.concatenate_translation(32.f, 32.f);
					fs1.m_gradient_matrix.concatenate_scale(1.0f / 512.0f);
					fs2.m_gradient_matrix.concatenate_translation(32.f, 32.f);
					fs2.m_gradient_matrix.concatenate_scale(1.0f / 512.0f);
				}

				matrix	m1, m2;
				m1.set_inverse(input_matrix1);
				fs1.m_gradient_matrix.concatenate(m1);
				m2.set_inverse(input_matrix2);
				fs2.m_gradient_matrix.concatenate(m2);

				// GRADIENT
				int	num_gradients = in->read_u8();
				assert(num_gradients >= 1 && num_gradients <= 8);

				fs1.m_gradients.resize(num_gradients);
				fs2.m_gradients.resize(num_gradients);

				for (int j = 0; j < num_gradients; j++)
				{
					fs1.m_gradients[j].read(in, tag_type);
					fs2.m_gradients[j].read(in, tag_type);
				}

				IF_VERBOSE_PARSE(log_msg("morph fsr: num_gradients = %d\n", num_gradients));

				// @@ hack.
				if (num_gradients > 0)
				{
					fs1.m_color = fs1.m_gradients[0].m_color;
					fs2.m_color = fs2.m_gradients[0].m_color;
				}
			}
			else
			if (fs1.m_type == 0x40 || fs1.m_type == 0x41)
			{

				int	bitmap_char_id = in->read_u16();
				IF_VERBOSE_PARSE(log_msg("morph fsr bitmap_char = %d\n", bitmap_char_id));

				// Look up the bitmap character.
				fs1.m_bitmap_character = md->get_bitmap_character(bitmap_char_id);
				fs2.m_bitmap_character = fs1.m_bitmap_character;

				matrix	m1, m2;
				m1.read(in);
				m2.read(in);

				// For some reason, it looks like they store the inverse of the
				// TWIPS-to-texcoords matrix.
				fs1.m_bitmap_matrix.set_inverse(m1);
				fs2.m_bitmap_matrix.set_inverse(m2);
			}
			else
			{
				assert(0);
			}

			m_shape1.m_fill_styles.push_back(fs1);
			m_shape2.m_fill_styles.push_back(fs2);
		}

		int line_style_count = in->read_variable_count();

		if (tag_type == 46)
		{
			for (i = 0; i < line_style_count; i++)
			{
				line_style ls1, ls2;
				ls1.m_width = in->read_u16();
				ls2.m_width = in->read_u16();
				ls1.m_color.read(in, tag_type);
				ls2.m_color.read(in, tag_type);
				m_shape1.m_line_styles.push_back(ls1);
				m_shape2.m_line_styles.push_back(ls2);
			}
		}
		else
		if (tag_type == 84)
		{
			for (i = 0; i < line_style_count; i++)
			{
				line_style ls1, ls2;

				ls1.m_width = in->read_u16();
				ls2.m_width = in->read_u16();
				int start_cap_style = in->read_uint(2);
				int join_style = in->read_uint(2);
				int has_fill_flag = in->read_uint(1);
				int no_h_scale_flag = in->read_uint(1);
				int no_v_scale_flag = in->read_uint(1);
				int pixel_hinting_flag = in->read_uint(1);
				int reserved = in->read_uint(1); assert( reserved == 0 );
				int no_close = in->read_uint(1);
				int end_cap_style = in->read_uint(2);
				UNUSED(start_cap_style);
				UNUSED(no_h_scale_flag);
				UNUSED(no_v_scale_flag);
				UNUSED(pixel_hinting_flag);
				UNUSED(no_close);
				UNUSED(end_cap_style);

				if (join_style == 2)
				{
					int miter_limit_factor_fixed = in->read_u16(); // 8.8 fixed point format
					float miter_limit_factor = (float) miter_limit_factor_fixed / 256.0f;
					UNUSED(miter_limit_factor);
				}

				if (has_fill_flag == 0)
				{
					rgba start_color;
					start_color.read_rgba(in);
					rgba end_color;
					end_color.read_rgba(in);
				}
				else
				{
					// todo factorize fill_style read
					fill_style fs1, fs2;

					fs1.m_type = in->read_u8();
					fs2.m_type = fs1.m_type;

					IF_VERBOSE_PARSE(log_msg("morph fill style type = 0x%X\n", fs1.m_type));

					if (fs1.m_type == 0x00)
					{
						fs1.m_color.read_rgba(in);
						fs2.m_color.read_rgba(in);

						IF_VERBOSE_PARSE(log_msg("morph fill style begin color: "); fs1.m_color.print());
						IF_VERBOSE_PARSE(log_msg("morph fill style end color: "); fs2.m_color.print());
					}
					else
					if (fs1.m_type == 0x10 || fs1.m_type == 0x12)
					{
						matrix	input_matrix1, input_matrix2;

						input_matrix1.read(in);
						input_matrix2.read(in);

						fs1.m_gradient_matrix.set_identity();
						fs2.m_gradient_matrix.set_identity();
						if (fs1.m_type == 0x10)
						{
							fs1.m_gradient_matrix.concatenate_translation(128.f, 0.f);
							fs1.m_gradient_matrix.concatenate_scale(1.0f / 128.0f);
							fs2.m_gradient_matrix.concatenate_translation(128.f, 0.f);
							fs2.m_gradient_matrix.concatenate_scale(1.0f / 128.0f);
						}
						else
						{
							fs1.m_gradient_matrix.concatenate_translation(32.f, 32.f);
							fs1.m_gradient_matrix.concatenate_scale(1.0f / 512.0f);
							fs2.m_gradient_matrix.concatenate_translation(32.f, 32.f);
							fs2.m_gradient_matrix.concatenate_scale(1.0f / 512.0f);
						}

						matrix	m1, m2;
						m1.set_inverse(input_matrix1);
						fs1.m_gradient_matrix.concatenate(m1);
						m2.set_inverse(input_matrix2);
						fs2.m_gradient_matrix.concatenate(m2);

						// GRADIENT
						int	num_gradients = in->read_u8();
						assert(num_gradients >= 1 && num_gradients <= 8);

						fs1.m_gradients.resize(num_gradients);
						fs2.m_gradients.resize(num_gradients);

						for (int j = 0; j < num_gradients; j++)
						{
							fs1.m_gradients[j].read(in, tag_type);
							fs2.m_gradients[j].read(in, tag_type);
						}

						IF_VERBOSE_PARSE(log_msg("morph fsr: num_gradients = %d\n", num_gradients));

						// @@ hack.
						if (num_gradients > 0)
						{
							fs1.m_color = fs1.m_gradients[0].m_color;
							fs2.m_color = fs2.m_gradients[0].m_color;
						}
					}
					else
					if (fs1.m_type == 0x40 || fs1.m_type == 0x41)
					{

						int	bitmap_char_id = in->read_u16();
						IF_VERBOSE_PARSE(log_msg("morph fsr bitmap_char = %d\n", bitmap_char_id));

						// Look up the bitmap character.
						fs1.m_bitmap_character = md->get_bitmap_character(bitmap_char_id);
						fs2.m_bitmap_character = fs1.m_bitmap_character;

						matrix	m1, m2;
						m1.read(in);
						m2.read(in);
					}
					else
					{
						assert(0);
					}
				}

				m_shape1.m_line_styles.push_back(ls1);
				m_shape2.m_line_styles.push_back(ls2);
			}
		}
		else
		{
			assert(0);
		}

		m_shape1.read(in, tag_type, false, md);
		in->align();
		m_shape2.read(in, tag_type, false, md);

		assert(m_shape1.m_fill_styles.size() == m_shape2.m_fill_styles.size());
		assert(m_shape1.m_fill_styles.size() == fill_style_count);
		assert(m_shape1.m_line_styles.size() == m_shape2.m_line_styles.size());
		assert(m_shape1.m_line_styles.size() == line_style_count);

		// setup array size
		m_fill_styles.resize(m_shape1.m_fill_styles.size());
		for (i = 0; i < m_fill_styles.size(); i++)
		{
			fill_style& fs = m_fill_styles[i];
			fill_style& fs1 = m_shape1.m_fill_styles[i];
			fs.m_gradients.resize(fs1.m_gradients.size());
		}
		m_line_styles.resize(m_shape1.m_line_styles.size());
		m_paths.resize(m_shape1.m_paths.size());

		int edges_count1 = 0;
		for (i = 0; i < m_paths.size(); i++)
		{
			path& p = m_paths[i];
			path& p1 = m_shape1.m_paths[i];
			int len = p1.m_edges.size();
			edges_count1 += len;
			p.m_edges.resize(len);
		}

		int edges_count2 = 0;
		for (i = 0; i < m_shape2.m_paths.size(); i++)
		{
			path& p2 = m_shape2.m_paths[i];
			int len = p2.m_edges.size();
			edges_count2 += len;
		}
		assert(edges_count1 == edges_count2);
	}

}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
