// gameswf_text.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code for the text tags.


//TODO: csm_textsetting() is common for text_def & edit_text_def
//	therefore make new base class for text_def & edit_text_def
//	and move csm_textsetting() to it.

#include "../base/utf8.h"
#include "../base/utility.h"
#include "gameswf_impl.h"
#include "gameswf_shape.h"
#include "gameswf_stream.h"
#include "gameswf_log.h"
#include "gameswf_font.h"
#include "gameswf_render.h"
#include "gameswf_as_classes/as_textformat.h"
#include "gameswf_root.h"
#include "gameswf_movie_def.h"

namespace gameswf
{


	void	as_global_textfield_ctor(const fn_call& fn);

	// Helper struct.
	struct text_style
	{
		int	m_font_id;
		mutable font*	m_font;
		mutable font*   m_lastfont;
		rgba	m_color;
		float	m_x_offset;
		float	m_y_offset;
		float	m_text_height;
		bool	m_has_x_offset;
		bool	m_has_y_offset;
		float m_scale;
		float m_leading;

		text_style() :
			m_font_id(-1),
			m_font(NULL),
			m_lastfont(NULL),
			m_x_offset(0),
			m_y_offset(0),
			m_text_height(1.0f),
			m_has_x_offset(false),
			m_has_y_offset(false),
			m_scale(0),
			m_leading(0)
		{
		}

		void	resolve_font(movie_definition_sub* root_def) const
		{
			if (m_font == NULL)
			{
				assert(m_font_id >= 0);

				m_font = root_def->get_font(m_font_id);
				if (m_font == NULL)
				{
					log_error("error: text style with undefined font; font_id = %d\n", m_font_id);
				}
			}
		}
	};


	// Helper structs.

	struct text_glyph_record
	{
		text_style	m_style;
		array<glyph>	m_glyphs;

		void	read(stream* in, int glyph_count, int glyph_bits, int advance_bits)
		{
			m_glyphs.resize(glyph_count);
			for (int i = 0; i < glyph_count; i++)
			{
				m_glyphs[i].m_glyph_index = in->read_uint(glyph_bits);
				m_glyphs[i].m_glyph_advance = (float) in->read_sint(advance_bits);
			}
		}
	};

	//
	// text_character_def
	//

	struct text_character_def : public character_def
	{
		movie_definition_sub*	m_root_def;
		rect	m_rect;
		matrix	m_matrix;
		array<text_glyph_record>	m_text_glyph_records;

		// Flash 8
		bool m_use_flashtype;
		int m_grid_fit;
		float m_thickness;
		float m_sharpness;

		// it's used to draw glyphs on startup
		bool m_is_glyphs_drawn;

		text_character_def(player* player, movie_definition_sub* root_def);

		void	read(stream* in, int tag_type, movie_definition_sub* m);
		void	display(character* inst);
		void	csm_textsetting(stream* in, int tag_type);

		virtual void get_bound(rect* bound);
	};

	//
	// edit_text_character_def
	//

	struct edit_text_character_def : public character_def
	// A definition for a text display character, whose text can
	// be changed at runtime (by script or host).
	{
		movie_definition_sub*	m_root_def;
		rect			m_rect;
		tu_string		m_var_name;
		bool			m_word_wrap;
		bool			m_multiline;
		bool			m_password;	// show asterisks instead of actual characters
		bool			m_readonly;
		bool			m_auto_size;	// resize our bound to fit the text
		bool			m_no_select;
		bool			m_border;	// forces white background and black border -- silly, but sometimes used
		bool			m_html;
		bool			m_use_outlines;	// when true, use specified SWF internal font.  Otherwise, renderer picks a default font
		int				m_font_id;
		font*			m_font;
		float			m_text_height;
		rgba			m_color;
		int				m_max_length;

		enum alignment
		{
			ALIGN_LEFT = 0,
			ALIGN_RIGHT,
			ALIGN_CENTER,
			ALIGN_JUSTIFY	// probably don't need to implement...
		};
		alignment	m_alignment;

		float			m_left_margin;	// extra space between box border and text
		float			m_right_margin;
		float			m_indent;	// how much to indent the first line of multiline text
		float			m_leading;	// extra space between lines (in addition to default font line spacing)
		tu_string	m_default_text;

		// Flash 8
		bool			m_use_flashtype;
		int				m_grid_fit;
		float			m_thickness;
		float			m_sharpness;

		edit_text_character_def(player* player, int width, int height);
		edit_text_character_def(player* player, movie_definition_sub* root_def);
		~edit_text_character_def();

		character*	create_character_instance(character* parent, int id);
		void	read(stream* in, int tag_type, movie_definition_sub* m);
		void	csm_textsetting(stream* in, int tag_type);

		virtual void get_bound(rect* bound) {	*bound = m_rect; }
	};

	//
	// edit_text_character
	//

	struct edit_text_character : public character
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_EDIT_TEXT };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return character::is(class_id);
		}

		gc_ptr<edit_text_character_def>	m_def;
		array<text_glyph_record>	m_text_glyph_records;
		array<fill_style>	m_dummy_style;	// used to pass a color on to shape_character::display()
		array<line_style>	m_dummy_line_style;
		rect	m_text_bounding_box;	// bounds of dynamic text, as laid out
		tu_string	m_text;
		bool m_has_focus;
		bool m_password;
		int m_cursor;
		float m_xcursor;
		float m_ycursor;
		float m_x;	// current X drawing position
		float m_y;	// current Y drawing position

		// instance specific
		rgba m_color;
		float m_text_height;
		gc_ptr<font> m_font;
		edit_text_character_def::alignment	m_alignment;
		float	m_left_margin;
		float	m_right_margin;
		float	m_indent;
		float	m_leading;
		rgba m_background_color;
		matrix m_world_matrix;	// current world matrix, for dynamic scaling

		edit_text_character(player* player, character* parent, edit_text_character_def* def, int id);
		~edit_text_character();

		virtual character_def* get_character_def() { return m_def.get_ptr();	}
		void reset_format(as_textformat* tf);

		const char *type_of() { return "edittext";}

		root* get_root();
		void show_cursor();
		void display();
		virtual bool on_event(const event_id& id);
		virtual bool can_handle_mouse_event();
		virtual bool get_topmost_mouse_entity( character * &te, float x, float y);
		const tu_string&	get_var_name() const;
		void	reset_bounding_box(float x, float y);
		void	set_text_value(const tu_string& new_text);
		virtual const char*	to_string();
		bool	set_member(const tu_stringi& name, const as_value& val);
		bool	get_member(const tu_stringi& name, as_value* val);
		void	align_line(edit_text_character_def::alignment align, int last_line_start_record, float x);

		void	format_text();
		bool	format_html_text(text_glyph_record& rec);
		void	format_plain_text(const tu_string& text, text_glyph_record& rec);
		const char* html_paragraph(const char* p, text_glyph_record& rec);
		const char* html_font(const char* p, text_glyph_record& rec);
		const char* html_text(const char* p, text_glyph_record& rec);

		virtual void advance(float delta_time);

	private:

		void	set_text(const tu_string& new_text);

	};

}	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
