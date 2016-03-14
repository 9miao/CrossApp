// grid_index.h	-- Thatcher Ulrich <tu@tulrich.com> 2004

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Simple grid-based spatial index container.

#ifndef GRID_INDEX_H
#define GRID_INDEX_H


#include "tu_config.h"
#include "container.h"


//
// grid_index_point
//


template<class coord_t>
struct index_point
// Simple point class for spatial index.
{
	index_point() {}
	index_point(coord_t x_in, coord_t y_in)
		:
		x(x_in),
		y(y_in)
	{
	}

	bool	operator==(const index_point<coord_t>& pt) const
	{
		return x == pt.x && y == pt.y;
	}

//data:
	coord_t	x, y;
};


template<class coord_t>
struct index_box
// Simple bounding box class.
{
	index_box() {}
	index_box(coord_t min_max_x, coord_t min_max_y)
		:
		min(min_max_x, min_max_y),
		max(min_max_x, min_max_y)
	{
	}
	index_box(const index_point<coord_t>& min_max_in)
		:
		min(min_max_in),
		max(min_max_in)
	{
	}
	index_box(const index_point<coord_t>& min_in, const index_point<coord_t>& max_in)
		:
		min(min_in),
		max(max_in)
	{
	}

	const index_point<coord_t>& get_min() const { return min; }
	const index_point<coord_t>& get_max() const { return max; }
	
	coord_t	get_width() const { return max.x - min.x; }
	coord_t	get_height() const { return max.y - min.y; }

	void set_to_point(coord_t x, coord_t y)
	{
		min.x = x; min.y = y;
		max.x = x; max.y = y;
	}
	void set_to_point(const index_point<coord_t>& loc) {
		set_to_point(loc.x, loc.y);
	}
	
	void expand_to_enclose(coord_t x, coord_t y)
	{
		if (x < min.x) min.x = x;
		if (y < min.y) min.y = y;
		if (x > max.x) max.x = x;
		if (y > max.y) max.y = y;
	}
	
	void	expand_to_enclose(const index_point<coord_t>& loc)
	{
		expand_to_enclose(loc.x, loc.y);
	}

	bool	contains_point(const index_point<coord_t>& loc) const
	{
		if (loc.x >= min.x && loc.x <= max.x && loc.y >= min.y && loc.y <= max.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

//data:
	index_point<coord_t>	min;
	index_point<coord_t>	max;
};


// Flag for passing to grid_index constructors; causes them to
// autosize their grid based on bbox and estimated item count.
enum grid_index_autosize {
	GRID_INDEX_AUTOSIZE,
};

template<class coord_t>
void grid_index_pick_good_grid_size(
	int* x_cells, int* y_cells, const index_box<coord_t>& bound, int item_count_estimate, float grid_scale = 0.707f)
// Utility to find good values for *x_cells and *y_cells for a grid
// index, based on the expected number of items to index and the
// bounding box.
//
// You can bias the values up or down using grid_scale.
{
	*x_cells = 1;
	*y_cells = 1;
	if (item_count_estimate > 0)
	{
		coord_t	width = bound.get_width();
		coord_t	height = bound.get_height();
		float	area = float(width) * float(height);
		if (area > 0)
		{
			float	sqrt_n = sqrt((float) item_count_estimate);
			float	w = width * width / area * grid_scale;
			float	h = height * height / area * grid_scale;
			*x_cells = int(w * sqrt_n);
			*y_cells = int(h * sqrt_n);
		}
		else
		{
			// Zero area.
			if (width > 0)
			{
				*x_cells = int(grid_scale * grid_scale * item_count_estimate);
			}
			else
			{
				*y_cells = int(grid_scale * grid_scale * item_count_estimate);
			}
		}
		*x_cells = iclamp(*x_cells, 1, 256);
		*y_cells = iclamp(*y_cells, 1, 256);
	}
}


template<class coord_t, class payload_t>
struct grid_entry_point
// Holds one entry for a grid point cell.
{
	index_point<coord_t>	location;
	payload_t	value;

	grid_entry_point<coord_t, payload_t>*	m_next;
};


template<class coord_t, class payload>
struct grid_index_point
// Grid-based container for points.
{
	typedef index_point<coord_t>	point_t;
	typedef index_box<coord_t>	box_t;
	typedef grid_entry_point<coord_t, payload>	grid_entry_t;

	grid_index_point(const box_t& bound, int x_cells, int y_cells)
		:
		m_bound(bound),
		m_x_cells(x_cells),
		m_y_cells(y_cells)
	{
		assert(x_cells > 0 && y_cells > 0);
		assert(bound.min.x <= bound.max.x);
		assert(bound.min.y <= bound.max.y);

		// Allocate the grid.
		m_grid = new grid_entry_t*[x_cells * y_cells];
		memset(&m_grid[0], 0, sizeof(grid_entry_t*) * x_cells * y_cells);
	}

	grid_index_point(grid_index_autosize p, const box_t& bound, int item_count_estimate, float grid_scale = 0.707f)
		:
		m_bound(bound)
	{
		grid_index_pick_good_grid_size(&m_x_cells, &m_y_cells, bound, item_count_estimate, grid_scale);
		
		assert(m_x_cells > 0 && m_y_cells > 0);
		assert(bound.min.x <= bound.max.x);
		assert(bound.min.y <= bound.max.y);

		// Allocate the grid.
		m_grid = new grid_entry_t*[m_x_cells * m_y_cells];
		memset(&m_grid[0], 0, sizeof(grid_entry_t*) * m_x_cells * m_y_cells);
	}

	
	~grid_index_point()
	{
		for (int i = 0, n = m_x_cells * m_y_cells; i < n; i++)
		{
			grid_entry_t*	e = m_grid[i];
			while (e)
			{
				grid_entry_t*	next = e->m_next;
				delete e;
				e = next;
			}
		}

		delete [] m_grid;
	}

	const box_t&	get_bound() const { return m_bound; }

	struct iterator
	{
		grid_index_point*	m_index;
		box_t	m_query;
		index_box<int>	m_query_cells;
		int	m_current_cell_x, m_current_cell_y;
		grid_entry_t*	m_current_entry;

		iterator()
			:
			m_index(NULL),
			m_query(point_t(0, 0), point_t(0, 0)),
			m_query_cells(index_point<int>(0, 0), index_point<int>(0, 0)),
			m_current_cell_x(0),
			m_current_cell_y(0),
			m_current_entry(NULL)
		{
		}

		bool	at_end() const { return m_current_entry == NULL; }

		void	operator++()
		{
			if (at_end() == false)
			{
				advance();
			}
		}

		void	advance()
		// Point at next element in the iteration.
		{
			if (m_current_entry)
			{
				// Continue through current cell.
				m_current_entry = m_current_entry->m_next;

				if (at_end() == false)
				{
					return;
				}
			}
			assert(m_current_entry == NULL);

			// Done with current cell; go to next cell.
			m_current_cell_x++;
			while (m_current_cell_y <= m_query_cells.max.y)
			{
				for (;;)
				{
					if (m_current_cell_x > m_query_cells.max.x)
					{
						break;
					}

					m_current_entry = m_index->get_cell(m_current_cell_x, m_current_cell_y);
					if (m_current_entry)
					{
						// Found a valid cell.
						return;
					}

					m_current_cell_x++;
				}
				m_current_cell_x = m_query_cells.min.x;
				m_current_cell_y++;
			}

			assert(m_current_cell_x == m_query_cells.min.x);
			assert(m_current_cell_y == m_query_cells.max.y + 1);

			// No more valid cells.
			assert(at_end());
		}

		grid_entry_t&	operator*()
		{
			assert(at_end() == false && m_current_entry != NULL);
			return *m_current_entry;
		}
		grid_entry_t*	operator->() { return &(operator*()); }

		// @@ TODO Finish
		//
		//bool	operator==(const const_iterator& it) const
		//{
		//	if (at_end() && it.at_end())
		//	{
		//		return true;
		//	}
		//	else
		//	{
		//		return
		//			m_hash == it.m_hash
		//			&& m_index == it.m_index;
		//	}
		//}
		//
		//bool	operator!=(const const_iterator& it) const { return ! (*this == it); }
	};

	iterator	begin(const box_t& q)
	{
		iterator	it;
		it.m_index = this;
		it.m_query = q;
		it.m_query_cells.min = get_containing_cell_clamped(q.min);
		it.m_query_cells.max = get_containing_cell_clamped(q.max);

		assert(it.m_query_cells.min.x <= it.m_query_cells.max.x);
		assert(it.m_query_cells.min.y <= it.m_query_cells.max.y);

		it.m_current_cell_x = it.m_query_cells.min.x;
		it.m_current_cell_y = it.m_query_cells.min.y;
		it.m_current_entry = get_cell(it.m_current_cell_x, it.m_current_cell_y);

		// Make sure iterator starts valid.
		if (it.m_current_entry == NULL)
		{
			it.advance();
		}

		return it;
	}

	iterator	end() const
	{
		iterator	it;
		it.m_index = this;
		it.m_current_entry = NULL;

		return it;
	}

	void	add(const point_t& location, payload p)
	// Insert a point, with the given location and payload, into
	// our index.
	{
		index_point<int>	ip = get_containing_cell_clamped(location);

		grid_entry_t*	new_entry = new grid_entry_t;
		new_entry->location = location;
		new_entry->value = p;

		// Link it into the containing cell.
		int	index = get_cell_index(ip);
		new_entry->m_next = m_grid[index];
		m_grid[index] = new_entry;
	}

	void	remove(grid_entry_t* entry)
	// Removes the entry from the index, and deletes it.
	{
		assert(entry);

		index_point<int>	ip = get_containing_cell_clamped(entry->location);
		int	index = get_cell_index(ip);

		// Unlink matching entry.
		grid_entry_t**	prev_ptr = &m_grid[index];
		grid_entry_t*	ptr = *prev_ptr;
		while (ptr)
		{
			if (ptr == entry)
			{
				// This is the one; unlink it.
				*prev_ptr = ptr->m_next;
				
				delete entry;

				return;
			}
			
			// Go to the next entry.
			prev_ptr = &ptr->m_next;
			ptr = *prev_ptr;
		}

		// Didn't find entry!  Something is wrong.
		assert(0);
	}

	iterator	find(const point_t& location, payload p)
	// Helper.  Search for matching entry.
	{
		iterator it;
		for (it = begin(box_t(location, location)); ! it.at_end(); ++it)
		{
			if (it->location == location && it->value == p)
			{
				// Found it.
				return it;
			}
		}

		// Didn't find it.
		assert(it.at_end());
		return it;
	}

private:
	
	grid_entry_t*	get_cell(int x, int y)
	{
		assert(x >= 0 && x < m_x_cells);
		assert(y >= 0 && y < m_y_cells);

		return m_grid[x + y * m_x_cells];
	}

	int	get_cell_index(const index_point<int>& ip)
	{
		assert(ip.x >= 0 && ip.x < m_x_cells);
		assert(ip.y >= 0 && ip.y < m_y_cells);

		int	index = ip.x + ip.y * m_x_cells;

		return index;
	}

	index_point<int>	get_containing_cell_clamped(const point_t& p) const
	// Get the indices of the cell that contains the given point.
	{
		index_point<int>	ip;
		ip.x = int(((p.x - m_bound.min.x) * coord_t(m_x_cells)) / (m_bound.max.x - m_bound.min.x));
		ip.y = int(((p.y - m_bound.min.y) * coord_t(m_y_cells)) / (m_bound.max.y - m_bound.min.y));

		// Clamp.
		if (ip.x < 0) ip.x = 0;
		if (ip.x >= m_x_cells) ip.x = m_x_cells - 1;
		if (ip.y < 0) ip.y = 0;
		if (ip.y >= m_y_cells) ip.y = m_y_cells - 1;

		return ip;
	}

//data:
	box_t	m_bound;
	int	m_x_cells;
	int	m_y_cells;
	grid_entry_t**	m_grid;
};


//
// grid_index_box
//


template<class coord_t, class payload>
struct grid_entry_box
// Holds one entry for a grid cell.
{
	index_box<coord_t>	bound;
	payload	value;

	int	m_last_query_id;	// avoid returning the same item multiple times

	grid_entry_box() : m_last_query_id(0) {}
};


template<class coord_t, class payload>
struct grid_index_box
// Grid-based container for boxes.
{
	typedef index_point<coord_t>	point_t;
	typedef index_box<coord_t>	box_t;
	typedef grid_entry_box<coord_t, payload>	grid_entry_t;

	grid_index_box(const box_t& bound, int x_cells, int y_cells)
		:
		m_bound(bound),
		m_x_cells(x_cells),
		m_y_cells(y_cells),
		m_query_id(0)
	{
		assert(x_cells > 0 && y_cells > 0);
		assert(bound.min.x <= bound.max.x);
		assert(bound.min.y <= bound.max.y);

		// Allocate the grid.
		m_grid = new swf_array<grid_entry_t*>[x_cells * y_cells];
	}

	// Constructor that picks good values for x_cells and y_cells,
	// assuming the (approximate) given number of items are
	// distributed throughout the given bounding box.
	//
	// You can optionally adjust grid_scale to get more or fewer
	// grid cells.
	grid_index_box(grid_index_autosize p, const box_t& bound, int item_count_estimate, float grid_scale = 0.707f)
		:
		m_bound(bound),
		m_query_id(0)
	{
		grid_index_pick_good_grid_size(&m_x_cells, &m_y_cells, bound, item_count_estimate, grid_scale);
		
		assert(m_x_cells > 0 && m_y_cells > 0);
		assert(bound.min.x <= bound.max.x);
		assert(bound.min.y <= bound.max.y);
		m_grid = new swf_array<grid_entry_t*>[m_x_cells * m_y_cells];
	}

	~grid_index_box()
	{
		// Need to delete all entries (be careful to only
		// delete each entry once, even though entries may be
		// repeated many times).
		swf_array<grid_entry_t*>&	cell_array = m_grid[0];
		for (int i = 0, n = cell_array.size(); i < n; i++)
		{
			grid_entry_t*	e = cell_array[i];
			if (e)
			{
				delete e;
			}
		}
		delete [] m_grid;
	}

	const box_t&	get_bound() const { return m_bound; }
	int	get_query_id() const { return m_query_id; }

	struct iterator
	{
		grid_index_box*	m_index;
		box_t	m_query;
		index_box<int>	m_query_cells;
		int	m_current_cell_x, m_current_cell_y;
		int	m_current_cell_array_index;
		grid_entry_t*	m_current_entry;

		iterator()
			:
			m_index(NULL),
			m_query(point_t(0, 0), point_t(0, 0)),
			m_query_cells(index_point<int>(0, 0), index_point<int>(0, 0)),
			m_current_cell_x(0),
			m_current_cell_y(0),
			m_current_cell_array_index(-1),
			m_current_entry(NULL)
		{
		}

		bool	at_end() const { return m_current_entry == NULL; }

		void	operator++()
		{
			if (at_end() == false)
			{
				advance();
			}
		}

		void	advance()
		// Point at next element in the iteration.
		{
			if (advance_in_cell())
			{
				return;
			}

			// Done with current cell; go to next cell.
			m_current_cell_x++;
			while (m_current_cell_y <= m_query_cells.max.y)
			{
				for (;;)
				{
					if (m_current_cell_x > m_query_cells.max.x)
					{
						break;
					}

					if (advance_in_cell())
					{
						// We're good.
						return;
					}

					m_current_cell_x++;
				}
				m_current_cell_x = m_query_cells.min.x;
				m_current_cell_y++;
			}

			assert(m_current_cell_x == m_query_cells.min.x);
			assert(m_current_cell_y == m_query_cells.max.y + 1);

			// No more valid cells.
			assert(at_end());
		}

		bool	advance_in_cell()
		// Go to the next valid element in the current cell.
		// If we reach the end of the cell, set
		// m_current_cell_array_index to -1 and return false.
		// Otherwise point m_current_entry at the valid
		// element, and return true.
		{
			int	query_id = m_index->get_query_id();
			swf_array<grid_entry_t*>*	cell_array = m_index->get_cell(m_current_cell_x, m_current_cell_y);

			while (++m_current_cell_array_index < cell_array->size())
			{
				// Continue through the current cell.
				m_current_entry = (*cell_array)[m_current_cell_array_index];
				if (m_current_entry->m_last_query_id == query_id)
				{
					// Skip this one; we already iterated over it.
				}
				else
				{
					// Valid entry.
					
					// Update query id.
					m_current_entry->m_last_query_id = query_id;

					return true;
				}
			}

			// No more valid entries in this cell.
			m_current_entry = NULL;
			m_current_cell_array_index = -1;

			return false;
		}

		grid_entry_t&	operator*()
		{
			assert(at_end() == false && m_current_entry != NULL);
			return *m_current_entry;
		}
		grid_entry_t*	operator->() { return &(operator*()); }

		// @@ TODO Finish
		//
		//bool	operator==(const const_iterator& it) const
		//{
		//	if (at_end() && it.at_end())
		//	{
		//		return true;
		//	}
		//	else
		//	{
		//		return
		//			m_hash == it.m_hash
		//			&& m_index == it.m_index;
		//	}
		//}
		//
		//bool	operator!=(const const_iterator& it) const { return ! (*this == it); }
	};

	iterator	begin(const box_t& q)
	{
		m_query_id++;
		if (m_query_id == 0)
		{
			// Query id wrapped around!  Clear m_last_query_id in all entries in our
			// array, to avoid aliasing from old queries.
			int	cell_count = m_x_cells * m_y_cells;
			for (int i = 0; i < cell_count; i++)
			{
				swf_array<grid_entry_t*>*	cell_array = &m_grid[i];
				for (int j = 0, n = cell_array->size(); j < n; j++)
				{
					(*cell_array)[j]->m_last_query_id = 0;
				}
			}
			m_query_id = 1;
		}

		iterator	it;
		it.m_index = this;
		it.m_query = q;
		it.m_query_cells.min = get_containing_cell_clamped(q.min);
		it.m_query_cells.max = get_containing_cell_clamped(q.max);

		assert(it.m_query_cells.min.x <= it.m_query_cells.max.x);
		assert(it.m_query_cells.min.y <= it.m_query_cells.max.y);

		it.m_current_cell_x = it.m_query_cells.min.x;
		it.m_current_cell_y = it.m_query_cells.min.y;

		it.advance();	// find first valid entry.

		return it;
	}

	iterator	begin_all()
	// For convenience.
	{
		return begin(get_bound());
	}

	iterator	end() const
	{
		iterator	it;
		it.m_index = this;
		it.m_current_entry = NULL;

		return it;
	}

	void	add(const box_t& bound, payload p)
	// Insert a box, with the given payload, into our index.
	{
//		index_box<int>	ib = get_containing_cells_clamped(bound);

		grid_entry_t*	new_entry = new grid_entry_t;
		new_entry->bound = bound;
		new_entry->value = p;

		// Add it to all cells it overlaps with.
		for (int i = 0, n = m_x_cells * m_y_cells; i < n; i++)
		{
			swf_array<grid_entry_t*>&	cell_array = m_grid[i];
			cell_array.push_back(new_entry);
		}
	}


	void	remove(grid_entry_t* entry)
	// Removes the entry from the index, and deletes it.
	{
		assert(entry);

		// Find and remove the entry from all cells that it overlaps with.
		index_box<int>	ib = get_containing_cells_clamped(entry->bound);
		
		for (int iy = ib.min.y; iy <= ib.max.y; iy++)
		{
			for (int ix = ib.min.x; ix <= ib.max.x; ix++)
			{
				swf_array<grid_entry_t*>*	cell_array = get_cell(ix, iy);

				int	i, n;
				for (i = 0, n = cell_array->size(); i < n; i++)
				{
					// Find entry, and remove it.
					if ((*cell_array)[i] == entry)
					{
						cell_array->remove(i);
						break;
					}
				}

				assert(i < n);	// Didn't find entry in this cell!  Something is wrong.
			}
		}

		delete entry;
	}


	iterator	find(const box_t& bound, payload p)
	// Helper.  Search for matching entry.
	{
		iterator it;
		for (it = begin(bound); ! it.at_end(); ++it)
		{
			if (it->bound == bound && it->value == p)
			{
				// Found it.
				return it;
			}
		}

		// Didn't find it.
		assert(it.at_end());
		return it;
	}

	grid_entry_t*	find_payload_from_point(const index_point<coord_t>& loc, payload p)
	// Helper.  Search for matching payload, given any point within its bound.
	// Should be relatively quick, assuming payload is unique.
	{
		index_point<int>	ip = get_containing_cell_clamped(loc);
		swf_array<grid_entry_t*>*	cell_array = get_cell(ip.x, ip.y);
		
		for (int i = 0, n = cell_array->size(); i < n; i++)
		{
			grid_entry_t*	entry = (*cell_array)[i];
			if (entry->value == p)
			{
				// Found it.
				return entry;
			}
		}

		// Didn't find it.
		return NULL;
	}

private:
	
	swf_array<grid_entry_t*>*	get_cell(int x, int y)
	{
		assert(x >= 0 && x < m_x_cells);
		assert(y >= 0 && y < m_y_cells);

		return &m_grid[x + y * m_x_cells];
	}

	index_point<int>	get_containing_cell_clamped(const point_t& p) const
	// Get the indices of the cell that contains the given point.
	{
		index_point<int>	ip;
		ip.x = int(((p.x - m_bound.min.x) * coord_t(m_x_cells)) / (m_bound.max.x - m_bound.min.x));
		ip.y = int(((p.y - m_bound.min.y) * coord_t(m_y_cells)) / (m_bound.max.y - m_bound.min.y));

		// Clamp.
		if (ip.x < 0) ip.x = 0;
		if (ip.x >= m_x_cells) ip.x = m_x_cells - 1;
		if (ip.y < 0) ip.y = 0;
		if (ip.y >= m_y_cells) ip.y = m_y_cells - 1;

		return ip;
	}

	index_box<int>	get_containing_cells_clamped(const box_t& p) const
	// Get the indices of the cell that contains the given point.
	{
		index_box<int>	ib(get_containing_cell_clamped(p.min), get_containing_cell_clamped(p.max));
		return ib;
	}

//data:
	box_t	m_bound;
	int	m_x_cells;
	int	m_y_cells;
	int	m_query_id;
	swf_array<grid_entry_t*>*	m_grid;
};


#endif // GRID_INDEX_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

