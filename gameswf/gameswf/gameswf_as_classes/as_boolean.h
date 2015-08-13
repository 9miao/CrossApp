// as_boolean.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.


#ifndef GAMESWF_AS_BOOLEAN_H
#define GAMESWF_AS_BOOLEAN_H

#include "../gameswf_action.h"	// for as_object

namespace gameswf
{

	void	as_global_boolean_ctor(const fn_call& fn);
	void	as_boolean_to_string(const fn_call& fn);
	void	as_boolean_valueof(const fn_call& fn);

}	// end namespace gameswf


#endif // GAMESWF_AS_BOOLEAN_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
