// as_math.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf/gameswf_as_classes/as_math.h"
#include "base/tu_random.h"

namespace gameswf
{
	//
	// math object
	//


#if 0
	// One-argument simple functions.
	#define MATH_WRAP_FUNC1(funcname)							\
	void	math_##funcname(as_value* result, as_object_interface* this_ptr,		\
				as_environment* env, int nargs, int first_arg_bottom_index)	\
	{											\
		double	arg = env->bottom(first_arg_bottom_index).to_number();			\
		result->set_double(funcname(arg));						\
	}
#else
	// One-argument simple functions.
	#define MATH_WRAP_FUNC1(funcname)							\
	void	math_##funcname(const fn_call& fn)						\
	{											\
		double	arg = fn.arg(0).to_number();						\
		fn.result->set_double(funcname(arg));						\
	}
#endif

	MATH_WRAP_FUNC1(fabs);
	MATH_WRAP_FUNC1(acos);
	MATH_WRAP_FUNC1(asin);
	MATH_WRAP_FUNC1(atan);
	MATH_WRAP_FUNC1(ceil);
	MATH_WRAP_FUNC1(cos);
	MATH_WRAP_FUNC1(exp);
	MATH_WRAP_FUNC1(floor);
	MATH_WRAP_FUNC1(log);
	MATH_WRAP_FUNC1(sin);
	MATH_WRAP_FUNC1(sqrt);
	MATH_WRAP_FUNC1(tan);

#if 0
	// Two-argument functions.
	#define MATH_WRAP_FUNC2_EXP(funcname, expr)										\
	void	math_##funcname(as_value* result, as_object_interface* this_ptr, as_environment* env, int nargs, int first_arg_bottom_index)	\
	{															\
		double	arg0 = env->bottom(first_arg_bottom_index).to_number();							\
		double	arg1 = env->bottom(first_arg_bottom_index - 1).to_number();						\
		result->set_double(expr);											\
	}
#else
	// Two-argument functions.
	#define MATH_WRAP_FUNC2_EXP(funcname, expr)										\
	void	math_##funcname(const fn_call& fn)										\
	{															\
		double	arg0 = fn.arg(0).to_number();										\
		double	arg1 = fn.arg(1).to_number();										\
		fn.result->set_double(expr);											\
	}
#endif
	MATH_WRAP_FUNC2_EXP(atan2, (atan2(arg0, arg1)));
	MATH_WRAP_FUNC2_EXP(max, (arg0 > arg1 ? arg0 : arg1));
	MATH_WRAP_FUNC2_EXP(min, (arg0 < arg1 ? arg0 : arg1));
	MATH_WRAP_FUNC2_EXP(pow, (pow(arg0, arg1)));

	// A couple of oddballs.
	void	math_random(const fn_call& fn)
	{
		// Random number between 0 and 1.
		fn.result->set_double(tu_random::next_random() / double(Uint32(0x0FFFFFFFF)));
	}
	void	math_round(const fn_call& fn)
	{
		// round argument to nearest int.
		double	arg0 = fn.arg(0).to_number();
		fn.result->set_double(floor(arg0 + 0.5));
	}
	
	as_object* math_init(player* player)
	{
		// Create built-in math object.
		as_object*	math_obj = new as_object(player);

		// constant
		math_obj->builtin_member("e", 2.7182818284590452354);
		math_obj->builtin_member("ln2", 0.69314718055994530942);
		math_obj->builtin_member("log2e", 1.4426950408889634074);
		math_obj->builtin_member("ln10", 2.30258509299404568402);
		math_obj->builtin_member("log10e", 0.43429448190325182765);
		math_obj->builtin_member("pi", 3.14159265358979323846);
		math_obj->builtin_member("sqrt1_2", 0.7071067811865475244);
		math_obj->builtin_member("sqrt2", 1.4142135623730950488);

		// math methods
		math_obj->builtin_member("abs", math_fabs);
		math_obj->builtin_member("acos", math_acos);
		math_obj->builtin_member("asin", math_asin);
		math_obj->builtin_member("atan", math_atan);
		math_obj->builtin_member("ceil", math_ceil);
		math_obj->builtin_member("cos", math_cos);
		math_obj->builtin_member("exp", math_exp);
		math_obj->builtin_member("floor", math_floor);
		math_obj->builtin_member("log", math_log);
		math_obj->builtin_member("random", math_random);
		math_obj->builtin_member("round", math_round);
		math_obj->builtin_member("sin", math_sin);
		math_obj->builtin_member("sqrt", math_sqrt);
		math_obj->builtin_member("tan", math_tan);
		math_obj->builtin_member("atan2", math_atan2);
		math_obj->builtin_member("max", math_max);
		math_obj->builtin_member("min", math_min);
		math_obj->builtin_member("pow", math_pow);

		return math_obj;
	}

}
