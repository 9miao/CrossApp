// gameswf_jit.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Platform-agnostic jit opcodes

#ifdef __GAMESWF_ENABLE_JIT__

#include "gameswf_jit_opcode.h"

#include "platforms/gameswf_jit_x86.hpp"

//#define jit_prologue( _function_ ) \
//{\
//	jit_push( jit_ebp );\
//	jit_mov( jit_ebp, jit_esp );\
//}

#define jit_epilogue( _function_ )\
{\
	jit_pop( jit_ebp );\
}

#endif	// __GAMESWF_ENABLE_JIT__
