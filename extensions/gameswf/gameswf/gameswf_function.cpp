// gameswf_function.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript function.

#include "gameswf_function.h"
#include "gameswf_log.h"
#include "gameswf_character.h"
#include "gameswf_sprite.h"
#include "gameswf_as_classes/as_array.h"

namespace gameswf
{
	// Invokes the function represented by a Function object.
	// public call(thisObject:Object, [parameter1:Object]) : Object
	void	as_s_function_call(const fn_call& fn)
	{
		assert(fn.this_ptr);
		if (fn.nargs > 0)
		{
			as_object* properties = fn.this_ptr;
			if (properties->m_this_ptr != NULL)
			{
				as_object* func = properties->m_this_ptr.get_ptr();
				if (func)
				{
					as_environment env(fn.get_player());
					int nargs = 0;
					if (fn.nargs > 1)
					{
						nargs = 1;
						env.push(fn.arg(1));
					}

					*fn.result = call_method(func, &env, fn.arg(0).to_object(),
						nargs, env.get_top_index());
				}
			}
		}
	}

	as_s_function::as_s_function(player* player,
		const action_buffer* ab, int start, const swf_array<with_stack_entry>& with_stack) :
		as_function(player),
		m_with_stack(with_stack),
		m_start_pc(start),
		m_length(0),
		m_is_function2(false),
		m_local_register_count(0),
		m_function2_flags(0)
	{
		// Flash stores a body(functions) of a class in _global,
		// action_buffer is in movie_def, therefore when we load another _root
		// action_buffer will be deleted and _global.MyClass will keep
		// the pointer to the removed object(action_buffer).
		// Therefore we have updated action_buffer* ==> action_buffer in as_s_function
		assert(ab);
		m_action_buffer = *ab;

		m_this_ptr = this;

		// any function MUST have prototype
		builtin_member("prototype", new as_object(player));

		builtin_member("call", as_s_function_call);
	}

	as_s_function::~as_s_function()
	{
	}

	void	as_s_function::operator()(const fn_call& fn)
	// Dispatch.
	{

		assert(fn.env);

		// Keep target alive during execution!
		gc_ptr<as_object> target(m_target.get_ptr());

		// try to use caller environment
		// if the caller object has own environment then we use its environment
		as_environment* env = fn.env;
		if (fn.this_ptr)
		{
			if (fn.this_ptr->get_environment())
			{
				env = fn.this_ptr->get_environment();
			}
		}

		// set 'this'
		as_object* this_ptr = env->get_target();
		if (fn.this_ptr)
		{
			this_ptr = fn.this_ptr;
			if (this_ptr->m_this_ptr != NULL)
			{
				this_ptr = this_ptr->m_this_ptr.get_ptr();
			}
		}

		// Function has been declared in moviclip ==> we should use its environment
		// At the same time 'this_ptr' may refers to another object
		// see testcase in .h file
		if (m_target != NULL)
		{
			character* ch = cast_to<character>(m_target.get_ptr());
			if (ch)
			{
				if (ch->is_alive())
				{
					env = m_target->get_environment();
				}
			}
		}

		// Set up local stack frame, for parameters and locals.
		int	local_stack_top = env->get_local_frame_top();
		env->add_frame_barrier();

		if (m_is_function2 == false)
		{
			// Conventional function.

			// Push the arguments onto the local frame.
			int	args_to_pass = imin_swf(fn.nargs, m_args.size());
			for (int i = 0; i < args_to_pass; i++)
			{
				assert(m_args[i].m_register == 0);
				env->add_local(m_args[i].m_name, fn.arg(i));
			}

			env->set_local("this", this_ptr);

			// Put 'super' in a local var.
			if (fn.this_ptr)
			{
				env->add_local("super", fn.this_ptr->get_proto());
			}
		}
		else
		{
			// function2: most args go in registers; any others get pushed.
			
			// Create local registers.
			env->add_local_registers(m_local_register_count);

			// Handle the explicit args.
			int	args_to_pass = imin_swf(fn.nargs, m_args.size());
			for (int i = 0; i < args_to_pass; i++)
			{
				if (m_args[i].m_register == 0)
				{
					// Conventional arg passing: create a local var.
					env->add_local(m_args[i].m_name, fn.arg(i));
				}
				else
				{
					// Pass argument into a register.
					int	reg = m_args[i].m_register;
					env->set_register(reg, fn.arg(i));
				}
			}

			// Handle the implicit args.
			int	current_reg = 1;

			if (m_function2_flags & 0x01)
			{
				// preload 'this' into a register.
				IF_VERBOSE_ACTION(log_msg("-------------- preload this=%p to register %d\n",
					this_ptr, current_reg));
				env->set_register(current_reg, this_ptr);
				current_reg++;

			}

			if (m_function2_flags & 0x02)
			{
				// Don't put 'this' into a local var.
			}
			else
			{
				// Put 'this' in a local var.
				env->add_local("this", as_value(this_ptr));
			}

			// Init arguments array, if it's going to be needed.
			gc_ptr<as_array>	arg_array;
			if ((m_function2_flags & 0x04) || ! (m_function2_flags & 0x08))
			{
				arg_array = new as_array(env->get_player());

				as_value	index_number;
				for (int i = 0; i < fn.nargs; i++)
				{
					index_number.set_int(i);
					arg_array->set_member(index_number.to_string(), fn.arg(i));
				}
			}

			if (m_function2_flags & 0x04)
			{
				// preload 'arguments' into a register.
				env->set_register(current_reg, arg_array.get_ptr());
				current_reg++;
			}

			if (m_function2_flags & 0x08)
			{
				// Don't put 'arguments' in a local var.
			}
			else
			{
				// Put 'arguments' in a local var.
 				env->add_local("arguments", as_value(arg_array.get_ptr()));
			}

			if (m_function2_flags & 0x10)
			{
				// Put 'super' in a register.
				IF_VERBOSE_ACTION(log_msg("-------------- preload super=%p to register %d\n",
					fn.this_ptr->get_proto(), current_reg));
				env->set_register(current_reg, fn.this_ptr->get_proto());
				current_reg++;
			}

			if (m_function2_flags & 0x20)
			{
				// Don't put 'super' in a local var.
			}
			else
			{
				// Put 'super' in a local var.
				env->add_local("super", fn.this_ptr->get_proto());
			}

			if (m_function2_flags & 0x40)
			{
				// Put '_root' in a register.
				env->set_register(current_reg, env->get_root()->get_root_movie());
				current_reg++;
			}

			if (m_function2_flags & 0x80)
			{
				// Put '_parent' in a register.
				swf_array<with_stack_entry>	dummy;
				as_value	parent = env->get_variable("_parent", dummy);
				IF_VERBOSE_ACTION(log_msg("-------------- preload _parent=%p to register %d\n", parent.to_object(), current_reg));
				env->set_register(current_reg, parent);
				current_reg++;
			}

			if (m_function2_flags & 0x100)
			{
				// Put '_global' in a register.
				IF_VERBOSE_ACTION(log_msg("-------------- preload _global=%p to register %d\n", 
					get_global(), current_reg));
				env->set_register(current_reg, get_global());
				current_reg++;
			}
		}

		// keep stack size
		int stack_size = env->get_stack_size();

		// Execute the actions.
		m_action_buffer.execute(env, m_start_pc, m_length, fn.result, m_with_stack, m_is_function2);

		// restore stack size
		// it should not be but it happens
		if (stack_size != env->get_stack_size())
		{
//			log_error("s_function: on entry stack size (%d) != on exit stack size (%d)\n", 
//				stack_size, env->m_stack.size());
			env->set_stack_size(stack_size);
		}

		// Clean up stack frame.
		env->set_local_frame_top(local_stack_top);

		if (m_is_function2)
		{
			// Clean up the local registers.
			env->drop_local_registers(m_local_register_count);
		}
				
	}

	as_c_function::as_c_function(player* player, as_c_function_ptr func) :
		as_function(player),
		m_func(func)
	{
		// any function MUST have prototype
		builtin_member("prototype", new as_object(player));
	}

	void	as_c_function::operator()(const fn_call& fn)
	{
		if (m_func)
		{
			(*m_func)(fn);
		}
	}
}
