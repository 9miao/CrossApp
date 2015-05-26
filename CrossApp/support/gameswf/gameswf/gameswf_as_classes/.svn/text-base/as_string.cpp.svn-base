// as_string.cpp	  -- Rob Savoye <rob@welcomehome.org> 2005

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Implementation of ActionScript String class.

#include "gameswf/gameswf_as_classes/as_string.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include "base/utf8.h"
#include "gameswf/gameswf_function.h"

namespace gameswf
{

	void string_char_code_at(const fn_call& fn)
	{
		const tu_string& str = fn.this_value.to_tu_string();

		int	index = fn.arg(0).to_int();
		if (index >= 0 && index < str.utf8_length())
		{
			fn.result->set_double(str.utf8_char_at(index));
			return;
		}
		fn.result->set_nan();
	}
  
	void string_concat(const fn_call& fn)
	{
		const tu_string& str = fn.this_value.to_tu_string();

		tu_string result(str);
		for (int i = 0; i < fn.nargs; i++)
		{
			result += fn.arg(i).to_string();
		}

		fn.result->set_tu_string(result);
	}
  
	void string_from_char_code(const fn_call& fn)
	{
		// Takes a variable number of args.  Each arg
		// is a numeric character code.  Construct the
		// string from the character codes.
		tu_string result;
		for (int i = 0; i < fn.nargs; i++)
		{
			uint32 c = (uint32) fn.arg(i).to_number();
			result.append_wide_char(c);
		}

		fn.result->set_tu_string(result);
	}

	void string_index_of(const fn_call& fn)
	{
		const tu_string& sstr = fn.this_value.to_tu_string();

		if (fn.nargs < 1)
		{
			fn.result->set_double(-1);
		}
		else
		{
			int	start_index = 0;
			if (fn.nargs > 1)
			{
				start_index = fn.arg(1).to_int();
			}
			const char*	str = sstr.c_str();
			const char*	p = strstr(
				str + start_index,	// FIXME: not UTF-8 correct!
				fn.arg(0).to_string());
			if (p == NULL)
			{
				fn.result->set_double(-1);
				return;
			}
			fn.result->set_double(tu_string::utf8_char_count(str, (int) (p - str)));
		}
	}


	void string_last_index_of(const fn_call& fn)
	{
		const tu_string& sstr = fn.this_value.to_tu_string();

		if (fn.nargs < 1)
		{
			fn.result->set_double(-1);
		} else {
			int	start_index = 0;
			if (fn.nargs > 1)
			{
				start_index = fn.arg(1).to_int();
			}
			const char* str = sstr.c_str();
			const char* last_hit = NULL;
			const char* haystack = str;
			for (;;) {
				const char*	p = strstr(haystack, fn.arg(0).to_string());
				if (p == NULL || (start_index !=0 && p > str + start_index ) )	// FIXME: not UTF-8 correct!
				{
					break;
				}
				last_hit = p;
				haystack = p + 1;
			}
			if (last_hit == NULL) {
				fn.result->set_double(-1);
			} else {
				fn.result->set_double(tu_string::utf8_char_count(str, (int) (last_hit - str)));
			}
		}
	}
	
	void string_slice(const fn_call& fn)
	{
		const tu_string& this_str = fn.this_value.to_tu_string();

		int len = this_str.utf8_length();
		int start = 0;
		if (fn.nargs >= 1) 
		{
			start = fn.arg(0).to_int();
			if (start < 0)
			{
				start = len + start;
			}
		}
		int end = len;
		if (fn.nargs >= 2)
		{
			end = fn.arg(1).to_int();
			if (end < 0)
			{
				end = len + end;
			}
		}

		start = iclamp(start, 0, len);
		end = iclamp(end, start, len);

		fn.result->set_tu_string(this_str.utf8_substring(start, end));
	}

	void string_split(const fn_call& fn)
	{
		const tu_string& this_str = fn.this_value.to_tu_string();

		gc_ptr<as_array> arr = new as_array(fn.get_player());

		tu_string delimiter;
		if (fn.nargs >= 1)
		{
			delimiter = fn.arg(0).to_string();
		}

		int max_count = this_str.utf8_length();
		if (fn.nargs >= 2)
		{
			max_count = fn.arg(1).to_int();
		}

		const char* p = this_str.c_str();
		const char* word_start = p;
		for (int i = 0; i < max_count; )
		{
			const char* n = p;
			if (delimiter.size() == 0)
			{
				utf8::decode_next_unicode_character(&n);
				if (n == p)
				{
					break;
				}

				tu_string word(p, (int) (n - p));
				as_value val;
				as_value index(i);
				val.set_tu_string(word);
				arr->set_member(index.to_tu_string(), val);
				p = n;
				i++;
			}
			else
			{
				bool match = strncmp(p, delimiter.c_str(), delimiter.size()) == 0;
				if (*p == 0 || match)
				{
					// Emit the previous word.
					tu_string word(word_start, int(p - word_start));
					as_value val;
					as_value index(i);
					val.set_tu_string(word);
					arr->set_member(index.to_tu_string(), val);
					i++;

					if (match)
					{
						// Skip the delimiter.
						p += delimiter.size();
						word_start = p;
					}

					if (*p == 0)
					{
						break;
					}
				} 
				else
				{
					utf8::decode_next_unicode_character(&p);
				}
			}
		}

		fn.result->set_as_object(arr.get_ptr());
	}

	// public substr(start:Number, length:Number) : String
	void string_substr(const fn_call& fn)
	{
		const tu_string& this_str = fn.this_value.to_tu_string();

		if (fn.nargs < 1)
		{
			return;
		}

		// Pull a slice out of this_string.
		int	utf8_len = this_str.utf8_length();
		int	len = utf8_len;

		int start = fn.arg(0).to_int();
		start = iclamp(start, 0, utf8_len);

		if (fn.nargs >= 2)
		{
			len = fn.arg(1).to_int();
			len = iclamp(len, 0, utf8_len);
		}

		if (len <= 0)
		{
			fn.result->set_tu_string("");
			return;
		}

		int end = start + len;
		if (end > utf8_len)
		{
			end = utf8_len;
		}

		if (start < end)
		{
			fn.result->set_tu_string(this_str.utf8_substring(start, end));
		}
	}

	void string_substring(const fn_call& fn)
	{
		const tu_string& this_str = fn.this_value.to_tu_string();

		// Pull a slice out of this_string.
		int	start = 0;
		int	utf8_len = this_str.utf8_length();
		int	end = utf8_len;
		if (fn.nargs >= 1)
		{
			start = fn.arg(0).to_int();
			start = iclamp(start, 0, utf8_len);
		}
		if (fn.nargs >= 2)
		{
			end = fn.arg(1).to_int();
			end = iclamp(end, 0, utf8_len);
		}

		if (end < start) tu_swap(&start, &end);	// dumb, but that's what the docs say
		assert(end >= start);

		fn.result->set_tu_string(this_str.utf8_substring(start, end));
	}

	void string_to_lowercase(const fn_call& fn)
	{
		const tu_string& this_str = fn.this_value.to_tu_string();
		fn.result->set_tu_string(this_str.utf8_to_lower());
	}
	
	void string_to_uppercase(const fn_call& fn) 
	{
		const tu_string& this_str = fn.this_value.to_tu_string();
		fn.result->set_tu_string(this_str.utf8_to_upper());
	}

	void string_char_at(const fn_call& fn)
	{
		const tu_string& this_str = fn.this_value.to_tu_string();

		int	index = fn.arg(0).to_int();
		if (index >= 0 && index < this_str.utf8_length()) 
		{
			char c[2];
			c[0] = this_str.utf8_char_at(index);
			c[1] = 0;
			fn.result->set_tu_string(c);
		}
	}
	
	void string_to_string(const fn_call& fn)
	{
		const tu_string& str = fn.this_value.to_tu_string();
		fn.result->set_tu_string(str);
	}

	void string_length(const fn_call& fn)
	{
		const tu_string& str = fn.this_value.to_tu_string();
		fn.result->set_int(str.size());
	}

	void as_global_string_ctor(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			fn.result->set_string(fn.arg(0).to_string());
		}	
		else
		{
			fn.result->set_string("");
		}
	}

	as_object * get_global_string_ctor(player * player)
	{
		as_object * string = new as_c_function( player, as_global_string_ctor );

		string->builtin_member( "fromCharCode", string_from_char_code );
		//string->builtin_member( "charCodeAt", string_char_at );

		return string;
	}

} // namespace gameswf
