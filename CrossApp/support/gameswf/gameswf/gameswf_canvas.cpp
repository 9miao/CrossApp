// gameswf_canvas.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Drawing API implementation

#include "gameswf_canvas.h"

namespace gameswf
{

	canvas::canvas(player* player) :
		shape_character_def(player),
		m_current_x(0),
		m_current_y(0),
		m_current_fill(0),
		m_current_line(0),
		m_current_path(-1)
	{
	}

	canvas::~canvas()
	{
	}

	void canvas::begin_fill(const rgba& color)
	{

		// default fill style is solid
		fill_style fs;
		fs.m_type = 0x00; 
		fs.m_color = color;
		m_fill_styles.push_back(fs);
		m_current_fill = m_fill_styles.size();

		add_path(true);
	}

	void canvas::end_fill()
	{
		if (m_current_path >= 0)
		{
			close_path();
		}

		m_current_path = -1;
		m_current_fill = 0;
	}

	void canvas::close_path()
	{
		path& p = m_paths[m_current_path];
		swf_array<edge>& edges = m_paths[m_current_path].m_edges;
		if (edges.size() > 0)
		{
			// Close it with a straight edge if needed
			const edge& ed = edges.back();
			if (ed.m_ax != p.m_ax
				|| ed.m_ay != p.m_ay)
			{
				edge new_edge(p.m_ax, p.m_ay, p.m_ax, p.m_ay);
				edges.push_back(new_edge);
			}
		}
	}

	void canvas::add_path(bool new_path)
	{
		if (m_current_path >= 0 && m_current_fill > 0)
		{
			close_path();
		}

		// Flash uses the left fill style
		path p(m_current_x, m_current_y, m_current_fill, 0, m_current_line);
		p.m_new_shape = new_path;
		m_paths.push_back(p);
		m_current_path = m_paths.size() - 1;
		
		flush_cache();
	}

	void canvas::set_line_style(Uint16 width, const rgba& color)
	{
		line_style ls;
		ls.m_color = color;
		ls.m_width = width;
		m_line_styles.push_back(ls);
		m_current_line = m_line_styles.size();
		add_path(false);
	}

	void canvas::move_to(float x, float y)
	{
		if (x != m_current_x || y != m_current_y)
		{
			m_current_x = x;
			m_current_y = y;
			add_path(false);
		}
	}

	void canvas::line_to(float x, float y)
	{
		if (m_current_path < 0)
		{
			add_path(true);
		}

		m_current_x = x;
		m_current_y = y;

		edge ed(x, y, x, y);
		m_paths[m_current_path].m_edges.push_back(ed); 
		flush_cache();

	}

	void canvas::curve_to(float cx, float cy, float ax, float ay)
	{
		if (m_current_path >= 0)
		{
			add_path(true); 
		}

		m_current_x = ax;
		m_current_y = ay;

		m_paths[m_current_path].m_edges.push_back(edge(cx, cy, ax, ay)); 
		flush_cache();
	}

	void canvas::get_bound(rect* bound)
	{
		compute_bound(bound);
	}

}	// end namespace gameswf
