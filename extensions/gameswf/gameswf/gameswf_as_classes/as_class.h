// as_class.h	-- Julien Hamaide <julien.hamaide@gmail.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script 3 Class object

#ifndef GAMESWF_AS_CLASS_H
#define GAMESWF_AS_CLASS_H

#include "../gameswf_object.h"
#include "../gameswf_abc.h"

namespace gameswf
{
	class as_class : public as_object
	{
	public:
		as_class( player * player) : 
			as_object( player )
		{
		}

		void set_class( class_info * info )
		{
			m_class = info;
		}

		exported_module virtual bool	find_property( const tu_stringi & name, as_value * val );

	private:

		gc_ptr<class_info> m_class;
		
	};

}


#endif
