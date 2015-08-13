// as_class.cpp	-- Julien Hamaide <julien.hamaide@gmail.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script 3 Class object

#include "as_class.h"

namespace gameswf
{

	bool	as_class::find_property( const tu_stringi & name, as_value * val )
	{
		if( get_member( name, val ) )
		{
			return true;
		}

		for (int i = 0; i < m_class->m_trait.size(); i++)
		{
			traits_info* ti = m_class->m_trait[i].get();
			const char* traits_name = m_class->m_abc->get_multiname(ti->m_name);

			if( name == traits_name)
			{
				if(ti->m_kind == traits_info::Trait_Slot)
				{
					as_object * object = new as_object( get_player() );
					set_member(name, object);
					val->set_as_object( object );
					return true;
				}
				return false;
			}
		}

		return as_object::find_property(name, val);
	}
}
