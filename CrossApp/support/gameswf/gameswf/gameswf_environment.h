// gameswf_environment.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript "environment", essentially VM state?

#ifndef GAMESWF_ENVIRONMENT_H
#define GAMESWF_ENVIRONMENT_H

#include "../base/weak_ptr.h"
#include "gameswf.h"
#include "gameswf_value.h"

namespace gameswf
{

	#define GLOBAL_REGISTER_COUNT 4

	struct character;
	struct sprite_instance;
	struct as_object;

	exported_module tu_string get_full_url(const tu_string& workdir, const char* url);

	//
	// with_stack_entry
	//
	// The "with" stack is for Pascal-like with-scoping.

	struct with_stack_entry
	{
		gc_ptr<as_object>	m_object;
		int	m_block_end_pc;
		
		with_stack_entry() :
			m_object(NULL),
			m_block_end_pc(0)
		{
		}

		with_stack_entry(as_object* obj, int end)	:
			m_object(obj),
			m_block_end_pc(end)
		{
		}
	};

	// stack access/manipulation
	// @@ TODO do more checking on these
	struct vm_stack : private array<as_value>
	{
		vm_stack() :
			m_stack_size(0)
		{
		}

		inline as_value&	operator[](int index) 
		{
//			assert(index >= 0 && index < m_stack_size);
			return array<as_value>::operator[](index);
		}

		inline const as_value&	operator[](int index) const 
		{
//			assert(index >= 0 && index < m_stack_size);
			return array<as_value>::operator[](index);
		}

		void reset(const as_value& val)
		{
			(*this)[m_stack_size] = val;
		}

		void reset(const char* val)
		{
			(*this)[m_stack_size].set_string(val);
		}

		void reset(const wchar_t* val)
		{
			(*this)[m_stack_size] = as_value(val);
		}

		void reset(bool val)
		{
			(*this)[m_stack_size].set_bool(val);
		}

		void reset(int val)
		{
			(*this)[m_stack_size].set_int(val);
		}

		void reset(float val)
		{
			(*this)[m_stack_size].set_double(val);
		}

		void reset(double val)
		{
			(*this)[m_stack_size].set_double(val);
		}

		void reset(as_object* val)
		{
			(*this)[m_stack_size].set_as_object(val);
		}

		template<class T>
		void	push(T val) 
		{
			if (m_stack_size < array<as_value>::size())
			{
				// this reduces NEW operators
//				(*this)[m_stack_size] = as_value(val);
				reset(val);
			}
			else
			{
				push_back(as_value(val)); 
			}
			m_stack_size++;
		}

		as_value&	pop();
		exported_module void	drop(int count);
		as_value&	top(int dist) { return (*this)[m_stack_size - 1 - dist]; }
		as_value&	bottom(int index) { return (*this)[index]; }
		inline int	get_top_index() const { return m_stack_size - 1; }
		inline int	size() const { return m_stack_size; }
		void resize(int new_size);
		void clear_refs(gameswf::hash<as_object*, bool>* visited_objects, as_object* this_ptr);

		// return object that contains the property
		as_object* find_property(const char* name);

		// get value of property
		bool get_property(const char* name, as_value* val);

	private:

		int m_stack_size;
	};

	struct as_environment : public vm_stack
	{
		vm_stack m_scope;	// scope stack for AVM2
		as_value	m_global_register[GLOBAL_REGISTER_COUNT];
		array<as_value>	m_local_register;	// function2 uses this
		gc_ptr<as_object>	m_target;

		// For local vars.  Use empty names to separate frames.
		struct frame_slot
		{
			tu_string	m_name;
			as_value	m_value;

			frame_slot() {}
			frame_slot(const tu_string& name, const as_value& val) : m_name(name), m_value(val) {}
		};
		array<frame_slot>	m_local_frames;

		gameswf::weak_ptr<player> m_player;

		exported_module as_environment(player* player);
		exported_module ~as_environment();

		bool	set_member(const tu_stringi& name, const as_value& val);
		bool	get_member(const tu_stringi& name, as_value* val);

		int get_stack_size() const { return size(); }
		void set_stack_size(int n) { resize(n); }

		character*	get_target() const;
		void set_target(character* target);
		void set_target(as_value& target, character* original_target);

		as_value	get_variable(const tu_string& varname, const array<with_stack_entry>& with_stack) const;
		// no path stuff:
		as_value	get_variable_raw(const tu_string& varname, const array<with_stack_entry>& with_stack) const;

		void	set_variable(const tu_string& path, const as_value& val, const array<with_stack_entry>& with_stack);
		// no path stuff:
		void	set_variable_raw(const tu_string& path, const as_value& val, const array<with_stack_entry>& with_stack);

		void	set_local(const tu_string& varname, const as_value& val);
		void	add_local(const tu_string& varname, const as_value& val);	// when you know it doesn't exist.
		void	declare_local(const tu_string& varname);	// Declare varname; undefined unless it already exists.

		// Parameter/local stack frame management.
		int	get_local_frame_top() const { return m_local_frames.size(); }
		void	set_local_frame_top(int t) { assert(t <= m_local_frames.size()); m_local_frames.resize(t); }
		void	add_frame_barrier() { m_local_frames.push_back(frame_slot()); }

		// Local registers.
		void	add_local_registers(int register_count)
		{
			// Flash 8 can have zero register (+1 for zero)
			m_local_register.resize(m_local_register.size() + register_count + 1);
		}
		void	drop_local_registers(int register_count)
		{
			// Flash 8 can have zero register (-1 for zero)
			m_local_register.resize(m_local_register.size() - register_count - 1);
		}

		as_value* get_register(int reg);
		void set_register(int reg, const as_value& val);

		// may be used in outside of class instance
		static bool	parse_path(const tu_string& var_path, tu_string* path, tu_string* var);

		// Internal.
		int	find_local(const tu_string& varname, bool ignore_barrier) const;
		character* load_file(const char* url, const as_value& target, int method = 0);
		as_object*	find_target(const as_value& target) const;
		void clear_refs(gameswf::hash<as_object*, bool>* visited_objects, as_object* this_ptr);
		player* get_player() const;
		root* get_root() const;

		private:

		as_value*	local_register_ptr(int reg);

	};


	// Parameters/environment for C functions callable from ActionScript.
	struct fn_call
	{
		as_value* result;
		as_object* this_ptr;
		const as_value& this_value;	// Number or String or Boolean value
		as_environment* env;
		int nargs;
		int first_arg_bottom_index;

		fn_call(as_value* res_in, const as_value& this_in, as_environment* env_in, int nargs_in, int first_in) :
			result(res_in),
			this_value(this_in),
			env(env_in),
			nargs(nargs_in),
			first_arg_bottom_index(first_in)
		{
			this_ptr = this_in.to_object();
		}

		as_value& arg(int n) const
		// Access a particular argument.
		{
			assert(n < nargs);
			return env->bottom(first_arg_bottom_index - n);
		}

		exported_module player* get_player() const;
		exported_module root* get_root() const;
	};

}

#endif
