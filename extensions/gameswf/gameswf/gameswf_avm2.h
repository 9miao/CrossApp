// gameswf_avm2.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// AVM2 implementation


#ifndef GAMESWF_AVM2_H
#define GAMESWF_AVM2_H

#include "../base/weak_ptr.h"
#include "gameswf_function.h"
#include "gameswf_jit.h"

namespace gameswf
{
	struct abc_def;

	struct option_detail
	{
		int m_value;
		Uint8 m_kind;
	};

	struct traits_info : public ref_counted
	{
		enum kind
		{
			Trait_Slot = 0,
			Trait_Method = 1,
			Trait_Getter = 2,
			Trait_Setter = 3,
			Trait_Class = 4,
			Trait_Function = 5,
			Trait_Const = 6
		};

		enum attr
		{
			ATTR_Final = 0x1,
			ATTR_Override = 0x2,
			ATTR_Metadata = 0x4
		};

		int m_name;
		Uint8 m_kind;
		Uint8 m_attr;

		// data
		union
		{
			struct
			{
				int m_slot_id;
				int m_type_name;
				int m_vindex;
				Uint8 m_vkind;
			} trait_slot;

			struct
			{
				int m_slot_id;
				int m_classi;
			} trait_class;

			struct
			{
				int m_slot_id;
				int m_function;
			} trait_function;
			
			struct
			{
				int m_disp_id;
				int m_method;
			} trait_method;

		};


		array<int> m_metadata;

		void	read(stream* in, abc_def* abc);
	};

	struct except_info : public ref_counted
	{
		int m_from;
		int m_to;
		int m_target;
		int m_exc_type;
		int m_var_name;

		void	read(stream* in, abc_def* abc);
	};

	struct as_3_function : public as_function
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_3_FUNCTION };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_function::is(class_id);
		}


		enum flags 
		{
			NEED_ARGUMENTS = 0x01,
			NEED_ACTIVATION = 0x02,
			NEED_REST = 0x04,
			HAS_OPTIONAL = 0x08,
			SET_DXNS = 0x40,
			HAS_PARAM_NAMES = 0x80
		};

		gameswf::weak_ptr<as_object>	m_target;
		gameswf::weak_ptr<abc_def> m_abc;

		// method_info
		int m_return_type;
		array<int> m_param_type;
		int m_name;
		Uint8 m_flags;
		array<option_detail> m_options;
		int m_method;	// index in method_info

		// body_info
		int m_max_stack;

		// this is the index of the highest-numbered local register plus one.	
		int m_local_count;

		int m_init_scope_depth;
		int m_max_scope_depth;
		membuf m_code;
		array<gc_ptr<except_info> > m_exception;
		array<gc_ptr<traits_info> > m_trait;
		jit_function m_compiled_code;

		as_3_function(abc_def* abc, int method, player* player);
		~as_3_function();

		// Dispatch.
		virtual void	operator()(const fn_call& fn);

		void	execute(array<as_value>& lregister, as_environment* env, as_value* result);
		void	compile();

		void	compile_stack_resize(int count);
		void	read(stream* in);
		void	read_body(stream* in);

		tu_string get_multiname(int index, vm_stack & stack) const;

	};

}

#endif
