// gameswf_action.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Implementation and helpers for SWF actions.


#ifndef GAMESWF_ACTION_H
#define GAMESWF_ACTION_H

#include "gameswf_value.h"
#include "gameswf_environment.h"
#include "gameswf_object.h"
#include "gameswf_types.h"
#include "../base/container.h"
#include "../base/membuf.h"
#include <wchar.h>

#ifndef ACTION_BUFFER_PROFILLING
#	define ACTION_BUFFER_PROFILLING 0
#endif

namespace gameswf
{
	struct as_environment;
	struct as_c_function;
	struct as_s_function;

	exported_module const char*	call_method_parsed(
		as_environment* env,
		as_object* this_ptr,
		const char* method_name,
		const char* method_arg_fmt,
		va_list args);
		
	exported_module tu_string	call_method(
		as_environment* env,
		as_object* this_ptr,
		const char* method_name,
		const as_value* arguments,
		int argument_count
		);

	const char* get_gameswf_version();

	// Dispatching methods from C++.
	exported_module as_value	call_method(
		const as_value& method,	as_environment* env, const as_value& this_ptr,
		int nargs, int first_arg_bottom_index);

	exported_module as_value	call_method(
		as_function* func,	as_environment* env, const as_value& this_ptr,
		int nargs, int first_arg_bottom_index);

	//
	// event_id
	//
	// For keyDown and stuff like that.

	struct event_id
	{
		// These must match the function names in event_id::get_function_name()
		enum id_code
		{
			INVALID,

			// These are for buttons & sprites.
			PRESS,
			RELEASE,
			RELEASE_OUTSIDE,
			ROLL_OVER,
			ROLL_OUT,
			DRAG_OVER,
			DRAG_OUT,
			KEY_PRESS,

			// These are for sprites only.
			INITIALIZE,
			LOAD,
			UNLOAD,
			ENTER_FRAME,
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_MOVE,
			KEY_DOWN,
			KEY_UP,
			DATA,

			CONSTRUCT,
			SETFOCUS,
			KILLFOCUS,			

			// MovieClipLoader events
			ONLOAD_COMPLETE,
			ONLOAD_ERROR,
			ONLOAD_INIT,
			ONLOAD_PROGRESS,
			ONLOAD_START,

			// sound
			ON_SOUND_COMPLETE,

			EVENT_COUNT
		};

		// 'm_unused' was added because of problem with hash<event_id, ...>
		// the reason: sizeof(unsigned char+unsigned char+character*)  == 8 
		// and sizeof(unsigned char+unsigned char+Uint16+character*)  == 8
		// I think that compiler do aligment
		unsigned char	m_id;
		unsigned char	m_key_code;
		Uint16	m_unused;
		array<as_value>* m_args;

		event_id() :
			m_id(INVALID),
			m_key_code(key::INVALID),
			m_unused(0),
			m_args(NULL)
		{
		}

		event_id(id_code id, array<as_value>* args) :
			m_id((unsigned char) id),
			m_key_code(key::INVALID),
			m_unused(0),
			m_args(args)
		{
		}

		event_id(id_code id, key::code c = key::INVALID) :
			m_id((unsigned char) id),
			m_key_code((unsigned char) c),
			m_unused(0),
			m_args(NULL)
		{
			// For the button key events, you must supply a keycode.
			// Otherwise, don't.
			assert((m_key_code == key::INVALID && (m_id != KEY_PRESS))
				|| (m_key_code != key::INVALID && (m_id == KEY_PRESS)));
		}

		bool	operator==(const event_id& id) const
		{
			return m_id == id.m_id &&
				m_key_code == id.m_key_code &&
				m_args == id.m_args;
		}

		// Return the name of a method-handler function corresponding to this event.
		const tu_string&	get_function_name() const;
	};

	// allows sharing of as byte code buffer
	class counted_buffer : public membuf, public gc_object
	{
	};


	// Base class for actions.
	struct action_buffer
	{
		action_buffer();
		void	read(stream* in);
		void	execute(as_environment* env);
		void	execute(
			as_environment* env,
			int start_pc,
			int exec_bytes,
			as_value* retval,
			const array<with_stack_entry>& initial_with_stack,
			bool is_function2) const;

		static as_object* load_as_plugin(player* player,
			const tu_string& classname, const array<as_value>& params);
		int	get_length() const { return m_buffer->size(); }
		void operator=(const action_buffer& ab);
		
#if ACTION_BUFFER_PROFILLING
		static void log_and_reset_profiling( void );
#endif

	private:
		// Don't put these as values in array<>!  They contain
		// internal pointers and cannot be moved or copied.
		// If you need to keep an array of them, keep pointers
		// to new'd instances.
		//action_buffer(const action_buffer& a) { assert(0); }

		void	process_decl_dict(int start_pc, int stop_pc);
		static void	enumerate(as_environment* env, as_object* object);

		// data:
		gc_ptr<counted_buffer>	m_buffer;
		array<tu_string>	m_dictionary;
		int	m_decl_dict_processed_at;
#if ACTION_BUFFER_PROFILLING		
		static gameswf::hash<int, Uint64> profiling_table;
#endif
	};


	struct fn_call;

	struct as_property_interface
	{
		virtual ~as_property_interface() {}
		virtual bool	set_property(int index, const as_value& val) = 0;
	};

	//
	// Some handy helpers
	//

	// tulrich: don't use this!  To register a class constructor,
	// just assign the classname to the constructor function.  E.g.:
	//
	// my_movie->set_member("MyClass", as_value(MyClassConstructorFunction));
	// 
	//void register_as_object(const char* object_name, as_c_function_ptr handler);

	// Numerical indices for standard member names.	 Can use this
	// to help speed up get/set member calls, by using a switch()
	// instead of nasty string compares.
	enum as_standard_member
	{
		M_INVALID_MEMBER = -1,
		M_X,
		M_Y,
		M_XSCALE,
		M_YSCALE,
		M_CURRENTFRAME,
		M_TOTALFRAMES,
		M_ALPHA,
		M_VISIBLE,
		M_WIDTH,
		M_HEIGHT,
		M_ROTATION,
		M_TARGET,
		M_FRAMESLOADED,
		M_NAME,
		M_DROPTARGET,
		M_URL,
		M_HIGHQUALITY,
		M_FOCUSRECT,
		M_SOUNDBUFTIME,
		M_XMOUSE,
		M_YMOUSE,
		M_PARENT,
		M_TEXT,
		M_TEXTWIDTH,
		M_TEXTCOLOR,
		M_BORDER,
		M_MULTILINE,
		M_WORDWRAP,
		M_TYPE,
		M_BACKGROUNDCOLOR,
		M_THIS,
		MTHIS,
		M_ROOT,
		MDOT,
		MDOT2,
		M_LEVEL0,
		M_GLOBAL,
		M_ENABLED,
		M_PASSWORD,
		M_MOUSE_MOVE,

		AS_STANDARD_MEMBER_COUNT
	};
	// Return the standard enum, if the arg names a standard member.
	// Returns M_INVALID_MEMBER if there's no match.
	as_standard_member	get_standard_member(const tu_stringi& name);

	enum builtin_object
	{
		BUILTIN_OBJECT_METHOD,
		BUILTIN_SPRITE_METHOD,
		BUILTIN_NUMBER_METHOD,
		BUILTIN_BOOLEAN_METHOD,
		BUILTIN_STRING_METHOD,
		// and so far

		BUILTIN_COUNT
	};

	bool get_builtin(builtin_object id, const tu_stringi& name, as_value* val);
	stringi_hash<as_value>* new_standard_method_map(builtin_object id);

}	// end namespace gameswf


#endif // GAMESWF_ACTION_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
