// array.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003, Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "as_array.h"
#include "../gameswf_function.h"
#include "../gameswf_character.h"

namespace gameswf
{

	void	as_array_join(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);

		if(a && fn.nargs > 0)
		{
			tu_string result;
			const char * separator = fn.arg(0).to_string();

			int size = a->size();
			for(int index = 0; index < size; ++index )
			{
				as_value _index( index );
				as_value value;
				if( index != 0 ) result += separator;
				a->get_member( _index.to_tu_stringi(), &value );

				result += value.to_tu_string();
			}

			fn.result->set_tu_string( result );
		}
		else
		{
			fn.result->set_undefined();
		}
	}

	void	as_array_tostring(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			fn.result->set_tu_string(a->to_string());
		}
	}

	void	as_array_push(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			if (fn.nargs > 0)
			{
				a->push(fn.arg(0));
			}
			fn.result->set_int(a->size());
		}
	}

	// remove the first item of array
	void	as_array_shift(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			as_value val;
			a->remove(0, &val);
			*fn.result = val;
		}
	}

	// public splice(startIndex:Number, [deleteCount:Number], [value:Object]) : Array
	// adds elements to and removes elements from an array.
	// This method modifies the array without making a copy.
	void	as_array_splice(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a && fn.nargs >= 1)
		{
			int index = fn.arg(0).to_int();
			int asize = a->size();	// for optimization
			if (index >=0 && index < asize)
			{

				// delete items

				as_array* deleted_items = new as_array(a->get_player());
				fn.result->set_as_object(deleted_items);

				int delete_count = asize - index;
				if (fn.nargs >= 2)
				{
					delete_count = fn.arg(1).to_int();
					if (delete_count > asize - index)
					{
						delete_count = asize - index;
					}
				}

				for (int i = 0; i < delete_count; i++)
				{
					as_value val;
					a->remove(index, &val);
					deleted_items->push(val);
				}

				// insert values

				if (fn.nargs >= 3)
				{
					const as_value& val = fn.arg(2);
					as_array* obj = cast_to<as_array>(val.to_object());
					if (obj)
					{
						// insert an array
						for (int i = obj->size() - 1; i >= 0; i--)
						{
							as_value val;
							obj->get_member(tu_string(i), &val);
							a->insert(index, val);
						}
					}
					else
					{
						// insert an item
						a->insert(index, val);
					}
				}
			}
		}
	}

	// public concat([value:Object]) : Array
	// Concatenates the elements specified in the parameters with the elements
	// in an array and creates a new array. 
	// If the value parameters specify an array, the elements of that array are concatenated,
	// rather than the array itself. The array my_array is left unchanged.
	void	as_array_concat(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a && fn.nargs > 0)
		{
			as_array* res = new as_array(a->get_player());
			fn.result->set_as_object(res);

			a->copy_to(res);

			for (int i = 0; i < fn.nargs; i++)
			{
				as_array* arg = cast_to<as_array>(fn.arg(i).to_object());
				if (arg)
				{
					for (int j = 0, n = arg->size(); j < n; j++)
					{
						as_value val;
						arg->get_member(tu_string(j), &val);
						res->push(val);
					}
				}
				else
				{
					res->push(fn.arg(i));
				}
			}
		}
	}

	// public slice([startIndex:Number], [endIndex:Number]) : Array
	// Returns a new array that consists of a range of elements from the original array,
	// without modifying the original array. The returned array includes the startIndex 
	// element and all elements up to, but not including, the endIndex element. 
	void	as_array_slice(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			as_array* res = new as_array(a->get_player());
			fn.result->set_as_object(res);

			// If you don't pass any parameters, a duplicate of the original array is created.
			if (fn.nargs == 0)
			{
				a->copy_to(res);
				return;
			}

			// If start is a negative number, the starting point begins at the end of the array,
			// where -1 is the last element.
			int start = fn.arg(0).to_int();
			if (start < 0)
			{
				start += a->size();
			}

			// If you omit this parameter, the slice includes all elements
			int end = a->size();
			if (fn.nargs >= 2)
			{
				end = fn.arg(1).to_int();
				if (end < 0)
				{
					end += a->size();
				}
			}

			for (int i = start; i < end; i++)
			{
				as_value val;
				if (a->get_member(tu_string(i), &val))
				{
					res->push(val);
				}
			}
		}
	}

	// public unshift(value:Object) : Number
	// Adds one or more elements to the beginning of an array and returns the new length
	void	as_array_unshift(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			for (int i = fn.nargs - 1; i >= 0; i--)
			{
				as_array* arg = cast_to<as_array>(fn.arg(i).to_object());
				if (arg)
				{
					for (int j = arg->size() - 1; j >= 0; j--)
					{
						as_value val;
						if (arg->get_member(tu_string(j), &val))
						{
							a->insert(0, val);
						}
					}
				}
				else
				{
					a->insert(0, fn.arg(i));
				}
			}
			fn.result->set_int(a->size());
		}
	}

	// public sort([compareFunction:Object], [options:Number]) : Array
	// Sorts the elements in an array according to Unicode values.
	void	as_array_sort(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			int options = 0;
			as_function* compare_function = NULL;

			if (fn.nargs > 0)
			{
				if (fn.arg(0).is_number())
				{
					options = fn.arg(0).to_int();
				}
				else
				{
					compare_function = fn.arg(0).to_function();
				}

				if (fn.nargs > 1)
				{
					if (fn.arg(1).is_number())
					{
						options = fn.arg(1).to_int();
					}
					else
					{
						compare_function = fn.arg(1).to_function();
					}
				}
			}
			a->sort(options, compare_function);
		}
	}

	void	as_array_pop(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			as_value val;
			a->pop(&val);
			*fn.result = val;
		}
	}

	void	as_array_length(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		if (a)
		{
			fn.result->set_int(a->size());
		}
	}

	void	as_global_array_ctor(const fn_call& fn)
	// Constructor for ActionScript class Array.
	{
		gc_ptr<as_array>	ao = new as_array(fn.get_player());

		// case of "var x = ["abc","def", 1,2,3,4,..];"
		// called from "init array" operation only
		if (fn.nargs == -1 && fn.first_arg_bottom_index == -1)
		{

			// Use the arguments as initializers.
			int	size = fn.env->pop().to_int();
			as_value	index_number;
			for (int i = 0; i < size; i++)
			{
				index_number.set_int(i);
				ao->set_member(index_number.to_string(), fn.env->pop());
			}
		}
		else

		// case of "var x = new Array(777)"
		if (fn.nargs == 1)
		{
			// Create an empty array with the given number of undefined elements.
			int size = fn.arg(0).to_int();
			as_value	index_number;
			for (int i = 0; i < size; i++)
			{
				index_number.set_int(i);
				ao->set_member(index_number.to_string(), as_value());
			}
		}
		else

		// case of "var x = new Array(1,2,3,4,5,6,7,8,..);"
		{
			assert(fn.env);

			// Use the arguments as initializers.
			as_value	index_number;
			for (int i = 0; i < fn.nargs; i++)
			{
				index_number.set_int(i);
				ao->set_member(index_number.to_string(), fn.arg(i));
			}
		}

		fn.result->set_as_object(ao.get_ptr());
	}

	as_global_array::as_global_array(player* player) :
		as_c_function(player, as_global_array_ctor)
	{
		builtin_member("CASEINSENSITIVE", CASEINSENSITIVE);
		builtin_member("DESCENDING", DESCENDING);
		builtin_member("UNIQUESORT", UNIQUESORT);
		builtin_member("RETURNINDEXEDARRAY ", RETURNINDEXEDARRAY);
		builtin_member("NUMERIC", NUMERIC);
	}

	as_array::as_array(player* player) :
		as_object(player)
	{
		builtin_member("join", as_array_join);
		builtin_member("concat", as_array_concat);
		builtin_member("slice", as_array_slice);
		builtin_member("unshift", as_array_unshift);
		builtin_member("sort", as_array_sort);
		//			this->set_member("sortOn", &array_not_impl);
		//			this->set_member("reverse", &array_not_impl);
		builtin_member("shift", as_array_shift);
		builtin_member("toString", as_array_tostring);
		builtin_member("push", as_array_push);
		builtin_member("pop", as_array_pop);
		builtin_member("length", as_value(as_array_length, as_value()));
		builtin_member("splice", as_array_splice);

		set_ctor(as_global_array_ctor);
	}

	const char* as_array::to_string()
	{
		// receive indexes
		// array may contain not numerical indexes
		array<tu_stringi> idx;
		for (stringi_hash<as_value>::iterator it = m_members.begin(); it != m_members.end(); ++it)
		{
			if (it->second.is_enum())
			{
				idx.push_back(it->first);
			}
		}

		// sort indexes
		int n = idx.size();
		for (int i = 0; i < n - 1; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				if (idx[i] > idx[j])
				{
					tu_swap(&idx[i], &idx[j]);
				}
			}
		}

		// form string
		m_string_value = "";
		for (int i = 0; i < n; i++)
		{
			as_value val;
			as_object::get_member(idx[i], &val);
			m_string_value += val.to_string();

			if (i < n - 1)
			{
				m_string_value +=  ",";
			}
		}

		return m_string_value.c_str();
	}

	int as_array::size()
	// get array size, excluding own methods
	{
		int n = 0;
		for (stringi_hash<as_value>::iterator it = m_members.begin(); it != m_members.end(); ++it)
		{
			if (it->second.is_enum())
			{
				n++;
			}
		}
		return n;
	}

	void as_array::push(const as_value& val)
	// Insert the given value at the end of the array.
	{
		as_value index(size());

		as_value enum_val(val);
		enum_val.set_flags(0);

		set_member(index.to_tu_stringi(), enum_val);
	}

	void as_array::remove(int index, as_value* val)
	// Removes the element starting from 'index' and returns the value of that element.
	{
		assert(val);

		// receive numerical indexes
		array<int> idx;
		for (stringi_hash<as_value>::iterator it = m_members.begin(); it != m_members.end(); ++it)
		{
			int index;
			if (string_to_number(&index, it->first.c_str()) && it->second.is_enum())
			{
				idx.push_back(index);
			}
		}

		if (index >= 0 && index < idx.size())
		{
			// sort indexes
			for (int i = 0, n = idx.size(); i < n - 1; i++)
			{
				for (int j = i + 1; j < n; j++)
				{
					if (idx[i] > idx[j])
					{
						tu_swap(&idx[i], &idx[j]);
					}
				}
			}

			// keep the element starting from 'index'
			get_member(tu_string(idx[index]), val);

			// shift elements
			for (int i = index + 1; i < idx.size(); i++)
			{
				as_value v;
				get_member(tu_string(idx[i]), &v);
				set_member(tu_string(idx[i - 1]), v);
			}

			// remove the last element
			erase(tu_string(idx[idx.size() - 1]));
		}
		else
		{
			//assert(0);
		}
	}


	void as_array::insert(int index, const as_value& val)
	// Inserts the element after 'index'
	{
		// receive numerical indexes
		array<int> idx;
		for (stringi_hash<as_value>::iterator it = m_members.begin(); it != m_members.end(); ++it)
		{
			int index;
			if (string_to_number(&index, it->first.c_str()) && it->second.is_enum())
			{
				idx.push_back(index);
			}
		}

		if (index >= 0 && index < idx.size())
		{
			// add new index
			idx.push_back(idx.size());

			// sort indexes
			for (int i = 0, n = idx.size(); i < n - 1; i++)
			{
				for (int j = i + 1; j < n; j++)
				{
					if (idx[i] > idx[j])
					{
						tu_swap(&idx[i], &idx[j]);
					}
				}
			}

			// shift elements
			for (int i = idx.size() - 1; i > index; i--)
			{
				as_value v;
				get_member(tu_string(idx[i - 1]), &v);
				set_member(tu_string(idx[i]), v);
			}

			// insert
			set_member(tu_string(idx[index]), val);

		}
		else
		{
			//assert(0);
		}
	}



	void as_array::pop(as_value* val)
	// Removes the last element from an array and returns the value of that element.
	{
		assert(val);
		as_value index(size() - 1);
		if (get_member(index.to_tu_stringi(), val))
		{
			erase(index.to_tu_stringi());
		}
	}

	void as_array::erase(const tu_stringi& index)
	// erases one member from array
	{
		m_members.erase(index);
	}

	void as_array::clear()
	// erases all members from array
	{
		for (int i = 0, n = size(); i < n; i++)
		{
			as_value index(i);
			m_members.erase(index.to_tu_stringi());
		}
	}

	void as_array::copy_to(as_object* target)
	// Copy items from 'this' to target
	{
		if (target)
		{
			for (stringi_hash<as_value>::const_iterator it = m_members.begin(); 
				it != m_members.end(); ++it ) 
			{ 
				if (it->second.is_enum())
				{
					target->set_member(it->first, it->second); 
				}
			} 
		}
	}

	// By default, Array.sort() works as described in the following list:
	// Sorting is case-sensitive (Z precedes a). 
	// Sorting is ascending (a precedes b). 
	// Numeric fields are sorted as if they were strings, so 100 precedes 99
	void as_array::sort(int options, as_function* compare_function)
	{
		int n = size();
		for (int i = 0; i < n - 1; i++)
		{
			as_value ival;
			get_member(tu_string(i), &ival);
			for (int j = i + 1; j < n; j++)
			{
				as_value jval;
				get_member(tu_string(j), &jval);

				bool do_swap = false;
				if (compare_function == NULL)
				{
					if (ival.to_tu_string() > jval.to_tu_string())
					{
						do_swap = true;
					}
				}
				else
				{
					// -1, if A should appear before B in the sorted sequence 
					// 0, if A equals B 
					// 1, if A should appear after B in the sorted sequence 

					// use _root environment
					character* mroot = get_player()->get_root_movie();
					as_environment* env = mroot->get_environment();
					
					// keep stack size
					int stack_size = env->get_stack_size();

					env->push(jval);
					env->push(ival);
					as_value ret = call_method(compare_function, env, this, 2, env->get_top_index());

					// restore stack size
					env->set_stack_size(stack_size);

					if (ret.to_int() > 0)
					{
						do_swap = true;
					}
				}

				if (options & as_global_array::DESCENDING)
				{
					do_swap = ! do_swap;
				}

				if (do_swap)
				{
					as_value tmp(ival);
					ival = jval;
					jval = tmp;
					set_member(tu_string(i), ival);
					set_member(tu_string(j), jval);
				}
			}
		}
	}

};
