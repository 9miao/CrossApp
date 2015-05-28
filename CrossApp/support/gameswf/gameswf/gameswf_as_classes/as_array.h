// array.h	-- Thatcher Ulrich <tu@tulrich.com> 2003, Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script Array implementation code for the gameswf SWF player library.


#ifndef GAMESWF_AS_ARRAY_H
#define GAMESWF_AS_ARRAY_H

#include "../gameswf_action.h"	// for as_object
#include "../gameswf_function.h"

namespace gameswf
{

	// constructor of an Array object
	void	as_global_array_ctor(const fn_call& fn);

	// this is an Array object
	struct as_array : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_ARRAY };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		exported_module as_array(player* player);
		exported_module virtual const char* to_string();
		exported_module void push(const as_value& val);
		exported_module void remove(int index, as_value* val);
		exported_module void insert(int index, const as_value& val);
		exported_module void pop(as_value* val);
		exported_module void erase(const tu_stringi& index);
		exported_module void clear();
		exported_module void sort(int options, as_function* compare_function);
		exported_module virtual void copy_to(as_object* target);
		exported_module int size();

		tu_string m_string_value;
	};

	// this is "_global.Array" object
	struct as_global_array : public as_c_function
	{
		enum option
		{
			CASEINSENSITIVE = 1,
			DESCENDING = 2,
			UNIQUESORT = 4,
			RETURNINDEXEDARRAY = 8,
			NUMERIC = 16
		};

		as_global_array(player* player);
	};

}	// end namespace gameswf


#endif // GAMESWF_AS_ARRAY_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
