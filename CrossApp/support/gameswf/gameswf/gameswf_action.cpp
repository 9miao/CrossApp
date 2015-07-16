// gameswf_action.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Implementation and helpers for SWF actions.


#include "gameswf_action.h"
#include "gameswf_impl.h"
#include "gameswf_log.h"
#include "gameswf_stream.h"
#include "gameswf_movie_def.h"
#include "gameswf_sprite.h"
#include "gameswf_function.h"
#include "gameswf_freetype.h"
#include "gameswf_player.h"
#include "gameswf_disasm.h"
#include "../base/tu_random.h"
#include "../base/tu_timer.h"
#include "../base/tu_loadlib.h"

#include "gameswf_as_classes/as_array.h"
#include "gameswf_as_classes/as_string.h"

// NOTES:
//
// Buttons
// on (press)				 onPress
// on (release)			   onRelease
// on (releaseOutside)		onReleaseOutside
// on (rollOver)			  onRollOver
// on (rollOut)			   onRollOut
// on (dragOver)			  onDragOver
// on (dragOut)			   onDragOut
// on (keyPress"...")		 onKeyDown, onKeyUp	  <----- IMPORTANT
//
// Sprites
// onClipEvent (load)		 onLoad
// onClipEvent (unload)	   onUnload				Hm.
// onClipEvent (enterFrame)   onEnterFrame
// onClipEvent (mouseDown)	onMouseDown
// onClipEvent (mouseUp)	  onMouseUp
// onClipEvent (mouseMove)	onMouseMove
// onClipEvent (keyDown)	  onKeyDown
// onClipEvent (keyUp)		onKeyUp
// onClipEvent (data)		 onData

// Text fields have event handlers too!

// Sprite built in methods:
// play()
// stop()
// gotoAndStop()
// gotoAndPlay()
// nextFrame()
// startDrag()
// getURL()
// getBytesLoaded()
// getBytesTotal()

// TODO builtins
// Number.MAX_VALUE, Number.MIN_VALUE
// String.fromCharCode()


namespace gameswf
{
	
	//
	// action stuff
	//

	void	as_global_trace(const fn_call& fn);

	//
	// Function/method dispatch.
	//

	as_value	call_method(
		as_function* func,
		as_environment* env,
		const as_value& this_ptr,
		int nargs,
		int first_arg_bottom_index)
	// first_arg_bottom_index is the stack index, from the bottom, of the first argument.
	// Subsequent arguments are at *lower* indices.  E.g. if first_arg_bottom_index = 7,
	// then arg1 is at env->bottom(7), arg2 is at env->bottom(6), etc.
	{
		as_value	val;
		if (func)
		{
			// It's a function.  Call it.
			(*func)(fn_call(&val, this_ptr, env, nargs, first_arg_bottom_index));
		}
		else
		{
			IF_VERBOSE_ACTION(log_error("error in call_method(): method is not a function\n"));
		}

		return val;
	}

	as_value	call_method(
		const as_value& method,
		as_environment* env,
		const as_value& this_ptr,
		int nargs,
		int first_arg_bottom_index)
	// first_arg_bottom_index is the stack index, from the bottom, of the first argument.
	// Subsequent arguments are at *lower* indices.  E.g. if first_arg_bottom_index = 7,
	// then arg1 is at env->bottom(7), arg2 is at env->bottom(6), etc.
	{
		as_value	val;
		as_function*	func = method.to_function();
		if (func)
		{
			// It's a function.  Call it.
			(*func)(fn_call(&val, this_ptr, env, nargs, first_arg_bottom_index));
		}
		else
		{
			IF_VERBOSE_ACTION(log_error("error in call_method(): method is not a function\n"));
		}

		return val;
	}

	const char*	call_method_parsed(
		as_environment* env,
		as_object* this_ptr,
		const char* method_name,
		const char* method_arg_fmt,
		va_list args)
	// Printf-like vararg interface for calling ActionScript.
	// Handy for external binding.
	{
		// Parse va_list args
		int	starting_index = env->get_top_index();
		const char* p = method_arg_fmt;
		for (;; p++)
		{
			char	c = *p;
			if (c == 0)
			{
				// End of args.
				break;
			}
			else if (c == '%')
			{
				p++;
				c = *p;
				// Here's an arg.
				if (c == 'd')
				{
					// Integer.
					env->push(va_arg(args, int));
				}
				else if (c == 'f')
				{
					// Double
					env->push(va_arg(args, double));
				}
				else if (c == 's')
				{
					// String
					env->push(va_arg(args, const char *));
				}
				else if (c == 'l')
				{
					p++;
					c = *p;
					if (c == 's')
					{
						// Wide string.
						env->push(va_arg(args, const wchar_t *));
					}
					else
					{
						log_error("call_method_parsed('%s','%s') -- invalid fmt '%%l%c'\n",
							  method_name,
							  method_arg_fmt,
							  c);
					}
				}
				else
				{
					// Invalid fmt, warn.
					log_error("call_method_parsed('%s','%s') -- invalid fmt '%%%c'\n",
						  method_name,
						  method_arg_fmt,
						  c);
				}
			}
			else
			{
				// Ignore whitespace and commas.
				if (c == ' ' || c == '\t' || c == ',')
				{
					// OK
				}
				else
				{
					// Invalid arg; warn.
					log_error("call_method_parsed('%s','%s') -- invalid char '%c'\n",
						  method_name,
						  method_arg_fmt,
						  c);
				}
			}
		}

		array<with_stack_entry>	dummy_with_stack;
		as_value	method = env->get_variable(method_name, dummy_with_stack);

		// check method

		// Reverse the order of pushed args
		int	nargs = env->get_top_index() - starting_index;
		for (int i = 0; i < (nargs >> 1); i++)
		{
			int	i0 = starting_index + 1 + i;
			int	i1 = starting_index + nargs - i;
			assert(i0 < i1);

			tu_swap(&(env->bottom(i0)), &(env->bottom(i1)));
		}

		// Do the call.
		as_value	result = call_method(method, env, this_ptr, nargs, env->get_top_index());
		env->drop(nargs);

		// Return pointer to static string for return value.
		static tu_string	s_retval;
		s_retval = result.to_tu_string();
		return s_retval.c_str();
	}
	
	tu_string	call_method(
		as_environment* env,
		as_object* this_ptr,
		const char* method_name,
		const as_value* arguments,
		int argument_count
		)
	{
		// Parse va_list args
		int	starting_index = env->get_top_index();
		for (int i = argument_count - 1 ; i >= 0 ; i--)
		{
			env->push( arguments[ i ] );
		}

		array<with_stack_entry>	dummy_with_stack;
		as_value	method = env->get_variable(method_name, dummy_with_stack);

		// check method

		int	nargs = env->get_top_index() - starting_index;

		// Do the call.
		as_value	result = call_method(method, env, this_ptr, nargs, env->get_top_index());
		env->drop(nargs);

		return result.to_tu_string();
	}

	//
	// do_action
	//


	// Thin wrapper around action_buffer.
	struct do_action : public execute_tag
	{
		action_buffer	m_buf;

		void	read(stream* in)
		{
			m_buf.read(in);
		}

		virtual void	execute(character* m)
		{
			m->add_action_buffer(&m_buf);
		}

		// Don't override because actions should not be replayed when seeking the movie.
		//void	execute_state(character* m) {}

		virtual bool	is_action_tag() const
		// Tell the caller that we are an action tag.
		{
			return true;
		}
	};

	void	do_action_loader(stream* in, int tag_type, movie_definition_sub* m)
	{
		IF_VERBOSE_PARSE(log_msg("tag %d: do_action_loader\n", tag_type));

		IF_VERBOSE_ACTION(log_msg("-------------- actions in frame %d\n", m->get_loading_frame()));

		assert(in);
		assert(tag_type == 12);
		assert(m);
		
		do_action*	da = new do_action;
		da->read(in);

		m->add_execute_tag(da);
	}

	
	//
	// do_init_action
	//


	void	do_init_action_loader(stream* in, int tag_type, movie_definition_sub* m)
	{
		assert(tag_type == 59);

		int	sprite_character_id = in->read_u16();

		IF_VERBOSE_PARSE(log_msg("  tag %d: do_init_action_loader\n", tag_type));

		IF_VERBOSE_ACTION(log_msg("  -- init actions for sprite %d\n", sprite_character_id));

		do_action*	da = new do_action;
		da->read(in);
		m->add_init_action(sprite_character_id, da);
	}

	//
	// action_buffer
	//

	action_buffer::action_buffer() :
		m_buffer(new counted_buffer),
		m_decl_dict_processed_at(-1)
	{
	}


	void	action_buffer::read(stream* in)
	{
		// Read action bytes.
		for (;;)
		{

			int	instruction_start = m_buffer->size();

			int	pc = m_buffer->size();

			int	action_id = in->read_u8();
			m_buffer->append((Uint8) action_id);

			if (action_id & 0x80)
			{
				// Action contains extra data.  Read it.
				int	length = in->read_u16();
				m_buffer->append(Uint8(length & 0x0FF));
				m_buffer->append(Uint8((length >> 8) & 0x0FF));
				for (int i = 0; i < length; i++)
				{
					Uint8	b = in->read_u8();
					m_buffer->append(b);
				}
			}

			IF_VERBOSE_ACTION(log_msg("%4d\t", pc); log_disasm(&(*m_buffer.get_ptr())[instruction_start]); );

			if (action_id == 0)
			{
				// end of action buffer.
				break;
			}
		}
	}


	void	action_buffer::process_decl_dict(int start_pc, int stop_pc)
	// Interpret the decl_dict opcode.  Don't read stop_pc or
	// later.  A dictionary is some static strings embedded in the
	// action buffer; there should only be one dictionary per
	// action buffer.
	//
	// NOTE: Normally the dictionary is declared as the first
	// action in an action buffer, but I've seen what looks like
	// some form of copy protection that amounts to:
	//
	// <start of action buffer>
	//          push true
	//          branch_if_true label
	//          decl_dict   [0]   // this is never executed, but has lots of orphan data declared in the opcode
	// label:   // (embedded inside the previous opcode; looks like an invalid jump)
	//          ... "protected" code here, including the real decl_dict opcode ...
	//          <end of the dummy decl_dict [0] opcode>
	//

	// So we just interpret the first decl_dict we come to, and
	// cache the results.  If we ever hit a different decl_dict in
	// the same action_buffer, then we log an error and ignore it.
	{
		assert(stop_pc <= m_buffer->size());

		membuf &buffer = *m_buffer.get_ptr();

		if (m_decl_dict_processed_at == start_pc)
		{
			// We've already processed this decl_dict.
			int	count = buffer[start_pc + 3] | (buffer[start_pc + 4] << 8);
			assert(m_dictionary.size() == count);
			UNUSED(count);
			return;
		}

		if (m_decl_dict_processed_at != -1)
		{
			log_error("error: process_decl_dict(%d, %d): decl_dict was already processed at %d\n",
				  start_pc,
				  stop_pc,
				  m_decl_dict_processed_at);
			return;
		}

		m_decl_dict_processed_at = start_pc;

		// Actual processing.
		int	i = start_pc;
		int	length = buffer[i + 1] | (buffer[i + 2] << 8);
		int	count = buffer[i + 3] | (buffer[i + 4] << 8);
		i += 2;

		UNUSED(length);

		assert(start_pc + 3 + length == stop_pc);

		m_dictionary.resize(count);


		// Index the strings.
		for (int ct = 0; ct < count; ct++)
		{
			// Point into the current action buffer.
			m_dictionary[ct] = (const char*) &buffer[3 + i];

			while (buffer[3 + i])
			{
				// safety check.
				if (i >= stop_pc)
				{
					log_error("error: action buffer dict length exceeded\n");

					// Jam something into the remaining (invalid) entries.
					while (ct < count)
					{
						m_dictionary[ct] = "<invalid>";
						ct++;
					}
					return;
				}
				i++;
			}
			i++;
		}
	}

	void	action_buffer::execute(as_environment* env)
	// Interpret the actions in this action buffer, and evaluate
	// them in the given environment.  Execute our whole buffer,
	// without any arguments passed in.
	{
		// Flash keeps local variables after executing of frame actions
		// therefore we should not delete them
//		int	local_stack_top = env->get_local_frame_top();
//		env->add_frame_barrier();

		// sanity check
		assert(env->m_local_frames.size() < 1000);


		array<with_stack_entry>	empty_with_stack;
		execute(env, 0, m_buffer->size(), NULL, empty_with_stack, false /* not function2 */);

//		env->set_local_frame_top(local_stack_top);

	}


	// local function, called from 0x46 & 0x55 opcode implementation only
	// static
	void action_buffer::enumerate(as_environment* env, as_object* obj)
	{
		assert(env);

		// The end of the enumeration
		as_value nullvalue;
		nullvalue.set_null();
		env->push(nullvalue);
		IF_VERBOSE_ACTION(log_msg("-------------- enumerate - push: NULL\n"));

		if (obj == NULL)
		{
			return;
		}

		obj->enumerate(env);
	}

	as_object* action_buffer::load_as_plugin(player* player, const tu_string& classname,
		 const array<as_value>& params)
	// loads user defined class from DLL / shared library
	{
		// look first in app registered
		gameswf_module_init module_init = find_type_handler(classname);

		if (module_init == NULL)
		{
			// try through DLLs
			tu_loadlib* lib = NULL;
			if (get_shared_libs()->get(classname, &lib) == false)
			{
				lib = new tu_loadlib(classname.c_str());
				get_shared_libs()->add(classname, lib);
			}
		
			assert(lib);

			// get module interface
			module_init = (gameswf_module_init) lib->get_function("gameswf_module_init");
		}

		// create plugin instance
		if (module_init)
		{
			return (module_init)(player, params);
		}

		return NULL;
	}
	
	void action_buffer::operator=(const action_buffer& ab)
	{
		// TODO: optimize
		m_buffer = ab.m_buffer;
		m_dictionary = ab.m_dictionary;
		m_decl_dict_processed_at = ab.m_decl_dict_processed_at;
	}

	void	action_buffer::execute(
		as_environment* env,
		int start_pc,
		int exec_bytes,
		as_value* retval,
		const array<with_stack_entry>& initial_with_stack,
		bool is_function2) const
	// Interpret the specified subset of the actions in our
	// buffer.  Caller is responsible for cleaning up our local
	// stack frame (it may have passed its arguments in via the
	// local stack frame).
	// 
	// The is_function2 flag determines whether to use global or local registers.
	{
		// for debugging action script
		// keep the latest var name(to log it if call_method failure)
		tu_string last_varname;

		assert(env);
		array<with_stack_entry>	with_stack(initial_with_stack);
	
		character*	original_target = env->get_target();
		membuf & buffer = *m_buffer.get_ptr();

		int	stop_pc = start_pc + exec_bytes;
		for (int pc = start_pc; pc < stop_pc; )
		{
			// Cleanup any expired "with" blocks.
			while (with_stack.size() > 0
				   && pc >= with_stack.back().m_block_end_pc)
			{
				// Drop this stack element
				with_stack.resize(with_stack.size() - 1);
			}
			
#if ACTION_BUFFER_PROFILLING
			Uint64 start_time;
			start_time = tu_timer::get_profile_ticks();
#endif

			// Get the opcode.
			int	action_id = buffer[pc];
			if ((action_id & 0x80) == 0)
			{
				IF_VERBOSE_ACTION(log_msg("EX:\t"); log_disasm(&buffer[pc]));

				// IF_VERBOSE_ACTION(log_msg("Action ID is: 0x%x\n", action_id));
			
				// Simple action; no extra data.
				switch (action_id)
				{
				default:
					break;

				case 0x00:	// end of actions.
					return;

				case 0x04:	// next frame.
					if (env->get_target() != NULL)
					{
						env->get_target()->goto_frame(env->get_target()->get_current_frame() + 1);
					}
					break;

				case 0x05:	// prev frame.
					if (env->get_target() != NULL)
					{
						env->get_target()->goto_frame(env->get_target()->get_current_frame() - 1);
					}
					break;

				case 0x06:	// action play
					if (env->get_target() != NULL)
					{
						env->get_target()->set_play_state(character::PLAY);
					}
					break;

				case 0x07:	// action stop
					if (env->get_target() != NULL)
					{
						env->get_target()->set_play_state(character::STOP);
					}
					break;

				case 0x08:	// toggle quality
					// @@ TODO
					log_error("todo opcode: %02X\n", action_id);
					break;

				case 0x09:	// stop sounds
				{
					sound_handler* s = get_sound_handler();
					if (s != NULL)
					{
						s->stop_all_sounds();
					}
					break;
				}

				case 0x0A:	// add
				{
					env->top(1) += env->top(0).to_number();
					env->drop(1);
					break;
				}
				case 0x0B:	// subtract
				{
					env->top(1) -= env->top(0).to_number();
					env->drop(1);
					break;
				}
				case 0x0C:	// multiply
				{
					env->top(1) *= env->top(0).to_number();
					env->drop(1);
					break;
				}
				case 0x0D:	// divide
				{
					env->top(1) /= env->top(0).to_number();
					env->drop(1);
					break;
				}
				case 0x0E:	// equal
				{
					env->top(1).set_bool(env->top(1) == env->top(0));
					env->drop(1);
					break;
				}
				case 0x0F:	// less than
				{
					env->top(1).set_bool(env->top(1) < env->top(0).to_number());
					env->drop(1);
					break;
				}
				case 0x10:	// logical and
				{
					env->top(1).set_bool(env->top(1).to_bool() && env->top(0).to_bool());
					env->drop(1);
					break;
				}
				case 0x11:	// logical or
				{
					env->top(1).set_bool(env->top(1).to_bool() || env->top(0).to_bool());
					env->drop(1);
					break;
				}
				case 0x12:	// logical not
				{
					env->top(0).set_bool(! env->top(0).to_bool());
					break;
				}
				case 0x13:	// string equal
				{
					env->top(1).set_bool(env->top(1).to_tu_string() == env->top(0).to_tu_string());
					env->drop(1);
					break;
				}
				case 0x14:	// string length
				{
					env->top(0).set_int(env->top(0).to_tu_string().utf8_length());
					break;
				}
				case 0x15:	// substring
				{
					int	size = env->top(0).to_int();
					int	base = env->top(1).to_int() - 1;  // 1-based indices
					const tu_string&	str = env->top(2).to_tu_string();

					// Keep base within range.
					base = iclamp(base, 0, str.length());

					// Truncate if necessary.
					size = imin_swf(str.length() - base, size);


					// @@ This can be done without new allocations if we get dirtier w/ internals
					// of as_value and tu_string...
					tu_string	new_string = str.c_str() + base;
					new_string.resize(size);

					env->drop(2);
					env->top(0).set_tu_string(new_string);

					break;
				}
				case 0x17:	// pop
				{
					env->drop(1);
					break;
				}
				case 0x18:	// int
				{
					env->top(0).set_int(int(floor(env->top(0).to_number())));
					break;
				}
				case 0x1C:	// get variable
				{
					const tu_string var_string = env->top(0).to_tu_string();
					
					// keep the latest var name(to log it if call_method failure)
					last_varname = var_string;

					as_value variable = env->get_variable(var_string, with_stack);
					env->top(0) = variable;

					if (variable.to_object() == NULL) 
					{
						IF_VERBOSE_ACTION(log_msg("-------------- get var: %s=%s\n",
									  var_string.c_str(),
									  variable.to_tu_string().c_str()));
					} 
					else
					{
						IF_VERBOSE_ACTION(log_msg("-------------- get var: %s=%s at %p\n",
									  var_string.c_str(),
									  variable.to_tu_string().c_str(), variable.to_object()));
					}
					break;
				}
				case 0x1D:	// set variable
				{
					env->set_variable(env->top(1).to_tu_string(), env->top(0), with_stack);
					IF_VERBOSE_ACTION(log_msg("-------------- set var: %s \n",
								  env->top(1).to_tu_string().c_str()));

					env->drop(2);
					break;
				}
				case 0x20:	// set target expression
				{
					// Flash doc says:
					// This action behaves exactly like the original ActionSetTarget from SWF 3, 
					// but is stack-based to enable the target path to be the result of expression evaluation.
					env->set_target(env->top(0), original_target);
					env->drop(1);
					break;
				}
				case 0x21:	// string concat
				{
					tu_string str = env->top(1).to_tu_string();
					str += env->top(0).to_tu_string();
					env->drop(1);
					env->top(0).set_tu_string(str);
					break;
				}
				case 0x22:	// get property
				{

					character*	target = cast_to<character>(env->find_target(env->top(1)));
					if (target)
					{
						env->top(1) = get_property(target, env->top(0).to_int());
					}
					else
					{
						env->top(1) = as_value();
					}
					env->drop(1);
					break;
				}

				case 0x23:	// set property
				{

					character*	target = cast_to<character>(env->find_target(env->top(2)));
					if (target)
					{
						set_property(target, env->top(1).to_int(), env->top(0));
					}
					env->drop(3);
					break;
				}

				case 0x24:
				// duplicateMovieClip(target:String, newname:String, depth:Number) : Void
				// duplicateMovieClip(target:MovieClip, newname:String, depth:Number) : Void
				{
					character* target = cast_to<character>(env->top(2).to_object());

					// try string
					if (target == NULL)
					{
						as_value val = env->get_variable(env->top(2).to_string(), with_stack);
						target = cast_to<character>(val.to_object());
					}

					if (target)
					{
						// Flash automatically adds 16384 to depth therefore
						// we don't apply ADJUST_DEPTH_VALUE
						target->clone_display_object(env->top(1).to_tu_string(), env->top(0).to_int());
					}

					env->drop(3);
					break;
				}

				case 0x25:	// remove clip
				{
					//	removeMovieClip(target:Object), deletes the specified movie clip.
					character*	target = cast_to<character>(env->find_target(env->top(0)));
					if (target)
					{
						sprite_instance* parent = cast_to<sprite_instance>(target->get_parent());
						if (parent)
						{
							parent->remove_display_object(target);
						}
					}
					env->drop(1);
					break;
				}

				case 0x26:	// trace
				{
					// Log the stack val.
					as_global_trace(fn_call(&env->top(0), as_value(), env, 1, env->get_top_index()));
					env->drop(1);
					break;
				}

				case 0x27:	// start drag movie
				{
					character::drag_state st;

					character* targetChar = cast_to<character>(env->find_target(env->top(0)));
					if (targetChar == NULL)
					{
						log_error("error: start_drag of invalid target '%s'.\n",  env->top(0).to_string());
					}

					st.SetCharacter(targetChar);
					st.SetLockCentered(env->top(1).to_bool());
					bool hasBounds = env->top(2).to_bool();
					if (hasBounds)
					{
						float bx0 = PIXELS_TO_TWIPS(env->top(6).to_float());
						float by0 = PIXELS_TO_TWIPS(env->top(5).to_float());
						float bx1 = PIXELS_TO_TWIPS(env->top(4).to_float());
						float by1 = PIXELS_TO_TWIPS(env->top(3).to_float());
						if (bx1 < bx0)
						{
							tu_swap(&bx0, &bx1);
						}
						if (by1 < by0)
						{
							tu_swap(&by0, &by1);
						}
						st.SetBounds(bx0, by0, bx1, by1);

						env->drop(4);
					}
					env->drop(3);

					if (targetChar)
					{
						root* swfRoot = env->get_root();
						assert( swfRoot );
						swfRoot->set_drag_state(st);
					}					
					break;
				}

				case 0x28:	// stop drag movie
				{
					character* targetChar = env->get_target();
					if (targetChar)
					{
						character *root_movie = targetChar->get_root_movie();
						assert( root_movie );
						root_movie->stop_drag();
					}
					break;
				}

				case 0x29:	// string less than
				{
					env->top(1).set_bool(env->top(1).to_tu_string() < env->top(0).to_tu_string());
					break;
				}

				case 0x2A:	// throw
				{
					log_error("todo opcode: %02X\n", action_id);
					break;
				}

				case 0x2B:	// cast_object
				{
					//Determines if object is an instance of constructor 
					// (doing the same comparison as ActionInstanceOf).

					as_object* obj = env->top(0).to_object();
					as_function* constructor = env->top(1).to_function();

					if ((obj && obj->is_instance_of(constructor)) == false)
					{
						obj = NULL;
					}

					//printf("cast_object 0x%p\n", obj);

					env->drop(1);
					env->top(0).set_as_object(obj);
					break;
				}

				case 0x2C:	// implements
				{
					// Declare that a class s1 implements one or more
					// interfaces (i2 == number of interfaces, s3..sn are the names
					// of the interfaces).
					log_error("todo opcode: %02X\n", action_id);
					break;
				}

				case 0x30:	// random
				{
					int	max = env->top(0).to_int();
					if (max < 1) max = 1;
					env->top(0).set_int(tu_random::next_random() % max);
					break;
				}
				case 0x31:	// mb length
				{
					// @@ TODO
					log_error("todo opcode: %02X\n", action_id);
					break;
				}
				case 0x32:	// ord
				{
					// ASCII code of first character
					env->top(0).set_int(env->top(0).to_string()[0]);
					break;
				}
				case 0x33:	// chr
				{
					char	buf[2];
					buf[0] = env->top(0).to_int();
					buf[1] = 0;
					env->top(0).set_string(buf);
					break;
				}

				case 0x34:	// get timer
					// Push milliseconds since we started playing.
					env->push((double) (tu_timer::get_ticks()));
					break;

				case 0x35:	// mb substring
				{
					// @@ TODO
					log_error("todo opcode: %02X\n", action_id);
					break;
				}
				case 0x37:	// mb chr
				{
					// @@ TODO
					log_error("todo opcode: %02X\n", action_id);
					break;
				}
				case 0x3A:	// delete
				{
					const tu_string varname = env->top(0).to_tu_string();
					as_object* obj_interface = env->top(1).to_object();
					env->drop(1);

					bool retcode = false;
					if (obj_interface)
					{
						as_value val;
						if (obj_interface->get_member(varname, &val))
						{
							obj_interface->set_member(varname, as_value());
							retcode = true;
						}
					}

					env->top(0).set_bool(retcode);
					break;
				}
				case 0x3B:	// delete2
				{
					tu_string varname(env->top(0).to_tu_string());
					as_value obj(env->get_variable_raw(varname, with_stack));

					if (obj.is_undefined() == false)
					{
						// drop refs
						obj.set_undefined();

						// finally to remove it we need set varname to undefined

						// try delete local var
						int	local_index = env->find_local(varname, true);
						if (local_index >= 0)
						{
							// local var is found
							env->m_local_frames[local_index].m_value.set_undefined();
							env->top(0).set_bool(true);
							break;
						}
						else
						{
							// check var in m_variables
							as_value val;
							if (env->get_member(varname, &val))
							{
								env->set_member(varname, obj);
								env->top(0).set_bool(true);
								break;
							}
						}
					}

					// can't delete var
					env->top(0).set_bool(false);
					break;
				}

				case 0x3C:	// set local
				{
					env->set_local(env->top(1).to_tu_string(), env->top(0));
					env->drop(2);
					break;
				}

				case 0x3D:	// call function
				{
					as_value	function;
					if (env->top(0).is_string())
					{
						// Function is a string; lookup the function.
						const tu_string&	function_name = env->top(0).to_tu_string();
						function = env->get_variable(function_name, with_stack);

						// super constructor, Flash 6 
						if (function.is_object())
						{
							as_object* obj = function.to_object();
							if (obj)
							{
								// assert(function_name == "super");
								obj->get_ctor(&function);
							}
						}

						if (function.is_function() == false)
						{
							log_error("error in call_function: '%s' is not a function\n",
								  function_name.c_str());
						}
					}
					else
					{
						// Hopefully the actual function object is here.
						function = env->top(0);
					}
					int	nargs = env->top(1).to_int();
					as_value	result = call_method(function, env, as_value(), nargs, env->get_top_index() - 2);
					env->drop(nargs + 1);
					env->top(0) = result;
					break;
				}
				case 0x3E:	// return
				{
					// Put top of stack in the provided a slot, if
					// it's not NULL.
					if (retval)
					{
						*retval = env->top(0);
					}
					env->drop(1);

					// Skip the rest of this buffer (return from this action_buffer).
					pc = stop_pc;

					break;
				}
				case 0x3F:	// modulo
				{
					as_value	result;
					double	y = env->pop().to_number();
					double	x = env->pop().to_number();
					if (y != 0)
					{
						result.set_double(fmod(x, y));
					}
					env->push(result);
					break;
				}
				case 0x40:	// new
				{
					as_value	classname = env->pop();
					IF_VERBOSE_ACTION(log_msg("-------------- new object: %s\n",
								  classname.to_tu_string().c_str()));
					int	nargs = env->pop().to_int();
					as_value constructor = env->get_variable(classname.to_tu_string(), with_stack);
					as_value new_obj;

					if (as_c_function* c_constructor = cast_to<as_c_function>(constructor.to_object()))
					{
						// C function is responsible for creating the new object and setting members.
						(*c_constructor)(fn_call(&new_obj, as_value(), env, nargs, env->get_top_index()));
					}
					else if (as_s_function* s_constructor = cast_to<as_s_function>(constructor.to_object()))
					{

						// Create an empty object
						gc_ptr<as_object>	new_obj_ptr = new as_object(env->get_player());
						as_object* proto = new_obj_ptr->create_proto(s_constructor);

						// override m_this_ptr with just new created object
						proto->m_this_ptr = new_obj_ptr.get_ptr();

						// Call the actual constructor function; new_obj is its 'this'.
						// We don't need the function result.
						call_method(s_constructor, env, new_obj_ptr.get_ptr(), nargs, env->get_top_index());
				
						new_obj_ptr->set_ctor(s_constructor);
						new_obj.set_as_object(new_obj_ptr.get_ptr());
					}
					else
					{
						// try to load user defined class from DLL / shared library
						array<as_value> params;
						int first_arg_bottom_index = env->get_top_index();
						for (int i = 0; i < nargs; i++)
						{
							params.push_back(env->bottom(first_arg_bottom_index - i));
						}

						as_object* plugin = load_as_plugin(env->get_player(), classname.to_tu_string(), params);
						if (plugin)
						{
							new_obj.set_as_object(plugin);
						}
						else
						{
							log_error("can't create object with unknown class '%s'\n",
								  classname.to_tu_string().c_str());
						}
					}

					// places new object to heap
					if (new_obj.to_object())
					{
						env->get_player()->set_alive(new_obj.to_object());
					}

					env->drop(nargs);
					env->push(new_obj);
					break;
				}
				case 0x41:	// declare local
				{
					const tu_string&	varname = env->top(0).to_tu_string();
					env->declare_local(varname);
					env->drop(1);
					break;
				}
				case 0x42:	// init array
				{
 					// Call the array constructor
 					as_value	result;
 					as_global_array_ctor(fn_call(&result, as_value(), env, -1, -1));
 					env->push(result);
					break;
				}
				case 0x43:	// declare object
				{
				// Pops elems off of the stack. Pops [value1, name1, ? valueN, nameN] off the stack.
				// It does the following:
				// 1 Pops the number of initial properties from the stack.
				// 2 Initializes the object as a ScriptObject.
				// 3 Sets the object type to “Object?
				// 4 Pops each initial property off the stack. For each initial property, the value of the property is
				// popped off the stack, then the name of the property is popped off the stack. The name of the
				// property is converted to a string. The value may be of any type.				{

					int n = env->pop().to_int();
					as_object* obj = new as_object(env->get_player());

					// Set members
					for (int i = 0; i < n; i++)
					{
						// a new item must be ENUM
//						obj->builtin_member(env->top(1).to_tu_string(), env->top(0));
						obj->set_member(env->top(1).to_tu_string(), env->top(0));

						env->drop(2);
					}

					env->push(obj); 
					break;
				}
				case 0x44:	// type of
				{
					env->top(0).set_string(env->top(0).type_of());
					break;
				}
				case 0x45:	// get target
				{
					// @@ TODO
					log_error("todo opcode: %02X\n", action_id);
					break;
				}
				case 0x46:	// enumerate
				{
					as_value variable = env->get_variable(env->top(0).to_tu_string(), with_stack);
					enumerate(env, variable.to_object());
					env->drop(1);
					break;
				}
				case 0x47:	// add_t (typed)
				{
					if (env->top(0).is_string() || env->top(1).is_string())
					{
						tu_string str = env->top(1).to_string();
						str += env->top(0).to_string();
						env->top(1).set_tu_string(str);
					}
					else
					{
						env->top(1) += env->top(0).to_number();
					}
					env->drop(1);
					break;
				}
				case 0x48:	// less than (typed)
				{
					if (env->top(1).is_string())
					{
						env->top(1).set_bool(env->top(1).to_tu_string() < env->top(0).to_tu_string());
					}
					else
					{
						env->top(1).set_bool(env->top(1) < env->top(0).to_number());
					}
					env->drop(1);
					break;
				}
				case 0x49:	// equal (typed)
				{
					// @@ identical to untyped equal, as far as I can tell...
					env->top(1).set_bool(env->top(1) == env->top(0));
					env->drop(1);
					break;
				}
				case 0x4A:	// to number
				{
					double n = env->top(0).to_number();
					env->top(0).set_double(n);
					break;
				}
				case 0x4B:	// to string
				{
					const tu_string& str = env->top(0).to_tu_string();
					env->top(0).set_tu_string(str);
					break;
				}
				case 0x4C:	// dup
					env->push(env->top(0));
					break;
				
				case 0x4D:	// swap
				{
					as_value	temp = env->top(1);
					env->top(1) = env->top(0);
					env->top(0) = temp;
					break;
				}
				case 0x4E:	// get member
				{
					// keep alive 'obj'
					gc_ptr<as_object>	obj = env->top(1).to_object();
					if (obj == NULL)
					{
						// try property/method of a primitive type, like String.length
						as_value val;
						env->top(1).find_property(env->top(0).to_tu_string(), &val);
						if (val.is_property())
						{
							val.get_property(env->top(1), &val);
						}
						env->top(1) = val;
					}
					else
					{
						// keep the latest var name(to log it if call_method failure)
						last_varname = env->top(0).to_tu_string();

						env->top(1).set_undefined();
						if (obj->get_member(env->top(0).to_tu_string(), &(env->top(1))) == false)
						{
							// try '__resolve' property
							as_value val;
							if (obj->get_member("__resolve", &val))
							{
								// call __resolve
								as_function* resolve = cast_to<as_function>(val.to_object());
								if (resolve)
								{
									(*resolve)(fn_call(&val, obj.get(), env, 1, env->get_top_index()));
									env->top(1) = val;
								}
							}
						}
						
						IF_VERBOSE_ACTION(
							if (env->top(1).to_object() == NULL)
							{
								log_msg("-------------- get_member %s=%s\n",
											env->top(0).to_tu_string().c_str(),
											env->top(1).to_tu_string().c_str());
							}
							else
							{
								log_msg("-------------- get_member %s=%s at %p\n",
											env->top(0).to_tu_string().c_str(),
											env->top(1).to_tu_string().c_str(), env->top(1).to_object());
						});
					}
					env->drop(1);
					break;
				}
				case 0x4F:	// set member
				{
					as_object*	obj = env->top(2).to_object();
					if (obj)
					{
						obj->set_member(env->top(1).to_tu_string(), env->top(0));
						IF_VERBOSE_ACTION(
							log_msg("-------------- set_member [%p].%s=%s\n",
//								env->top(2).to_tu_string().c_str(),
								obj,
								env->top(1).to_tu_string().c_str(),
								env->top(0).to_tu_string().c_str()));
					}
					else
					{
						// Invalid object, can't set.
						IF_VERBOSE_ACTION(
							log_msg("-------------- set_member %s.%s=%s on invalid object!\n",
								env->top(2).to_tu_string().c_str(),
								env->top(1).to_tu_string().c_str(),
								env->top(0).to_tu_string().c_str()));
					}
					env->drop(3);
					break;
				}
				case 0x50:	// increment
					env->top(0) += 1;
					break;
				case 0x51:	// decrement
					env->top(0) -= 1;
					break;
				case 0x52:	// call method
				{
					int	nargs = env->top(2).to_int();
					as_value	result;
					const tu_string&	method_name = env->top(0).to_tu_string();

					as_value func;
					if (env->top(1).find_property(method_name, &func))
					{
						result = call_method(
							func,
							env,
							env->top(1),
							nargs,
							env->get_top_index() - 3);
					}
					else
					if (method_name == "undefined" || method_name == "")
					{
						// Flash help says:
						// If the method name is blank or undefined, the object is taken to be 
						// a function object that should be invoked, 
						// rather than the container object of a method. For example, if
						// CallMethod is invoked with object obj and method name blank,
						// it's equivalent to using the syntax:
						// obj();

						as_object* obj = env->top(1).to_object();
						if (obj && obj->m_this_ptr != NULL)
						{
							as_value constructor;
							if (obj->get_ctor(&constructor))
							{
								obj->create_proto(constructor);
								result = call_method(
									constructor,
									env,
									obj,
									nargs,
									env->get_top_index() - 3);
							}
						}
						else
						{
							log_error("error: call_method can't call constructor 0x%p.%s\n", env->top(1).to_object(), method_name.c_str());
						}
					}
					else
					{
//#ifdef _DEBUG
						// put as much as possible information
						if (env->top(1).to_object())
						{
							sprite_instance* clip = cast_to<sprite_instance>(env->top(1).to_object());
							if (clip)
							{
								as_value val;
								clip->get_member("_name", &val);
								log_error("error: can't find %s[0x%p, _name='%s'].%s\n", 
									last_varname.c_str(), clip, val.to_string(), method_name.c_str());
							}
							else
							{
								log_error("error: can't find %s[0x%p].%s\n", 
									last_varname.c_str(), env->top(1).to_object(), method_name.c_str());
							}
						}
						else
						{
							log_error("error: can't find %s[0x0].%s\n", 
								last_varname.c_str(), method_name.c_str());
						}
//#else
//						log_error("error: can't find method %s\n", method_name.c_str());
//#endif

					}
					env->drop(nargs + 2);
					env->top(0) = result;
					break;
				}

				case 0x53:	// new method
				{

				// Invokes a constructor function to create a new object.
				// A new object is constructed and passed to the constructor function 
				// as the value of the this keyword. Arguments may be specified to the
				// constructor function. The return value from the constructor function 
				// is discarded. The newly constructed object is pushed to the stack. 
				// Similar to ActionCallMethod and ActionNewObject.

				// 1 Pops the name of the method from the stack.
				// 2 Pops the ScriptObject from the stack. If the name of the method is blank,
				// the ScriptObject is treated as a function object which is invoked as 
				// the constructor function. If the method name is not blank, the named method
				// of the ScriptObject is invoked.
				// 3 Pops the number of arguments from the stack.
				// 4 Executes the method call.
				// 5 Pushes the newly constructed object to the stack. 
				// Note, if there is no appropriate return value (i.e: the function does not have
				// a “return?statement), a “push undefined?is generated by the
				// compiler and is pushed to the stack. 
				// The “undefined?return value should be popped off the stack.

					as_value	constructor = env->pop();
					as_object*	obj = env->pop().to_object();
					int	nargs = env->pop().to_int();

					// Create an empty object
					gc_ptr<as_object>	new_obj;

					if (cast_to<as_c_function>(constructor.to_object()))
					{
						// C function is responsible for creating the new object and setting members.
						new_obj = call_method( constructor, env, as_value(), nargs, env->get_top_index()).to_object();
					}
					else if (cast_to<as_s_function>(constructor.to_object()))
					{
						new_obj = new as_object(env->get_player());
						as_object* proto = new_obj->create_proto(constructor);
						proto->m_this_ptr = new_obj.get_ptr();

						// Call the actual constructor function; new_obj is its 'this'.
						// We don't need the function result.
						call_method(constructor, env, new_obj.get_ptr(), nargs, env->get_top_index());
					}
					else
					{
						as_value val;
						if (obj && obj->get_member(constructor.to_string(), &val))
						{
							if ( as_s_function* func = cast_to<as_s_function>(val.to_object()))
							{
								// there is already _constructor_ in prototype 
//								as_value prototype;
//								func->get_member("prototype", &prototype);
//								prototype.to_object()->set_member("__constructor__", func);

								new_obj = new as_object(env->get_player());
								as_object* proto = new_obj->create_proto(func);
								proto->m_this_ptr = new_obj.get_ptr();

								// Call the actual constructor function; new_obj is its 'this'.
								// We don't need the function result.
								call_method(func, env, new_obj.get_ptr(), nargs, env->get_top_index());
							}
							else if( as_c_function* c_func = cast_to<as_c_function>(val.to_object()) )
							{
								// TODO: I don't get this prototype stuff, need to implement it
								
								// Result contains the new output.
								new_obj = call_method(c_func, env, as_value(), nargs, env->get_top_index()).to_object();
							}
							else
							{
								assert( false && "not a function" );
							}
						}

					}


					// places new object to heap
					env->get_player()->set_alive(new_obj.get_ptr());

					env->drop(nargs);
					env->push(new_obj.get_ptr());
					break;
				}

				case 0x54:	// instance of
				{
					const as_function* constructor = env->top(0).to_function();
					bool ret = env->top(1).is_instance_of(constructor);
					env->top(1).set_bool(ret);
					env->drop(1);
					break;
				}

				case 0x55:	// enumerate object
				{
					as_value variable = env->pop();
					enumerate(env, variable.to_object());
					break;
				}
				case 0x60:	// bitwise and
					env->top(1) &= env->top(0).to_int();
					env->drop(1);
					break;
				case 0x61:	// bitwise or
					env->top(1) |= env->top(0).to_int();
					env->drop(1);
					break;
				case 0x62:	// bitwise xor
					env->top(1) ^= env->top(0).to_int();
					env->drop(1);
					break;
				case 0x63:	// shift left
					env->top(1).shl(env->top(0).to_int());
					env->drop(1);
					break;
				case 0x64:	// shift right (signed)
					env->top(1).asr(env->top(0).to_int());
					env->drop(1);
					break;
				case 0x65:	// shift right (unsigned)
					env->top(1).lsr(env->top(0).to_int());
					env->drop(1);
					break;
				case 0x66:	// strict equal
					env->top(1).set_bool(env->top(1) == env->top(0));
					env->drop(1);
					break;
				case 0x67:	// gt (typed)
					if (env->top(1).is_string())
					{
						env->top(1).set_bool(env->top(1).to_tu_string() > env->top(0).to_tu_string());
					}
					else
					{
						env->top(1).set_bool(env->top(1).to_number() > env->top(0).to_number());
					}
					env->drop(1);
					break;
				case 0x68:	// string gt
					env->top(1).set_bool(env->top(1).to_tu_string() > env->top(0).to_tu_string());
					env->drop(1);
					break;

				case 0x69:	// extends

					// These steps are the equivalent to the following ActionScript:
					// Subclass.prototype = new Object();
					// Subclass.prototype.__proto__ = Superclass.prototype;
					// Subclass.prototype.__constructor__ = Superclass;

					as_value& super = env->top(0);
					as_value& sub = env->top(1);

					assert(sub.to_object());
					assert(super.is_function());

					as_value super_prototype;
					super.to_object()->get_member("prototype", &super_prototype);

					as_object* new_prototype = new as_object(env->get_player());

					new_prototype->m_proto = super_prototype.to_object();
					new_prototype->set_ctor(super);
					sub.to_object()->set_member("prototype", new_prototype);
					env->drop(2);
					break;

				}
				pc++;	// advance to next action.
			}
			else
			{
				IF_VERBOSE_ACTION(log_msg("EX:\t"); log_disasm(&buffer[pc]));

				// Action containing extra data.
				int	length = buffer[pc + 1] | (buffer[pc + 2] << 8);
				int	next_pc = pc + length + 3;

				switch (action_id)
				{
				default:
					break;

				case 0x81:	// goto frame
				{
					int	frame = buffer[pc + 3] | (buffer[pc + 4] << 8);
					// 0-based already?
					//// Convert from 1-based to 0-based
					//frame--;

					if (env->get_target() != NULL)
					{
						env->get_target()->goto_frame(frame);
					}
					break;
				}

				case 0x83:	// get url
				{
					// If this is an FSCommand, then call the callback
					// handler, if any.

					// Two strings as args.
					const char*	url = (const char*) &(buffer[pc + 3]);
					int	url_len = int(strlen(url));
					const char*	target = (const char*) &(buffer[pc + 3 + url_len + 1]);

					// If the url starts with "FSCommand:", then this is
					// a message for the host app.
					if (strncmp(url, "FSCommand:", 10) == 0)
					{
						if (get_fscommand_callback())
						{
							// Call into the app.
							character* mroot = env->get_player()->get_root_movie();
							assert(mroot);
							(*get_fscommand_callback())(mroot, url + 10, target);
						}
					}
					else
					{
						env->load_file(url, as_value(target));
					}

					break;
				}

				case 0x87:	// store_register
				{
					int	reg = buffer[pc + 3];
					// Save top of stack in specified register.
					if (is_function2)
					{
						env->set_register(reg, env->top(0));

						IF_VERBOSE_ACTION(
							log_msg("-------------- local register[%d] = '%s'\n",
								reg,
								env->top(0).to_string()));
					}
					else if (reg >= 0 && reg < 4)
					{
						env->m_global_register[reg] = env->top(0);
					
						IF_VERBOSE_ACTION(
							log_msg("-------------- global register[%d] = '%s'\n",
								reg,
								env->top(0).to_string()));
					}
					else
					{
						log_error("store_register[%d] -- register out of bounds!", reg);
					}

					break;
				}

				case 0x88:	// decl_dict: declare dictionary
				{
					int	i = pc;
					//int	count = buffer[pc + 3] | (buffer[pc + 4] << 8);
					i += 2;

					const_cast<action_buffer*>( this )->process_decl_dict(pc, next_pc);

					break;
				}

				case 0x8A:	// wait for frame
				{
					// If we haven't loaded a specified frame yet, then we're supposed to skip
					// some specified number of actions.
					//
					// Since we don't load incrementally, just ignore this opcode.
					break;
				}

				case 0x8B:	// set target
				{
					// Change the movie we're working on.
					as_value val = (const char*) &buffer[pc + 3];
					env->set_target(val, original_target);
					break;
				}

				case 0x8C:	// go to labeled frame, goto_frame_lbl
				{
					char*	frame_label = (char*) &buffer[pc + 3];
					if (env->get_target() != NULL)
					{
						env->get_target()->goto_labeled_frame(frame_label);
					}
					break;
				}

				case 0x8D:	// wait for frame expression (?)
				{
					// Pop the frame number to wait for; if it's not loaded skip the
					// specified number of actions.
					//
					// Since we don't support incremental loading, pop our arg and
					// don't do anything.
					env->drop(1);
					break;
				}

				case 0x8E:	// function2
				{
					int	i = pc;
					i += 3;

					// Extract name.
					// @@ security: watch out for possible missing terminator here!
					tu_string	name = (const char*) &buffer[i];
					i += name.length() + 1;

					bool function_exists = false;

					if (name.length() > 0)
					{
						as_value value;
						env->get_member(name, &value);
						as_s_function* existing_function = cast_to<as_s_function>(value.to_object());

						if (existing_function)
						{
							if (existing_function->m_action_buffer.m_buffer == m_buffer
								&& existing_function->m_start_pc == next_pc )
							{ 
								function_exists = true;
							}
						}
					}

					if (function_exists == false)
					{
						as_s_function*	func = new as_s_function(env->get_player(), this, next_pc, with_stack);
						func->set_target(env->get_target());
						func->set_is_function2();

						// Get number of arguments.
						int	nargs = buffer[i] | (buffer[i + 1] << 8);
						i += 2;

						// Get the count of local registers used by this function.
						uint8	register_count = buffer[i];
						i += 1;
						func->set_local_register_count(register_count);

						// Flags, for controlling register assignment of implicit args.
						uint16	flags = buffer[i] | (buffer[i + 1] << 8);
						i += 2;
						func->set_function2_flags(flags);

						// Get the register assignments and names of the arguments.
						for (int n = 0; n < nargs; n++)
						{
							int	arg_register = buffer[i];
							i++;

							// @@ security: watch out for possible missing terminator here!
							func->add_arg(arg_register, (const char*) &buffer[i]);
							i += func->m_args.back().m_name.length() + 1;
						}

						// Get the length of the actual function code.
						int	length = buffer[i] | (buffer[i + 1] << 8);
						i += 2;
						func->set_length(length);

						// Skip the function body (don't interpret it now).
						next_pc += length;

						// If we have a name, then save the function in this
						// environment under that name.
						as_value	function_value(func);
						if (name.length() > 0)
						{
							// @@ NOTE: should this be m_target->set_variable()???
							env->set_member(name, function_value);
						}
						else
						{
							// Also leave it on the stack.
							env->push(function_value);
						}
					}
					else
					{
						int	nargs = buffer[i] | (buffer[i + 1] << 8);
						i += 2;

						// Get the count of local registers used by this function.
						i += 1;

						// Flags, for controlling register assignment of implicit args.
						i += 2;

						// Get the register assignments and names of the arguments.
						for (int n = 0; n < nargs; n++)
						{
							i++;
							// @@ security: watch out for possible missing terminator here!
							i += (int) strlen((const char*) &buffer[i]) + 1;
						}

						// Skip the length of the actual function code.
						int	length = buffer[i] | (buffer[i + 1] << 8);
						i += 2;

						// Skip the function body (don't interpret it now).
						next_pc += length;
					}
					break;
				}

				case 0x94:	// with
				{
					IF_VERBOSE_ACTION(log_msg("-------------- with block start: stack size is %d\n", with_stack.size()));
					if (with_stack.size() < 8) //todo, depends on flash version, could be 16
					{
 						int	block_length = buffer[pc + 3] | (buffer[pc + 4] << 8);
 						int	block_end = next_pc + block_length;
 						as_object*	with_obj = env->top(0).to_object();
 						with_stack.push_back(with_stack_entry(with_obj, block_end));
					}
					env->drop(1);
					break;
				}
				case 0x96:	// push_data
				{
					int i = pc;
					while (i - pc < length)
					{
						int	type = buffer[3 + i];
						i++;
						if (type == 0)
						{
							// string
							const char*	str = (const char*) &buffer[3 + i];
							i += int(strlen(str)) + 1;
							env->push(str);

							IF_VERBOSE_ACTION(log_msg("-------------- pushed '%s'\n", str));
						}
						else if (type == 1)
						{
							// float (little-endian)
							union {
								float	f;
								Uint32	i;
							} u;
							compiler_assert(sizeof(u) == sizeof(u.i));

							memcpy(&u.i, &buffer[3 + i], 4);
							u.i = swap_le32(u.i);
							i += 4;

							env->push(u.f);

							IF_VERBOSE_ACTION(log_msg("-------------- pushed '%f'\n", u.f));
						}
						else if (type == 2)
						{
							as_value nullvalue;
							nullvalue.set_null();
							env->push(nullvalue);	

							IF_VERBOSE_ACTION(log_msg("-------------- pushed NULL\n"));
						}
						else if (type == 3)
						{
							env->push(as_value());

							IF_VERBOSE_ACTION(log_msg("-------------- pushed UNDEFINED\n"));
						}
						else if (type == 4)
						{
							// contents of register
							int	reg = buffer[3 + i];
							UNUSED(reg);
							i++;
							if (is_function2)
							{
								env->push(*env->get_register(reg));
								IF_VERBOSE_ACTION(
									log_msg("-------------- pushed local register[%d] = '%s'\n",
										reg,
										env->top(0).to_string()));
							}
							else if (reg < 0 || reg >= 4)
							{
								env->push(as_value());
								log_error("push register[%d] -- register out of bounds!\n", reg);
							}
							else
							{
								env->push(env->m_global_register[reg]);
								IF_VERBOSE_ACTION(
									log_msg("-------------- pushed global register[%d], '%s', 0x%p\n",
										reg,
										env->top(0).to_string(),
										env->top(0).to_object()));
							}

						}
						else if (type == 5)
						{
							bool	bool_val = buffer[3 + i] ? true : false;
							i++;
//							log_msg("bool(%d)\n", bool_val);
							env->push(bool_val);

							IF_VERBOSE_ACTION(log_msg("-------------- pushed %s\n", bool_val ? "true" : "false"));
						}
						else if (type == 6)
						{
							// double
							// wacky format: 45670123
							union {
								double	d;
								Uint64	i;
								struct {
									Uint32	lo;
									Uint32	hi;
								} sub;
							} u;
							compiler_assert(sizeof(u) == sizeof(u.i));

							memcpy(&u.sub.hi, &buffer[3 + i], 4);
							memcpy(&u.sub.lo, &buffer[3 + i + 4], 4);
							u.i = swap_le64(u.i);
							i += 8;

//							if (i == 9)	// spec case, NaN constant
//							{
//								env->push(get_nan());
//							}
//							else
//							{
							env->push(u.d);
//							}

							IF_VERBOSE_ACTION(log_msg("-------------- pushed double %f\n", u.d));
						}
						else if (type == 7)
						{
							// int32
							Sint32	val = buffer[3 + i]
								| (buffer[3 + i + 1] << 8)
								| (buffer[3 + i + 2] << 16)
								| (buffer[3 + i + 3] << 24);
							i += 4;
						
							env->push(val);


							IF_VERBOSE_ACTION(log_msg("-------------- pushed int32 %d\n", val));
						}
						else if (type == 8)
						{
							int	id = buffer[3 + i];
							i++;
							if (id < m_dictionary.size())
							{
								env->push(m_dictionary[id]);

								IF_VERBOSE_ACTION(log_msg("-------------- pushed '%s'\n", m_dictionary[id].c_str()));
							}
							else
							{
								log_error("error: dict_lookup(%d) is out of bounds!\n", id);
								env->push(0);
								IF_VERBOSE_ACTION(log_msg("-------------- pushed 0\n"));
							}
						}
						else if (type == 9)
						{
							int	id = buffer[3 + i] | (buffer[4 + i] << 8);
							i += 2;
							if (id < m_dictionary.size())
							{
								env->push(m_dictionary[id]);
								IF_VERBOSE_ACTION(log_msg("-------------- pushed '%s'\n", m_dictionary[id].c_str()));
							}
							else
							{
								log_error("error: dict_lookup(%d) is out of bounds!\n", id);
								env->push(0);

								IF_VERBOSE_ACTION(log_msg("-------------- pushed 0"));
							}
						}
					}
					
					break;
				}
				case 0x99:	// branch always (goto)
				{
					Sint16	offset = buffer[pc + 3] | (buffer[pc + 4] << 8);
					next_pc += offset;
					// @@ TODO range checks
					break;
				}
				
				case 0x9A:	// get url2
				{
					int	method = buffer[pc + 3];
					const char*	url = env->top(1).to_string();

					// If the url starts with "FSCommand:", then this is
					// a message for the host app.
					if (strncmp(url, "FSCommand:", 10) == 0)
					{
						if (get_fscommand_callback())
						{
							// Call into the app.
							character* mroot = env->get_player()->get_root_movie();
							assert(mroot);
							(*get_fscommand_callback())(mroot, url + 10, env->top(0).to_string());
						}
					}
					else
					{
						env->load_file(url, env->top(0), method);
					}
					env->drop(2);
					break;
				}

				case 0x9B:	// declare function
				{
					int	i = pc;
					i += 3;

					// Extract name.
					// @@ security: watch out for possible missing terminator here!
					tu_string	name = (const char*) &buffer[i];
					i += name.length() + 1;

					bool function_exists = false;

					if (name.length() > 0)
					{
						as_value value;
						env->get_member(name, &value);
						as_s_function* existing_function = cast_to<as_s_function>(value.to_object());

						if (existing_function)
						{
							if (existing_function->m_action_buffer.m_buffer == m_buffer
								&& existing_function->m_start_pc == next_pc )
							{ 
								function_exists = true;
							}
						}
					}

					if (function_exists == false)
					{
						as_s_function*	func = new as_s_function(env->get_player(), this, next_pc, with_stack);
						func->set_target(env->get_target());

						// Get number of arguments.
						int	nargs = buffer[i] | (buffer[i + 1] << 8);
						i += 2;

						// Get the names of the arguments.
						for (int n = 0; n < nargs; n++)
						{
							// @@ security: watch out for possible missing terminator here!
							func->add_arg(0, (const char*) &buffer[i]);
							i += func->m_args.back().m_name.length() + 1;
						}

						// Get the length of the actual function code.
						int	length = buffer[i] | (buffer[i + 1] << 8);
						i += 2;
						func->set_length(length);

						// Skip the function body (don't interpret it now).
						next_pc += length;

						// ActionDefineFunction can be used in the following ways:
						// Usage #1. Pushes an anonymous function on the stack that does not persist.
						// Usage #2. Sets a variable with a given FunctionName and a given function definition.
						// Thanks to Julien Hamaide
						as_value	function_value(func);
						if (name.length() > 0)
						{
							// @@ NOTE: should this be m_target->set_variable()???
							// Usage #1. If we have a name, then save the function in this
							// environment under that name.
							env->set_member(name, function_value);
						}
						else
						{
							// Usage #2. Leave it on the stack
							env->push(function_value);
						}
					}
					else
					{
						// Get number of arguments.
						int	nargs = buffer[i] | (buffer[i + 1] << 8);
						i += 2;

						// Get the names of the arguments.
						for (int n = 0; n < nargs; n++)
						{
							// @@ security: watch out for possible missing terminator here!
							i += (int) strlen((const char*) &buffer[i]) + 1;
						}

						// Get the length of the actual function code.
						int	length = buffer[i] | (buffer[i + 1] << 8);

						// Skip the function body (don't interpret it now).
						next_pc += length;
					}

					break;
				}

				case 0x9D:	// branch if true
				{
					Sint16	offset = buffer[pc + 3] | (buffer[pc + 4] << 8);
					
					bool	test = env->top(0).to_bool();
					env->drop(1);
					if (test)
					{
						IF_VERBOSE_ACTION(log_msg("-------------- branch is done\n"));
						next_pc += offset;

						if (next_pc > stop_pc)
						{
							log_error("branch to offset %d -- this section only runs to %d\n",
								  next_pc,
								  stop_pc);
						}
					}
					else
					{
						IF_VERBOSE_ACTION(log_msg("-------------- no branch\n"));
					}
					break;
				}
				case 0x9E:	// call frame
				{
					// Note: no extra data in this instruction!
					assert(env->get_target() != NULL);
					env->get_target()->call_frame_actions(env->top(0));
					env->drop(1);

					break;
				}

				case 0x9F:	// goto frame expression, goto_frame_exp
				{
					// From Alexi's SWF ref:
					//
					// Pop a value or a string and jump to the specified
					// frame. When a string is specified, it can include a
					// path to a sprite as in:
					// 
					//   /Test:55
					// 
					// When f_play is ON, the action is to play as soon as
					// that frame is reached. Otherwise, the
					// frame is shown in stop mode.

					unsigned char	play_flag = buffer[pc + 3];
					character::play_state	state = play_flag ? character::PLAY : character::STOP;

					character* target = env->get_target();
					bool success = false;

					if (env->top(0).is_undefined())
					{
						// No-op.
					}
					else if (env->top(0).is_string())
					{
						// @@ TODO: parse possible sprite path...
						
						// Also, if the frame spec is actually a number (not a label), then
						// we need to do the conversion...

						const char* frame_label = env->top(0).to_string();
						if (target->goto_labeled_frame(frame_label))
						{
							success = true;
						}
						else
						{
							// Couldn't find the label.  Try converting to a number.
							double num;
							if (string_to_number(&num, env->top(0).to_string()))
							{
								int frame_number = int(num);
								target->goto_frame(frame_number);
								success = true;
							}
							// else no-op.
						}
					}
					else if (env->top(0).is_number())
					{
						// Frame numbers appear to be 0-based!  @@ Verify.
						int frame_number = env->top(0).to_int();
						target->goto_frame(frame_number);
						success = true;
					}

					if (success)
					{
						target->set_play_state(state);
					}
					
					env->drop(1);

					break;
				}
				
				}
				pc = next_pc;
			}
			
#if ACTION_BUFFER_PROFILLING
			if( profiling_table.find( action_id ) != profiling_table.end() )
			{
				profiling_table.set( action_id, 0 );
			}
			profiling_table[ action_id ] += tu_timer::get_profile_ticks() - start_time;
#endif
		}

		env->set_target(original_target);
	}

#if ACTION_BUFFER_PROFILLING
	void action_buffer::log_and_reset_profiling( void )
	{
		log_msg( "--------------------\n" );
		for( gameswf::hash<int,Uint64>::iterator it = profiling_table.begin(); it != profiling_table.end(); ++it )
		{
			log_msg( "%d => %.2f millisecond \n", it.get_key(), (float)it.get_value()/1000.0f );
		}
		log_msg( "--------------------\n" );
		profiling_table.clear();
	}

	gameswf::hash<int, Uint64> action_buffer::profiling_table;
#endif
	
	//
	// event_id
	//


	static tu_string	s_function_names[event_id::EVENT_COUNT] =
	{
		"INVALID",		 // INVALID
		"onPress",		 // PRESS
		"onRelease",		 // RELEASE
		"onRelease_Outside",	 // RELEASE_OUTSIDE
		"onRollOver",		 // ROLL_OVER
		"onRollOut",		 // ROLL_OUT
		"onDragOver",		 // DRAG_OVER
		"onDragOut",		 // DRAG_OUT
		"onKeyPress",		 // KEY_PRESS
		"onInitialize",		 // INITIALIZE

		"onLoad",		 // LOAD
		"onUnload",		 // UNLOAD
		"onEnterFrame",		 // ENTER_FRAME
		"onMouseDown",		 // MOUSE_DOWN
		"onMouseUp",		 // MOUSE_UP
		"onMouseMove",		 // MOUSE_MOVE
		"onKeyDown",		 // KEY_DOWN
		"onKeyUp",		 // KEY_UP
		"onData",		 // DATA

		"onConstruct",
		"onSetFocus",
		"onKillFocus",

		// MovieClipLoader events
		"onLoadComplete",
		"onLoadError",
		"onLoadInit",
		"onLoadProgress",
		"onLoadStart",

		// sound
		"onSoundComplete"

	};

	const tu_string&	event_id::get_function_name() const
	{
		assert(m_id > INVALID && m_id < EVENT_COUNT);
		return s_function_names[m_id];
	}

};


// Local Variables:
// mode: C++
// c-basic-offset: 8
// tab-width: 8
// indent-tabs-mode: t
// End:
