// as_sharedobject.h	-- Julien Hamaide <julien.hamaide@gmail.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script SharedObject implementation code for the gameswf SWF player library.


#ifndef GAMESWF_AS_SHAREOBJECT_H
#define GAMESWF_AS_SHAREOBJECT_H

#include "gameswf/gameswf_action.h"	// for as_object

namespace gameswf
{

	class as_sharedobject : public as_object
	{
		static string_hash<gc_ptr<as_object> > m_local;

	public:

		as_sharedobject( player * player );

		bool	get_member(const tu_stringi& name, as_value* val);

		static gc_ptr<as_object> get_local( const tu_string & name, player * player );

	};

}

#endif //GAMESWF_AS_SHAREOBJECT_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
