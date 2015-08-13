// as_number.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "as_number.h"
#include "as_string.h"
#include "../gameswf_log.h"

namespace gameswf
{

	// static builtins methods of Number class

	static const char s_hex_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVXYZW";

	void as_global_parse_float(const fn_call& fn)
	{
		if (fn.nargs > 0)  
		{
			double res;
			if (string_to_number(&res, fn.arg(0).to_string()))
			{
				fn.result->set_double(res);
				return;
			}
		}
		fn.result->set_nan();
	}

	void as_global_parse_int(const fn_call& fn)
	{
		if (fn.nargs > 0)
		{
			int res;
			int base = fn.nargs > 1 ? fn.arg(1).to_int() : 10;
			if (string_to_number(&res, fn.arg(0).to_string(), base))
			{
				fn.result->set_int(res);
				return;
			}
		}
		fn.result->set_nan();
	}

	void as_global_isnan(const fn_call& fn)
	{
		if (fn.nargs == 1)  
		{
			if (isnan(fn.arg(0).to_number()) == false)
			{
				fn.result->set_bool(false);
				return;
			}
		}
		fn.result->set_bool(true);
	}

	// Number(num:Object)
	void	as_global_number_ctor(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			fn.result->set_double(fn.arg(0).to_number());
		}	
		else
		{
			fn.result->set_double(0);
		}
	}

	void	as_number_to_string(const fn_call& fn)
	{
		double number = fn.this_value.to_number();
		if (fn.nargs >= 1)
		{
			// radix:Number - Specifies the numeric base (from 2 to 36) to use for 
			// the number-to-string conversion. 
			// If you do not specify the radix parameter, the default value is 10.

			tu_string res;
			int val = (int) number;
			int radix = fn.arg(0).to_int();
			if (radix >= 2 && radix <= (int) strlen(s_hex_digits))
			{
				do
				{
					int k = val % radix;
					val = (int) (val / radix);
					tu_string digit;
					digit += s_hex_digits[k];
					res = digit + res;
				}
				while (val > 0);
			}
			fn.result->set_tu_string(res);
		}
		else
		{
			// @@ Moock says if value is a NAN, then result is "NaN"
			// INF goes to "Infinity"
			// -INF goes to "-Infinity"
			if (isnan(number))
			{
				fn.result->set_string("NaN");
			} 
			else
			{
				char buffer[50];
				snprintf(buffer, 50, "%.14g", number);
				fn.result->set_string(buffer);
			}
		}
	}

	void	as_number_valueof(const fn_call& fn)
	{
		assert(0);	//TODO
	}

};
