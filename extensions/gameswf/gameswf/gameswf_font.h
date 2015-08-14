// gameswf_font.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A font type for gameswf.


#ifndef GAMESWF_FONT_H
#define GAMESWF_FONT_H


#include "../base/container.h"
#include "gameswf.h"
#include "gameswf_types.h"
#include "gameswf_impl.h"
#include "gameswf_freetype.h"

class tu_file;

namespace gameswf
{
	struct shape_character_def;
	struct stream;
	struct bitmap_info;

	// Struct for holding (cached) textured glyph info.
	struct glyph
	{
		int m_glyph_index;
		float	m_glyph_advance;
		gc_ptr<shape_character_def>	m_shape_glyph;
		gc_ptr<bitmap_info> m_bitmap_info;
		rect m_bounds;	// the origin of the glyph box, in uv coords
		int m_fontsize;

		glyph() :
			m_glyph_index(-1),
			m_glyph_advance(512)
		{
		}
	
		~glyph()
		{
		}

		bool	is_renderable() const
		// Return true if this can be used for rendering.
		{
			return m_bitmap_info != NULL;
		}

		void	set_bitmap_info(bitmap_info* bi)
		{
			m_bitmap_info = bi;
		}

	};

	struct font : public character_def
	{
		font(player* player);
		~font();

		// Unique id of a gameswf resource
		enum { m_class_id = AS_FONT };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return character_def::is(class_id);
		}

		int	get_glyph_count() const { return m_glyphs.size(); }
		shape_character_def*	get_glyph_by_index(int glyph_index) const;
		int	get_code_by_index(int glyph_index) const;

		void	read(stream* in, int tag_type, movie_definition_sub* m);
		void	read_font_info(stream* in, int tag_type);
		void	read_font_alignzones(stream* in, int tag_type);

		void	output_cached_data(tu_file* out, const cache_options& options);
		void	input_cached_data(tu_file* in);

		const tu_string& get_name() const { return m_fontname; }
		void	set_name(const tu_string& name) { m_fontname = name; }
		bool	is_bold() const { return m_is_bold; }
		void	set_bold(bool bold) { m_is_bold = bold; }
		bool	is_italic() const { return m_is_italic; }
		void	set_italic(bool italic) { m_is_italic = italic; }

		movie_definition_sub*	get_owning_movie() const { return m_owning_movie; }

		bool	get_glyph(glyph* g, Uint16 code, int fontsize) const;
		float	get_kerning_adjustment(int last_code, int this_code) const;
		float	get_leading() const { return m_leading; }
		float	get_descent() const { return m_descent; }
		bool	is_define_font3() const { return m_zone_table.size() == 0 ? false : true; }

	private:
		void	read_code_table(stream* in);

		swf_array<gc_ptr<shape_character_def> >	m_glyphs;
		tu_string	m_fontname;
		movie_definition_sub*	m_owning_movie;
		bool	m_has_layout;
		bool	m_unicode_chars;
		bool	m_shift_jis_chars;
		bool	m_ansi_chars;
		bool	m_is_italic;
		bool	m_is_bold;
		bool	m_wide_codes;

		// This table maps from Unicode character number to glyph index.
		// m_code_table[character_code] = glyph_index
		//
		// @@ TODO: avoid little allocs; replace this with a flat hash, or else a sorted array (binary search)
		template<class T>
		struct simple_code_hash
		// Dummy hash functor.
		{
			size_t	operator()(const T& data) const { return data; }
		};
		gameswf::hash<Uint16, int, simple_code_hash<Uint16> > m_code_table;

		// Layout stuff.
		float	m_ascent;
		float	m_descent;
		float	m_leading;
		swf_array<float>	m_advance_table;
		// @@ we don't seem to use this thing at all, so don't bother keeping it.
		// swf_array<rect>	m_bounds_table;	// @@ this thing should be optional.

		// @@ replace this with a flat hash, or else a sorted array (binary search)
		struct kerning_pair
		{
			Uint16	m_char0, m_char1;

			bool	operator==(const kerning_pair& k) const
			{
				return m_char0 == k.m_char0 && m_char1 == k.m_char1;
			}
		};
		gameswf::hash<kerning_pair, float>	m_kerning_pairs;

		// Font thickness hint. Refers to the thickness of the typical stroke used in the font
		// 0 = thin, 1 = medium, 2 = thick
		Uint8 m_table_hint;

		// Alignment zones
		struct zone_record
		{
			struct zone_data
			{
				float m_alignment_coord;
				float m_range;
			};
			swf_array<zone_data> m_zone_data;

			bool m_has_maskx;
			bool m_has_masky;
		};

		swf_array<zone_record> m_zone_table;
	};

}	// end namespace gameswf



#endif // GAMESWF_FONT_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
