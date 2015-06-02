// gameswf_font.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A font type for gameswf.


#include "gameswf_font.h"
#include "gameswf_stream.h"
#include "gameswf_impl.h"
#include "gameswf_log.h"
#include "gameswf_shape.h"
#include "gameswf_movie_def.h"
#include "gameswf_freetype.h"
#include "../base/tu_file.h"


namespace gameswf
{
	font::font(player* player) :
		character_def(player),
		m_fontname("Times New Roman"),	// default
		m_owning_movie(NULL),
		m_unicode_chars(false),
		m_shift_jis_chars(false),
		m_ansi_chars(true),
		m_is_italic(false),
		m_is_bold(false),
		m_wide_codes(false),
		m_ascent(0.0f),
		m_descent(0.0f),
		m_leading(0.0f),
		m_table_hint(0)
	{
	}

	font::~font()
	{
		m_glyphs.resize(0);
	}

	void	font::read(stream* in, int tag_type, movie_definition_sub* m)
	{
		assert(tag_type == 10 || tag_type == 48 || tag_type == 75);

		// No add_ref() here, to avoid cycle.  m_owning_movie is our owner, so it has a ref to us.
		m_owning_movie = m;

		if (tag_type == 10)
		{
			IF_VERBOSE_PARSE(log_msg("reading DefineFont\n"));

			int	table_base = in->get_position();

			// Read the glyph offsets.  Offsets
			// are measured from the start of the
			// offset table.
			array<int>	offsets;
			offsets.push_back(in->read_u16());
			IF_VERBOSE_PARSE(log_msg("offset[0] = %d\n", offsets[0]));
			int	count = offsets[0] >> 1;
			for (int i = 1; i < count; i++)
			{
				offsets.push_back(in->read_u16());
				IF_VERBOSE_PARSE(log_msg("offset[%d] = %d\n", i, offsets[i]));
			}

			m_glyphs.resize(count);
			if (m->get_create_font_shapes() == DO_LOAD_FONT_SHAPES)
			{
				// Read the glyph shapes.
				{for (int i = 0; i < count; i++)
				{
					// Seek to the start of the shape data.
					int	new_pos = table_base + offsets[i];
					in->set_position(new_pos);

					// Create & read the shape.
					shape_character_def* s = new shape_character_def(m->get_player());
					s->read(in, 2, false, m);

					m_glyphs[i] = s;
				}}
			}
		}
		else if (tag_type == 48 || tag_type == 75)
		{
			IF_VERBOSE_PARSE(
				log_msg("DefineFont%d tag\n", tag_type == 48 ? 2 : 3)
			);

			bool	has_layout = (in->read_uint(1) != 0);
			m_shift_jis_chars = (in->read_uint(1) != 0);
			m_unicode_chars = (in->read_uint(1) != 0);
			m_ansi_chars = (in->read_uint(1) != 0);
			bool	wide_offsets = (in->read_uint(1) != 0);
			m_wide_codes = (in->read_uint(1) != 0);
			m_is_italic = (in->read_uint(1) != 0);
			m_is_bold = (in->read_uint(1) != 0);

			//Uint8	reserved = 
			in->read_u8();

			in->read_string_with_length(&m_fontname);
			int	glyph_count = in->read_u16();
			int	table_base = in->get_position();

			// Read the glyph offsets.  Offsets
			// are measured from the start of the
			// offset table.
			array<int>	offsets;
			int	font_code_offset;
			if (wide_offsets)
			{
				// 32-bit offsets.
				for (int i = 0; i < glyph_count; i++)
				{
					offsets.push_back(in->read_u32());
				}
				font_code_offset = in->read_u32();
			}
			else
			{
				// 16-bit offsets.
				for (int i = 0; i < glyph_count; i++)
				{
					offsets.push_back(in->read_u16());
				}
				font_code_offset = in->read_u16();
			}

			m_glyphs.resize(glyph_count);
			if (m->get_create_font_shapes() == DO_LOAD_FONT_SHAPES)
			{
				// Read the glyph shapes.
				{for (int i = 0; i < glyph_count; i++)
				{
					// Seek to the start of the shape data.
					int	new_pos = table_base + offsets[i];

					// if we're seeking backwards, then that looks like a bug.
					// Vitaly: I think that it not so
					// assert(new_pos >= in->get_position());
					in->set_position(new_pos);

					// Create & read the shape.
					shape_character_def* s = new shape_character_def(m->get_player());
					s->read(in, 22, false, m);

					m_glyphs[i] = s;
				}}

				int	current_position = in->get_position();
				if (font_code_offset + table_base != current_position)
				{
					// Bad offset!  Don't try to read any more.
					return;
				}
			}
			else
			{
				// Skip the shape data.
				int	new_pos = table_base + font_code_offset;
				if (new_pos >= in->get_tag_end_position())
				{
					// No layout data!
					return;
				}

				in->set_position(new_pos);
			}

			read_code_table(in);

			// Read layout info for the glyphs.
			if (has_layout)
			{
				m_ascent = (float) in->read_s16();
				m_descent = (float) in->read_s16();
				m_leading = (float) in->read_s16();
				
				// Advance table; i.e. how wide each character is.
				m_advance_table.resize(m_glyphs.size());
				for (int i = 0, n = m_advance_table.size(); i < n; i++)
				{
					m_advance_table[i] = (float) in->read_s16();
				}

				// Bounds table.
				//m_bounds_table.resize(m_glyphs.size());	// kill
				rect	dummy_rect;
				{for (int i = 0, n = m_glyphs.size(); i < n; i++)
				{
					//m_bounds_table[i].read(in);	// kill
					dummy_rect.read(in);
				}}

				// Kerning pairs.
				int	kerning_count = in->read_u16();
				{for (int i = 0; i < kerning_count; i++)
				{
					Uint16	char0, char1;
					if (m_wide_codes)
					{
						char0 = in->read_u16();
						char1 = in->read_u16();
					}
					else
					{
						char0 = in->read_u8();
						char1 = in->read_u8();
					}
					float	adjustment = (float) in->read_s16();

					kerning_pair	k;
					k.m_char0 = char0;
					k.m_char1 = char1;

					// Remember this adjustment; we can look it up quickly
					// later using the character pair as the key.
					m_kerning_pairs[k] = adjustment;
				}}
			}
		}
	}


	void	font::read_font_info(stream* in, int tag_type)
	// Read additional information about this font, from a
	// DefineFontInfo tag.  The caller has already read the tag
	// type and font id.
	{
		in->read_string_with_length(&m_fontname);
		unsigned char	flags = in->read_u8();
		m_unicode_chars = (flags & 0x20) != 0;
		m_shift_jis_chars = (flags & 0x10) != 0;
		m_ansi_chars = (flags & 0x08) != 0;
		m_is_italic = (flags & 0x04) != 0;
		m_is_bold = (flags & 0x02) != 0;
		m_wide_codes = (flags & 0x01) != 0;

		// language code
		if (tag_type == 62)
		{
			// now unused
			in->read_u8();
		}

		read_code_table(in);
	}

	void	font::read_font_alignzones(stream* in, int tag_type)
	// Flash uses alignment zones to establish the borders of a glyph for pixel snapping.
	// Alignment zones are critical for high-quality display of fonts.
	// The alignment zone defines a bounding box for strong vertical and horizontal components of
	// a glyph. The box is described by a left coordinate, thickness, baseline coordinate, and height.	
	{
		m_table_hint = in->read_uint(2);
		in->read_uint(6);	// reserved

		// Read alignment zone information for each glyph.
		m_zone_table.resize(m_glyphs.size());
		for (int i = 0, n = m_glyphs.size(); i < n; i++)
		{
			int num_zone_data = in->read_u8();
			m_zone_table[i].m_zone_data.resize(num_zone_data);
			for (int j = 0; j < num_zone_data; j++)
			{
				m_zone_table[i].m_zone_data[j].m_alignment_coord = in->read_float16();
				m_zone_table[i].m_zone_data[j].m_range = in->read_float16();
			}
			m_zone_table[i].m_has_maskx = in->read_uint(1) == 1 ? true : false;
			m_zone_table[i].m_has_masky = in->read_uint(1) == 1 ? true : false;
			in->read_uint(6);	// reserved
		}

	}

	void	font::read_code_table(stream* in)
	// Read the table that maps from glyph indices to character
	// codes.
	{
		IF_VERBOSE_PARSE(log_msg("reading code table at offset %d\n", in->get_position()));

		assert(m_code_table.is_empty());

		if (m_wide_codes)
		{
			// Code table is made of Uint16's.
			for (int i = 0; i < m_glyphs.size(); i++)
			{
				m_code_table[in->read_u16()] = i;
			}
		}
		else
		{
			// Code table is made of bytes.
			for (int i = 0; i < m_glyphs.size(); i++)
			{
				m_code_table.add(in->read_u8(), i);
			}
		}
	}

	bool	font::get_glyph(glyph* g, Uint16 code, int fontsize) const
	{
		// defaults
		g->m_glyph_index = -1;
		g->m_glyph_advance = 512;

		if (m_code_table.get(code, &g->m_glyph_index))
		{
			g->m_shape_glyph = m_glyphs[g->m_glyph_index];
		}

		// first try glyph provider
		glyph_provider* fp = get_glyph_provider();
		if (fp)
		{
			g->m_bitmap_info = fp->get_char_image(g->m_shape_glyph, code, m_fontname, m_is_bold, m_is_italic, 
				fontsize,	&g->m_bounds, &g->m_glyph_advance);
			if (g->m_bitmap_info != NULL)
			{
				if (is_define_font3())
				{
					g->m_glyph_advance *= 20.0f;
				}

				// for embedded characters use advance value from 'm_advance_table'
				if (g->m_glyph_index >= 0 && g->m_glyph_index < m_advance_table.size())
				{
					g->m_glyph_advance = m_advance_table[g->m_glyph_index];
				}

				return true;
			}
		}

		// then use an embedded character
		if (g->m_glyph_index >= 0)
		{
			if (g->m_glyph_index < m_advance_table.size())
			{
				g->m_glyph_advance = m_advance_table[g->m_glyph_index];
			}
			else
			{
				if (is_define_font3())
				{
					g->m_glyph_advance *= 20.0f;
				}
			}
			return true;
		}

		return false;
	}

	shape_character_def*	font::get_glyph_by_index(int glyph_index) const
	{
		return glyph_index < m_glyphs.size() ? m_glyphs[glyph_index].get_ptr() : NULL;
	}

	int	font::get_code_by_index(int glyph_index) const
	{
		for (gameswf::hash<Uint16, int, simple_code_hash<Uint16> >::const_iterator it = m_code_table.begin(); it != m_code_table.end(); ++it)
		{
			if (it->second == glyph_index)
			{
				return it->first;
			}
		}
		return -1;
	}

	float	font::get_kerning_adjustment(int last_code, int code) const
	// Return the adjustment in advance between the given two
	// characters.  Normally this will be 0; i.e. the 
	{
		float	adjustment;
		kerning_pair	k;
		k.m_char0 = last_code;
		k.m_char1 = code;
		if (m_kerning_pairs.get(k, &adjustment))
		{
			return adjustment;
		}
		return 0;
	}


	void	font::output_cached_data(tu_file* out, const cache_options& options)
	// Dump our cached data into the given stream.
	{
// @@ Disabled.  Need to fix input_cached_data, so that it has a
// reliable and cheap way to skip over data for NULL glyphs.
#if 0
		// Dump cached shape data for glyphs (i.e. this will
		// be tesselations used to render larger glyph sizes).
		int	 n = m_glyphs.size();
		out->write_le32(n);
		for (int i = 0; i < n; i++)
		{
			shape_character_def*	s = m_glyphs[i].get_ptr();
			if (s)
			{
				s->output_cached_data(out, options);
			}
		}
#endif // 0
	}

	
	void	font::input_cached_data(tu_file* in)
	// Read our cached data from the given stream.
	{
// @@ Disable.  See comment in output_cached_data().
#if 0
		// Read cached shape data for glyphs.
		int	n = in->read_le32();
		if (n != m_glyphs.size())
		{
			log_error("error reading cache file in font::input_cached_data() "
				  "glyph count mismatch.\n");
			in->go_to_end();	// ensure that no more data will be read from this stream.
			return;
		}

		for (int i = 0; i < n; i++)
		{
			m_glyphs[i]->input_cached_data(in);
		}
#endif // 0
	}

};	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
