// as_date.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// The Date class lets you retrieve date and time values relative to universal time
// (Greenwich mean time, now called universal time or UTC) 
// or relative to the operating system on which Flash Player is running


#ifndef GAMESWF_AS_DATE_H
#define GAMESWF_AS_DATE_H

#include "../gameswf_action.h"	// for as_object
#include "../../base/tu_timer.h"

namespace gameswf
{

	void	as_global_date_ctor(const fn_call& fn);

	struct as_date : public as_object, tu_datetime
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_DATE };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		as_date(const fn_call& fn);
	};

}	// end namespace gameswf


#endif // GAMESWF_AS_DATE_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
