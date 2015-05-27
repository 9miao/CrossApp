// gameswf_jit.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Jit helpers

#ifdef __GAMESWF_ENABLE_JIT__ 

#include "gameswf_jit.h"

void jit_function::initialize()
{
	m_executable_byte_code = &m_work_byte_code[ 0 ];

	int patch_index, patch_count;
	patch_count = m_address_patches.size();

	for( patch_index = 0; patch_index < patch_count; ++patch_index )
	{
		patch_entry & entry = m_address_patches[ patch_index ];
		uint32 offset = (Uint32)
			(entry.m_address - &m_work_byte_code[entry.m_byte_code_position] - entry.m_byte_count);

		assert( entry.m_byte_count == 4 );
		*(uint32*)&m_work_byte_code[ entry.m_byte_code_position ] = offset;
	}
}

void jit_function::push_byte( const uint8 byte )
{
	m_work_byte_code.push_back( byte );
}

void jit_function::push_integer( const uint32 value )
{
	uint8 * byte_value = (uint8*)&value;

	m_work_byte_code.push_back( byte_value[ 0 ] );
	m_work_byte_code.push_back( byte_value[ 1 ] );
	m_work_byte_code.push_back( byte_value[ 2 ] );
	m_work_byte_code.push_back( byte_value[ 3 ] );
}

void jit_function::add_address_patch( void * address, int byte_count )
{
	int current_byte_count = m_work_byte_code.size();

	patch_entry entry;

	entry.m_address = (uint8*)address;
	entry.m_byte_code_position = current_byte_count;
	entry.m_byte_count = 4;

	m_work_byte_code.resize( m_work_byte_code.size() + byte_count );
	m_address_patches.push_back( entry );
}

int jit_function::add_stack_offset( int size )
{
	m_current_stack_offset += size;

	return m_current_stack_offset;
}

#endif	// __GAMESWF_ENABLE_JIT__
