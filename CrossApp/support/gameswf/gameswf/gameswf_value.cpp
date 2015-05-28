// gameswf_value.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript value type.

#include "gameswf_value.h"
#include "gameswf.h"
#include "gameswf_root.h"
#include "gameswf_action.h"
#include "gameswf_character.h"
#include "gameswf_function.h"
#include "gameswf_movie_def.h"
#include "gameswf_as_classes/as_number.h"
#include "gameswf_as_classes/as_boolean.h"
#include "gameswf_as_classes/as_string.h"
#include <float.h>

namespace gameswf
{

	bool string_to_number(int* result, const char* str, int base)
	// Utility.  Try to convert str to a number.  If successful,
	// put the result in *result, and return true.  If not
	// successful, put 0 in *result, and return false.
	{
		char* tail = 0;
		*result = strtol(str, &tail, base);
		if (tail == str || *tail != 0)
		{
			// Failed conversion to Number.
			return false;
		}
		return true;
	}

	bool string_to_number(double* result, const char* str)
	// Utility.  Try to convert str to a number.  If successful,
	// put the result in *result, and return true.  If not
	// successful, put 0 in *result, and return false.
	{
		char* tail = 0;
		*result = strtod(str, &tail);
		if (tail == str || *tail != 0)
		{
			// Failed conversion to Number.
			return false;
		}
		return true;
	}

	as_value::as_value(as_object* obj) :
		m_type(OBJECT),
		m_object(obj),
		m_flags(0)
	{
	}


	as_value::as_value(as_s_function* func)	:
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_as_object(func);
	}

	as_value::as_value(const as_value& getter, const as_value& setter) :
		m_type(PROPERTY),
		m_property_target(NULL),
		m_flags(0)
	{
		m_property = new as_property(getter, setter);
	}

	const char*	as_value::to_string() const
	// Conversion to string.
	{
		return to_tu_string().c_str();
	}

	const char*	as_value::to_xstring() const
	// Conversion to string, for debuging
	{
		static char buf[16];
		if (m_type == OBJECT)
		{
			snprintf(buf, 16, "0x%p", m_object.get());
			return buf;
		}
		return to_tu_string().c_str();
	}


	const tu_stringi&	as_value::to_tu_stringi() const
	{
		return reinterpret_cast<const tu_stringi&>(to_tu_string());
	}


	const tu_string&	as_value::to_tu_string() const
	// Conversion to const tu_string&.
	{
		switch (m_type)
		{
			case STRING:
				// don't need to do anything
				break;

			case UNDEFINED:
			{
				// gameswf supports Flash9 only
				m_string = "undefined";

				// Behavior depends on file version.  In
				// version 7+, it's "undefined", in versions
				// 6-, it's "".
//				if (version <= 6)
//				{
//					m_string = "";
//				}
//				else
//				{
//					m_string = "undefined";
//				}
				break;
			}

			case BOOLEAN:
				m_string = m_bool ? "true" : "false";
				break;

			case NUMBER:
				// @@ Moock says if value is a NAN, then result is "NaN"
				// INF goes to "Infinity"
				// -INF goes to "-Infinity"
				if (isnan(m_number))
				{
					m_string = "NaN";
				} 
				else
				{
					char buffer[50];
					snprintf(buffer, 50, "%.14g", m_number);
					m_string = buffer;
				}
				break;

			case OBJECT:
				// Moock says, "the value that results from
				// calling toString() on the object".
				//
				// The default toString() returns "[object
				// Object]" but may be customized.
				if (m_object == NULL)
				{
					m_string = "null";
				}
				else
				{
					m_string = m_object->to_string();
				}
				break;
	
			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				m_string = val.to_tu_string();
				break;
			}

			default:
				assert(0);
		}
		return m_string;
	}

	double	as_value::to_number() const
	// Conversion to double.
	{
		switch (m_type)
		{
			case STRING:
			{
				// @@ Moock says the rule here is: if the
				// string is a valid float literal, then it
				// gets converted; otherwise it is set to NaN.
				//
				// Also, "Infinity", "-Infinity", and "NaN"
				// are recognized.
				double val;
				if (! string_to_number(&val, m_string.c_str()))
				{
					// Failed conversion to Number.
					val = 0.0;	// TODO should be NaN
				}
				return val;
			}

			case NUMBER:
				return m_number;

			case BOOLEAN:
				return m_bool ? 1 : 0;

			case OBJECT:
				if (m_object)
				{
					return m_object->to_number();
				}
	 			// Evan: from my tests
				return 0;
	
			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_number();
			}

			case UNDEFINED:
			{
				return get_nan();
			}

			default:
				return 0.0;
		}
	}


	bool	as_value::to_bool() const
	// Conversion to boolean.
	{
		switch (m_type)
		{
			case STRING:

				// gameswf supports Flash9 only
				return m_string.size() > 0 ? true : false;

				// From Moock
/*				if (get_root()->get_movie_version() >= 7)
				{
					return m_string.size() > 0 ? true : false;
				}

				if (m_string == "false")
				{
					return false;
				}
				else
				if (m_string == "true")
				{
					return true;
				}
				else
				{
					// @@ Moock: "true if the string can
					// be converted to a valid nonzero
					// number".
					//
					// Empty string --> false
					return to_number() != 0.0;
				}*/

			case OBJECT:
				if (m_object)
				{
					return m_object->to_bool();
				}
				return false;

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_bool();
			}

			case NUMBER:
				return m_number != 0;

			case BOOLEAN:
				return m_bool;

			case UNDEFINED:
				return false;

			default:
				assert(0);
		}
		return false;
	}

	
	as_object*	as_value::to_object() const
	// Return value as an object.
	{
		switch (m_type)
		{
			case OBJECT:
				return m_object;

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_object();
			}

			default:
				break;
		}
		return NULL;
	}

	as_function*	as_value::to_function() const
	// Return value as an function.
	{
		switch (m_type)
		{
			case OBJECT:
				return cast_to<as_function>(m_object);

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_function();
			}

			default:
				break;
		}
		return NULL;
	}

	void	as_value::set_as_object(as_object* obj)
	{
		if (m_type != OBJECT || m_object != obj)
		{
			drop_refs();
			m_type = OBJECT;
			m_object = obj;
		}
	}

	void	as_value::operator=(const as_value& v)
	{
		switch (v.m_type)
		{
			case UNDEFINED:
				set_undefined();
				break;

			case NUMBER:
				set_double(v.m_number);
				m_flags = v.m_flags;
				break;

			case BOOLEAN:
				set_bool(v.m_bool);
				m_flags = v.m_flags;
				break;

			case STRING:
				set_tu_string(v.m_string);
				m_flags = v.m_flags;
				break;

			case OBJECT:
				set_as_object(v.m_object);
				m_flags = v.m_flags;
				break;

			case PROPERTY:
				drop_refs(); 
				
				// is binded property ?
				if (v.m_property_target == NULL)
				{
					m_type = PROPERTY;
					m_property = v.m_property;
					m_property_target = NULL;
				}
				else
				{
					v.get_property(this);
				}
				m_flags = v.m_flags;

				break;

			default:
				assert(0);
		}
	}

	bool	as_value::operator==(const as_value& v) const
	// Return true if operands are equal.
	{
		// types don't match
		if (m_type != PROPERTY && v.m_type != PROPERTY && m_type != v.m_type)
		{
			if ((is_undefined() && v.is_null()) || (is_null() && v.is_undefined()))
			{
				return true;
			}
			return false;
		}

		switch (m_type)
		{
			case UNDEFINED:
				return v.m_type == UNDEFINED;

			case STRING:
				return m_string == v.to_tu_string();

			case NUMBER:
				return m_number == v.to_number();

			case BOOLEAN:
				return m_bool == v.to_bool();

			case OBJECT:
				return m_object.get() == v.to_object();

			case PROPERTY:
			{
				as_value prop;
				get_property(&prop);
				return prop == v;
			}

			default:
				assert(0);
				return false;
		}
	}

	
	bool	as_value::operator!=(const as_value& v) const
	// Return true if operands are not equal.
	{
		return ! (*this == v);
	}

	void	as_value::drop_refs()
	// Drop any ref counts we have; this happens prior to changing our value.
	{
		m_object = NULL;
		m_property = NULL;
		m_property_target = NULL;

		m_flags = 0;
	}

	void	as_value::set_property(const as_value& val)
	{
		assert(is_property());
		m_property->set(m_property_target, val);
	}

	// get property of primitive value, like Number
	void as_value::get_property(const as_value& primitive, as_value* val) const
	{
		assert(is_property());
		m_property->get(primitive, val);
	}

	void as_value::get_property(as_value* val) const
	{
		assert(is_property());
		m_property->get(m_property_target, val);
	}

	as_property* as_value::to_property() const
	{
		if (is_property())
		{
			return m_property;
		}
		return NULL;
	}

	const as_object* as_value::get_property_target() const
	{
		if (is_property())
		{
			return m_property_target;
		}
		return NULL;
	}

	void as_value::set_property_target(as_object* target)
	// Sets the target to the given object.
	{
		assert(is_property());
		m_property_target = target;
	}

	as_value::as_value(float val) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_double(val);
	}

	as_value::as_value(int val) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_double(val);
	}

	as_value::as_value(double val) :
		m_type(NUMBER),
		m_number(val),
		m_flags(0)
	{
	}

	void	as_value::set_double(double val)
	{
		drop_refs();
		m_type = NUMBER;
		m_number = val;
	}

	as_value::as_value(bool val) :
		m_type(BOOLEAN),
		m_bool(val),
		m_flags(0)
	{
	}

	void	as_value::set_bool(bool val)
	{
		drop_refs();
		m_type = BOOLEAN;
		m_bool = val;
	}


	bool as_value::is_function() const
	{
		if (m_type == OBJECT)
		{
			return cast_to<as_function>(m_object) ? true : false;
		}
		return false;
	}

	as_value::as_value(as_c_function_ptr func) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_as_c_function(func);
	}

	void	as_value::set_as_c_function(as_c_function_ptr func)
	{
		// c_function object has no pointer to player instance
		set_as_object(new as_c_function(NULL, func));
	}

		
	bool as_value::is_instance_of(const as_function* constructor) const
	{
		switch (m_type)
		{
			case UNDEFINED:
				break;

			case STRING:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_string_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case NUMBER:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_number_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case BOOLEAN:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_boolean_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case OBJECT:
				if (m_object)
				{
					return m_object->is_instance_of(constructor);
				}
				break;

			case PROPERTY:
			{
				// FIXME:
				break;
			}

			default:
				break;

		}
		return false;
	}

	const char*	as_value::type_of() const
	{
		switch (m_type)
		{
			case UNDEFINED:
				return "undefined";

			case STRING:
				return "string";

			case NUMBER:
				return "number";

			case BOOLEAN:
				return "boolean";

			case OBJECT:
				if (m_object)
				{
					return m_object->type_of();
				}
				return "null";

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.type_of();
			}

			default:
				assert(0);

		}
		return 0;
	}


	bool as_value::find_property( const tu_string& name, as_value* val)
	{
		switch (m_type)
		{
			default:
				break;

			case STRING:
			{
				return get_builtin(BUILTIN_STRING_METHOD, name, val);
			}

			case NUMBER:
			{
				return get_builtin(BUILTIN_NUMBER_METHOD, name, val);
			}

			case BOOLEAN:
			{
				return get_builtin(BUILTIN_BOOLEAN_METHOD, name, val);
			}

			case OBJECT:
			{
				if (m_object)
				{
					return m_object->get_member(name, val);
				}
			}
		}
		return false;
	}

	bool as_value::find_property_owner(const tu_string& name, as_value* val)
	{
		as_value dummy;

		switch (m_type)
		{
		default:
			break;

		case STRING:
			{
				if( get_builtin(BUILTIN_STRING_METHOD, name, &dummy) )
				{
					*val = *this;
				}
				else
				{
					return false;
				}
			}

		case NUMBER:
			{
				if( get_builtin(BUILTIN_NUMBER_METHOD, name, &dummy) )
				{
					*val = *this;
				}
				else
				{
					return false;
				}
			}

		case BOOLEAN:
			{
				if( get_builtin(BUILTIN_BOOLEAN_METHOD, name, &dummy) )
				{
					*val = *this;
				}
				else
				{
					return false;
				}
			}

		case OBJECT:
			{
				if (m_object)
				{
					return m_object->find_property(name, val);
				}
			}
		}
		return false;
	}

	void	as_value::set_tu_string(const tu_string& str)
	{
		drop_refs();
		m_type = STRING;
		m_string = str; 
	}
	
	void	as_value::set_string(const char* str)
	{
		drop_refs();
		m_type = STRING;
		m_string = str; 
	}
	
	as_value::as_value(const char* str) :
		m_type(STRING),
		m_string(str),
		m_flags(0)
	{
	}

	as_value::as_value(const wchar_t* wstr)	:
		m_type(STRING),
		m_flags(0)
	{
		// Encode the string value as UTF-8.
		//
		// Is this dumb?  Alternatives:
		//
		// 1. store a tu_wstring instead of tu_string?
		// Bloats typical ASCII strings, needs a
		// tu_wstring type, and conversion back the
		// other way to interface with char[].
		// 
		// 2. store a tu_wstring as a union with
		// tu_string?  Extra complexity.
		//
		// 3. ??
		//
		// Storing UTF-8 seems like a pretty decent
		// way to do it.  Everything else just
		// continues to work.

#if (WCHAR_MAX != MAXLONG)
		tu_string::encode_utf8_from_wchar(&m_string, (const uint16 *)wstr);
#else
# if (WCHAR_MAX != MAXSHORT)
# error "Can't determine the size of wchar_t"
# else
			tu_string::encode_utf8_from_wchar(&m_string, (const uint32 *)wstr);
# endif
#endif
	}

	as_value::as_value() :
		m_type(UNDEFINED),
		m_flags(0)
	{
	}

	as_value::as_value(const as_value& v) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		*this = v;
	}

	bool as_value::abstract_equality_comparison( const as_value & first, const as_value & second )
	{
		if (first.type_of() == second.type_of())
		{
			if( first.is_undefined() ) return true;
			if( first.is_null() ) return true;

			if( first.is_number() )
			{
				double first_number = first.to_number();
				double second_number = second.to_number();
				if( first_number == get_nan() || second_number == get_nan() )
				{
					return false;
				}

				return first_number == second_number;
			}
			else if( first.is_string() )
			{
				return first.to_tu_string() == second.to_tu_string();
			}
			else if( first.is_bool() )
			{
				return first.to_bool() == second.to_bool();
			}

			//13.Return true if x and y refer to the same object or if they refer to objects joined to each other (see
			//13.1.2). Otherwise, return false.
			// TODO: treat joined object

			return first.to_object() == second.to_object();
		}
		else
		{

			if( first.is_null() && second.is_undefined() ) return true;
			if( second.is_null() && first.is_undefined() ) return true;

			if( ( first.is_number() && second.is_string() ) 
				|| (second.is_number() && first.is_string() ) )
			{
				return first.to_number() == second.to_number();
			}

			if( first.is_bool() || second.is_bool() ) return first.to_number() == second.to_number();

			// TODO:20.If Type(x) is either String or Number and Type(y) is Object,
			//return the result of the comparison x == ToPrimitive(y).
			//21.If Type(x) is Object and Type(y) is either String or Number,
			//return the result of the comparison ToPrimitive(x) == y.

			return false;
		}
	}

	//ECMA-262 11.8.5
	as_value as_value::abstract_relational_comparison( const as_value & first, const as_value & second )
	{
		return as_value( first.to_number()  < second.to_number() );
		// todo
	}

	//
	//	as_property
	//

	as_property::as_property(const as_value& getter,	const as_value& setter)
	{
		m_getter = cast_to<as_function>(getter.to_object());
		m_setter = cast_to<as_function>(setter.to_object());
	}

	as_property::~as_property()
	{
	}

	void	as_property::set(as_object* target, const as_value& val)
	{
		if (target != NULL)
		{
			as_environment env(target->get_player());
			env.push(val);
			if (m_setter != NULL)
			{
				gc_ptr<as_object> tar = target;
				(*m_setter.get_ptr())(fn_call(NULL, tar.get_ptr(),	&env, 1, env.get_top_index()));
			}
		}
	}

	void as_property::get(as_object* target, as_value* val) const
	{
		if (target == NULL)
		{
			val->set_undefined();
			return;
		}

		// env is used when m_getter->m_env is NULL
		as_environment env(target->get_player());
		if (m_getter != NULL)
		{
			gc_ptr<as_object> tar = target;
			(*m_getter.get_ptr())(fn_call(val, tar.get_ptr(), &env, 0,	0));
		}
	}

	// call static method
	void as_property::get(const as_value& primitive, as_value* val) const
	{
		if (m_getter != NULL)
		{
			(*m_getter.get_ptr())(fn_call(val, primitive, NULL, 0,	0));
		}
	}

}
