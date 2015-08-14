// gameswf_jit.h	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Platform-agnostic jit opcodes

#ifndef GAMESWF_JIT_OPCODE_H
#define GAMESWF_JIT_OPCODE_H

	#include "platforms/gameswf_jit_x86.h"

	#define jit_prologue( _function_ ) \
	{\
		jit_push( _function_, jit_ebp );\
		jit_mov( _function_, jit_ebp, jit_esp );\
	}

	#define jit_return( _function_ )\
	{\
		jit_mov( _function_, jit_esp, jit_ebp );\
		jit_pop( _function_, jit_ebp );\
		jit_ret( _function_ ); \
	}

	#define jit_allocate_stack_object_memory( _function_, _size_ ) \
		(jit_subi( _function_, jit_stack_pointer, _size_ ), _function_.add_stack_offset( _size_ ))


#endif