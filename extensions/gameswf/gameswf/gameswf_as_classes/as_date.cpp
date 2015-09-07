// as_date.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// The Date class lets you retrieve date and time values relative to universal time
// (Greenwich mean time, now called universal time or UTC) 
// or relative to the operating system on which Flash Player is running

#include "as_date.h"
#include "../gameswf_log.h"
#include "../gameswf_function.h"

namespace gameswf
{

	void	as_global_date_ctor(const fn_call& fn)
	{
		gc_ptr<as_date>	obj = new as_date(fn);
		fn.result->set_as_object(obj.get_ptr());
	}

	// getTime() : Number
	// Returns the number of milliseconds since midnight January 1, 1970, universal time
	void	as_date_gettime(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_double(dt->get_time());
	}

	// setTime(millisecond:Number) : Number
	// Sets the date for the specified Date object in milliseconds since midnight on January 1, 1970, 
	// and returns the new time in milliseconds.
	void	as_date_settime(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		dt->set_time(fn.arg(0).to_number());
		fn.result->set_double(dt->get_time());
	}

	// getDate() : Number
	// Returns the day of the month (an integer from 1 to 31)
	void	as_date_getdate(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::MDAY));
	}

	void	as_date_setdate(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::MDAY, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}

	// getDay() : Number
	// Returns the day of the week (0 for Sunday, 1 for Monday, and so on)
	void	as_date_getday(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::WDAY));
	}

	void	as_date_setday(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::WDAY, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}

	// getFullYear() : Number
	// Returns the full year (a four-digit number, such as 2000)
	void	as_date_getfullyear(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::FULLYEAR));
	}

	void	as_date_setfullyear(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::FULLYEAR, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}

	// getHours() : Number
	// Returns the hour (an integer from 0 to 23)
	void	as_date_gethours(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::HOUR));
	}

	void	as_date_sethours(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::HOUR, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}

	// getMilliseconds() : Number
	// Returns the milliseconds (an integer from 0 to 999)
	void	as_date_getmilli(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(0);	// TODO
	}

	void	as_date_setmilli(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			// TODO
			fn.result->set_double(dt->get_time());
		}
	}

	// getMinutes() : Number
	// Returns the minutes (an integer from 0 to 59)
	void	as_date_getminutes(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::MIN));
	}

	void	as_date_setminutes(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::MIN, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}

	// getMonth() : Number
	// Returns the month (0 for January, 1 for February, and so on)
	void	as_date_getmonth(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::MON));
	}

	// public setMonth(month:Number) : Number
	// Sets the month for the specified Date object in local time and returns the new time in milliseconds.
	void	as_date_setmonth(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::MON, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}


	// getSeconds() : Number
	// Returns the seconds (an integer from 0 to 59)
	void	as_date_getseconds(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::SEC));
	}

	void	as_date_setseconds(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::SEC, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}

	// getYear() : Number
	// Returns the year - 1900
	void	as_date_getyear(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		fn.result->set_int(dt->get(as_date::YEAR));
	}

	// public setYear(year:Number) : Number
	// Sets the year for the specified Date object in local time and returns the new time in milliseconds.
	void	as_date_setyear(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);
		if (fn.nargs > 0)
		{
			dt->set(as_date::YEAR, fn.arg(0).to_int());
			fn.result->set_double(dt->get_time());
		}
	}

	// Date([yearOrTimevalue:Number], [month:Number], [date:Number],
	// [hour:Number], [minute:Number], [second:Number], [millisecond:Number])
	as_date::as_date(const fn_call& fn) :
		as_object(fn.get_player())
	{
		// reset if there are args
		if (fn.nargs > 0)
		{
			int arg = fn.arg(0).to_int();
			set(FULLYEAR, arg);
			if (fn.nargs > 1)
			{
				arg = fn.arg(1).to_int();
				set(MON, arg);
				if (fn.nargs > 2)
				{
					arg = fn.arg(2).to_int();
					set(MDAY, arg);
					if (fn.nargs > 3)
					{
						arg = fn.arg(3).to_int();
						set(HOUR, arg);
						if (fn.nargs > 4)
						{
							arg = fn.arg(4).to_int();
							set(MIN, arg);
							if (fn.nargs > 5)
							{
								arg = fn.arg(5).to_int();
								set(SEC, arg);
							}
						}
					}
				}
			}
		}

		builtin_member("getTime", as_date_gettime);
		builtin_member("setTime", as_date_settime);

		builtin_member("getYear", as_date_getyear);
		builtin_member("setYear", as_date_setyear);

		builtin_member("getMonth", as_date_getmonth);
		builtin_member("setMonth", as_date_setmonth);

		builtin_member("getDate", as_date_getdate);
		builtin_member("setDate", as_date_setdate);

		builtin_member("getDay", as_date_getday);
		builtin_member("setDay", as_date_setday);

		builtin_member("getFullYear", as_date_getfullyear);
		builtin_member("setFullYear", as_date_setfullyear);

		builtin_member("getHours", as_date_gethours);
		builtin_member("setHours", as_date_sethours);

		builtin_member("getMilliseconds", as_date_getmilli);
		builtin_member("setMilliseconds", as_date_setmilli);

		builtin_member("getMinutes", as_date_getminutes);
		builtin_member("setMinutes", as_date_setminutes);

		builtin_member("getSeconds", as_date_getseconds);
		builtin_member("setSeconds", as_date_setseconds);

	}

};
