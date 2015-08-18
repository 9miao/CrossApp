// tu_random.h	-- Thatcher Ulrich 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Pseudorandom number generator.


#ifndef TU_RANDOM_H
#define TU_RANDOM_H


#include "tu_config.h"
#include "tu_types.h"


namespace tu_random
{
	// Global generator.
	exported_module Uint32	next_random();
	exported_module void	seed_random(Uint32 seed);
	exported_module float	get_unit_float();

	const int	SEED_COUNT = 8;
	
	// In case you need independent generators.  The global
	// generator is just an instance of this.
	struct generator
	{
		generator();
		void	seed_random(Uint32 seed);	// not necessary
		Uint32	next_random();
		float	get_unit_float();

	private:
		Uint32	m_Q[SEED_COUNT];
		Uint32	m_c;
		Uint32	m_i;
	};

}	// end namespace tu_random


#endif // TU_RANDOM_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
