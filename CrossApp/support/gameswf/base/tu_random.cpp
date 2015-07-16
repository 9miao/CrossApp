// tu_random.cpp	-- Thatcher Ulrich 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Pseudorandom number generator.


#include "tu_random.h"


namespace tu_random
{
	// Global generator.
	static generator	s_generator;

	Uint32	next_random()
	{
		return s_generator.next_random();
	}

	void	seed_random(Uint32 seed)
	{
		s_generator.seed_random(seed);
	}

	float	get_unit_float()
	{
		return s_generator.get_unit_float();
	}


	// PRNG code adapted from the complimentary-multiply-with-carry
	// code in the article: George Marsaglia, "Seeds for Random Number
	// Generators", Communications of the ACM, May 2003, Vol 46 No 5,
	// pp90-93.
	//
	// The article says:
	//
	// "Any one of the choices for seed table size and multiplier will
	// provide a RNG that has passed extensive tests of randomness,
	// particularly those in [3], yet is simple and fast --
	// approximately 30 million random 32-bit integers per second on a
	// 850MHz PC.  The period is a*b^n, where a is the multiplier, n
	// the size of the seed table and b=2^32-1.  (a is chosen so that
	// b is a primitive root of the prime a*b^n + 1.)"
	//
	// [3] Marsaglia, G., Zaman, A., and Tsang, W.  Toward a universal
	// random number generator.  _Statistics and Probability Letters
	// 8_ (1990), 35-39.

//	const Uint64	a = 18782;	// for SEED_COUNT=4096, period approx 2^131104 (from Marsaglia usenet post 2003-05-13)
//	const Uint64	a = 123471786;	// for SEED_COUNT=1024, period approx 2^32794
//	const Uint64	a = 123554632;	// for SEED_COUNT=512, period approx 2^16410
//	const Uint64	a = 8001634;	// for SEED_COUNT=256, period approx 2^8182
//	const Uint64	a = 8007626;	// for SEED_COUNT=128, period approx 2^4118
//	const Uint64	a = 647535442;	// for SEED_COUNT=64, period approx 2^2077
//	const Uint64	a = 547416522;	// for SEED_COUNT=32, period approx 2^1053
//	const Uint64	a = 487198574;	// for SEED_COUNT=16, period approx  2^540
	const Uint64	a = 716514398;	// for SEED_COUNT=8, period approx 2^285


	generator::generator()
	{
		seed_random(987654321);
	}


	void	generator::seed_random(Uint32 seed)
	{
		if (seed == 0) {
			// 0 is a terrible seed (probably the only bad
			// choice), substitute something else:
			seed = 12345;
		}

		// Simple pseudo-random to reseed the seeds.
		// Suggested by the above article.
		Uint32	j = seed;
		for (int i = 0; i < SEED_COUNT; i++)
		{
			j = j ^ (j << 13);
			j = j ^ (j >> 17);
			j = j ^ (j << 5);
			m_Q[i] = j;
		}

		m_c = 362436;
		m_i = SEED_COUNT - 1;
	}


	Uint32	generator::next_random()
	// Return the next pseudo-random number in the sequence.
	{
		Uint64	t;
		Uint32	x;

		//static Uint32	c = 362436;
		//static Uint32	i = SEED_COUNT - 1;
		const Uint32	r = 0xFFFFFFFE;

		m_i = (m_i + 1) & (SEED_COUNT - 1);
		t = a * m_Q[m_i] + m_c;
		m_c = (Uint32) (t >> 32);
		x = (Uint32) (t + m_c);
		if (x < m_c)
		{
			x++;
			m_c++;
		}
		
		Uint32	val = r - x;
		m_Q[m_i] = val;
		return val;
	}

	
	float	generator::get_unit_float()
	{
		Uint32	r = next_random();

		// 24 bits of precision.
		return float(r >> 8) / (16777216.0f - 1.0f);
	}

}	// end namespace tu_random


#ifdef TEST_TU_RANDOM

#include "tu_timer.h"
#include <math.h>
#include <assert.h>


int count_ones(uint32 i)
// Return the number of set bits.
{
	// I know there are cleverer ways to do this.
	int count = 0;
	for (uint32 j = 1; j != 0; j <<= 1) {
		if (j & i) count++;
	}
	return count;
}

unsigned char s_on_bits[256];

void init_count()
{
	for (int i = 0; i < 256; i++) {
		s_on_bits[i] = count_ones(i);
	}
}


int count_ones_fast(uint32 i)
{
	return s_on_bits[(i & 0x0FF)]
		+ s_on_bits[((i >> 8) & 0x0FF)]
		+ s_on_bits[((i >> 16) & 0x0FF)]
		+ s_on_bits[((i >> 24) & 0x0FF)];
}


// Compile with e.g.:
//
//  gcc -o tu_random_test tu_random.cpp -I.. -g -DTEST_TU_RANDOM -DMEASURE_SPEED -lstdc++
//  cl -o tu_random_test.exe tu_random.cpp tu_timer.cpp -I.. -Od -DTEST_TU_RANDOM -DMEASURE_SPEED /link winmm.lib
//
// Generate a test file of random numbers for DIEHARD.
int	main()
{
#ifdef MEASURE_SPEED
	const int SQRT_COUNT = 20000;

	uint64 start = tu_timer::get_ticks();
	unsigned int dummy = 0;
	for (int j = 0; j < SQRT_COUNT; j++) {
		for (int i = 0; i < SQRT_COUNT; i++) {
			dummy += tu_random::next_random();
		}
	}
	uint64 ticks = tu_timer::get_ticks() - start;

	double period = double(tu_random::a) * pow(pow(2.0, 32.0), double(tu_random::SEED_COUNT));
	double log2_period = log(period) / log(2.0);

	double secs = tu_timer::ticks_to_seconds(ticks);
	double counts_per_sec = floor(double(SQRT_COUNT) * double(SQRT_COUNT) / secs);
	printf("dummy = %u, period = %g ~= 2 ^ %g, generated %g rands / sec\n", dummy, period, log2_period, counts_per_sec);
#endif // MEASURE_SPEED
	
#ifdef GENERATE_DIEHARD_DATA
	const int	COUNT = 15000000 / 4;	// number of 4-byte words; DIEHARD needs ~80M bits

	// Generate random bitstream for DIEHARD.
	for (int i = 0; i < COUNT; i++)
	{
		Uint32	val = tu_random::next_random();
		fwrite(&val, sizeof(val), 1, stdout);
	}
#endif // GENERATE_DIEHARD_DATA

#ifdef TEST_SMALL_SEEDS
	// Test small seeds.
	for (int i = 0; i < 10; i++) {
		tu_random::seed_random(i);
		srand(i);
		for (int j = 0; j < 100; j++) {
			printf("seed = %d, tur = 0x%X, rand = 0x%X\n", i, tu_random::next_random(), rand());//xxxxxxx
		}
		printf("\n");
	}
#endif // TEST_SMALL_SEEDS

#ifdef FIND_BAD_SEEDS
	init_count();
	// Search for bad seeds:
	uint32 worst_seed = 0;
	uint32 worst_seed_bits = 16;
	uint32 worst_seed_j = 0;
	// We know 0 is bad.  Try every other 32-bit value.
	for (uint32 i = 1; i != 0; i++) {
		uint32 j = i;
		j = j ^ (j << 13);
		j = j ^ (j >> 17);
		j = j ^ (j << 5);

		int one_bits = count_ones_fast(j);// _fast
		if (one_bits <= worst_seed_bits) {
			worst_seed_bits = one_bits;
			worst_seed = i;
			worst_seed_j = j;

			printf("i = 0x%X, worst seed = 0x%X, j = 0x%X, bits = 0x%X\n", i, worst_seed, worst_seed_j, worst_seed_bits);
		}
	}
#endif // FIND_BAD_SEEDS

#ifdef TEST_DETERMINISM
	// Two generators seeded with the same value should produce
	// the same sequence.
	tu_random::generator a;
	tu_random::generator b;
	for (int i = 0; i < 100; i++) {
		assert(a.next_random() == b.next_random());
	}
	b.next_random();
	b.next_random();
	b.next_random();
	b.next_random();
	b.seed_random(456);
	b.next_random();
	b.next_random();
	b.next_random();
	b.next_random();
	// a and b should be out of sync now.
	assert(a.next_random() != b.next_random());

	a.seed_random(123);
	b.seed_random(123);
	// a and b should be back in sync.
	for (int i = 0; i < 100; i++) {
		assert(a.next_random() == b.next_random());
	}
	for (int i = 0; i < 10; i++) {
		printf("%08x %08x\n", a.next_random(), b.next_random());
	}
#endif // TEST_DETERMINISM

	return 0;
}


#endif // TEST_TU_RANDOM


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
