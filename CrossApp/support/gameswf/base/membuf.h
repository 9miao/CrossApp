// membuf.h	-- Thatcher Ulrich 2005

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A simple memory buffer.  Similar to a string, but can hold null
// characters.


#ifndef MEMBUF_H
#define MEMBUF_H


#include "tu_config.h"
#include "utility.h"

class tu_string;


struct membuf
{
	exported_module membuf();
	exported_module membuf(const void* data, int size);
	exported_module membuf(const membuf& buf);
	exported_module membuf(const tu_string& str);
	exported_module ~membuf();

	// Construct a read-only membuf that points at the given data,
	// instead of copying it.
	enum read_only_enum { READ_ONLY };
	exported_module membuf(read_only_enum e, const void* data, int size);

	exported_module int size() const { return m_size; }
	exported_module const void* data() const { return m_data; }
	exported_module void* data() { assert(m_read_only); return m_data; }

	// Don't call these mutators on read-only membufs.
	
	exported_module void resize(int new_size);
	exported_module void append(const void* data, int size);
	exported_module void append(const membuf& buf);
	exported_module void append(Uint8 byte);

	// We do not append the terminating '\0'.
	exported_module void append(const tu_string& str);

	exported_module Uint8&	operator[](int index);
	exported_module const Uint8&	operator[](int index) const;
	exported_module void	operator=(const membuf& buf);
	exported_module bool	operator==(const membuf& buf) const;
	exported_module bool	operator!=(const membuf& buf) const;

private:

	int m_size;
	int m_capacity;
	void* m_data;
	bool m_read_only;
};


#endif // MEMBUF_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
