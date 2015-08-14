// gameswf_styles.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Fill and line style types.


#ifndef GAMESWF_STYLES_H
#define GAMESWF_STYLES_H


#include "gameswf_impl.h"


namespace gameswf
{
	struct stream;


	struct gradient_record
	{
		gradient_record();
		void	read(stream* in, int tag_type);

	//data:
		Uint8	m_ratio;
		rgba	m_color;
	};

	struct base_fill_style
	{
		virtual ~base_fill_style() {}
		virtual void apply(int fill_side, float ratio, render_handler::bitmap_blend_mode bm) const = 0;
	};

	struct fill_style : public base_fill_style
	// For the interior of outline shapes.
	{
		fill_style();
		virtual ~fill_style();

		void	read(stream* in, int tag_type, movie_definition_sub* m);
		rgba	sample_gradient(int ratio) const;
		gameswf::bitmap_info*	create_gradient_bitmap() const;
		virtual void	apply(int fill_side, float ratio, render_handler::bitmap_blend_mode bm) const;

		rgba	get_color() const { return m_color; }
		void	set_color(rgba new_color) { m_color = new_color; }
		int	    get_type() const { return m_type; }
		void    set_blend_mode(Uint8 bm) {m_blend_mode = bm;}

		// For shape morphing
		void	set_lerp(const fill_style& a, const fill_style& b, float t);

	private:
		friend struct morph2_character_def;
		friend struct canvas;

		int	m_type;
		Uint8 m_blend_mode;
		rgba	m_color;
		matrix	m_gradient_matrix;
		array<gradient_record>	m_gradients;
		gc_ptr<gameswf::bitmap_info>	m_gradient_bitmap_info;
		gc_ptr<bitmap_character_def>	m_bitmap_character;
		matrix	m_bitmap_matrix;
	};


	struct morph_fill_style : public base_fill_style
	{
		morph_fill_style();
		morph_fill_style(stream* in, movie_definition_sub* m);
		virtual ~morph_fill_style();
		
		void read(stream* in, movie_definition_sub* m);
		rgba sample_gradient(int ratio, float morph);
		bitmap_info* create_gradient_bitmap(float morph) const;
		virtual void apply(int fill_side, float morph, render_handler::bitmap_blend_mode bm) const;
		rgba get_color(float morph) const;
		void set_colors(rgba new_color_orig, rgba new_color_target);
	private:
		int m_type;
		rgba m_color[2];
		matrix m_gradient_matrix[2];
		array<gradient_record> m_gradients[2];
		gc_ptr<bitmap_info> m_gradient_bitmap_info[2];
		gc_ptr<bitmap_character_def> m_bitmap_character;
		matrix m_bitmap_matrix[2];
	};

	struct base_line_style
	{
		virtual ~base_line_style() {}
		virtual void apply(float ratio) const = 0;
	};

	struct line_style : public base_line_style
	// For the outside of outline shapes, or just bare lines.
	{
		line_style();
		virtual ~line_style() {}
		void	read(stream* in, int tag_type, movie_definition_sub* m);
		virtual void	apply(float ratio) const;

		Uint16	get_width() const { return m_width; }
		const rgba&	get_color() const { return m_color; }

	private:
		friend struct morph2_character_def;
		friend struct canvas;

		Uint16	m_width;	// in TWIPS
		rgba	m_color;
		fill_style m_fill_style;
		Uint8 m_start_capstyle;
		Uint8 m_joinstyle;
		bool m_has_fill_flag;
		bool m_no_hscale_flag;
		bool m_no_vscale_flag;
		bool m_pixelhinting_flag;
		bool m_noclose;
		Uint8 m_end_capstyle;
		Uint16 m_miter_limit_factor;
	};

	struct morph_line_style : public base_line_style
	{
		morph_line_style();
		morph_line_style(stream* in);
		virtual ~morph_line_style() {}

		void read(stream* in);
		virtual void apply(float morph) const;

	private:
		Uint16 m_width[2];
		rgba   m_color[2];
	};
}


#endif // GAMESWF_STYLES_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
