// as_boolean.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf/gameswf_as_classes/as_boolean.h"

namespace gameswf
{

	// boolean(num:Object)
	void	as_global_boolean_ctor(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			fn.result->set_bool(fn.arg(0).to_bool());
		}	
		else
		{
			fn.result->set_bool(false);	// test it
		}
	}

	void	as_boolean_to_string(const fn_call& fn)
	{
		bool val = fn.this_value.to_bool();
		fn.result->set_string(val ? "true" : "false");
	}

	void	as_boolean_valueof(const fn_call& fn)
	{
		assert(0);	//TODO
	}

};
