// gameswf_jit.h	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Jit helpers

#ifndef GAMESWF_JIT_H
#define GAMESWF_JIT_H

#ifdef __GAMESWF_ENABLE_JIT__

#include "../base/container.h"

class jit_function
{
	struct patch_entry
	{
		uint8 * m_address;
		int m_byte_code_position;
		int m_byte_count;
	};

	int m_current_stack_offset;
	array<uint8> m_work_byte_code;
	array<patch_entry> m_address_patches;
	void * m_executable_byte_code; // must allocate with execute rights

public:

	jit_function() :
		m_executable_byte_code( NULL ),
		m_current_stack_offset( 4 )
	{
	}

	template<typename T1, typename T2, typename T3, typename T4>
	void call( T1 t1, T2 t2, T3 t3, T4 t4 )
	{
		typedef void (*function)(T1, T2, T3, T4);

		// run compiled code
		((function) m_executable_byte_code)(t1, t2, t3, t4);
	}

	bool is_valid() const { return m_executable_byte_code != NULL; }

	void push_bytes( const uint8 * bytes, const int byte_count );
	void push_byte( const uint8 byte );
	void push_integer( const uint32 value );
	void add_address_patch( void * address, int byte_count );
	void initialize();
	int add_stack_offset( int size );

};

#include "gameswf_jit_opcode.h"

#else	// __GAMESWF_ENABLE_JIT__ is false

class jit_function
{

public:

	template<typename T1, typename T2, typename T3, typename T4>
	void call( T1 t1, T2 t2, T3 t3, T4 t4 )
	{
		assert( false );
	}

	bool is_valid() const { return false; }
	void initialize(){};
};


#endif	// __GAMESWF_ENABLE_JIT__

#endif	// GAMESWF_JIT_H
