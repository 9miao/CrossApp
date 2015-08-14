// as_textformat.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "as_textformat.h"

namespace gameswf
{

	// TextFormat([font:String], [size:Number], [color:Number], [bold:Boolean], 
	// [italic:Boolean], [underline:Boolean], [url:String], [target:String], 
	// [align:String], [leftMargin:Number], [rightMargin:Number], [indent:Number],
	// [leading:Number])

	static tu_string as_global_textformat_ctor_paramnames[] =
	{"font", "size", "color", "bold", "italic", "underline", "url", "target",
	 "align", "leftMargin", "rightMargin", "indent", "leading"};

	void	as_global_textformat_ctor(const fn_call& fn)
	{
		gc_ptr<as_textformat>	obj = new as_textformat(fn.get_player());

		assert(unsigned(fn.nargs) <= TU_ARRAYSIZE(as_global_textformat_ctor_paramnames));

		// TODO: check parameters type
		for (int i = 0; i < fn.nargs; i++)
		{
			obj->set_member(as_global_textformat_ctor_paramnames[i], fn.arg(i));
		}

		fn.result->set_as_object(obj.get_ptr());
	}

	as_textformat::as_textformat(player* player) :
		as_object(player)
	{
	}


};	// gameswf
