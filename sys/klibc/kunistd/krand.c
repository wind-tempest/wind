// krand.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kstdio/kstdbool.h>
#include <wt/kstdio/kstdint.h>

#define PCG32_MULT     6364136223846793005ULL
#define PCG32_INIT_SEQ 0xDEADBEEFULL

static kuint64_t pcg_state  = 0;
static kuint64_t pcg_inc    = 0;
static kbool	 pcg_inited = kfalse;

static inline kuint32_t
    krdtsc32 (void)
{
	kuint32_t lo;
	__asm__ volatile("rdtsc" : "=a"(lo)::"edx");
	return lo;
}

void
    ksrand32 (kuint64_t seed, kuint64_t seq)
{
	pcg_state = 0;
	pcg_inc	  = (seq << 1) | 1;
	// Advance the state a few times to mix up bits (PCG recommends at least once)
	for ( int i = 0; i < 3; i++ )
		pcg_state = pcg_state * PCG32_MULT + pcg_inc;
	pcg_state += seed;
	pcg_state  = pcg_state * PCG32_MULT + pcg_inc;
	pcg_inited = ktrue;
}

static void
    kensure_pcg_init (void)
{
	if ( !pcg_inited )
		{
			kuint32_t t = krdtsc32();
			ksrand32(t, t ^ PCG32_INIT_SEQ);
		}
}

kuint32_t
    k_u_rand32 (void)
{
	kensure_pcg_init();
	kuint64_t prev_state = pcg_state;
	pcg_state	     = prev_state * PCG32_MULT + pcg_inc;
	// Output function (XSH RR), as per PCG reference
	kuint32_t xorshifted = (kuint32_t) (((prev_state >> 18u) ^ prev_state) >> 27u);
	kuint32_t rot	     = (kuint32_t) (prev_state >> 59u);
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

kint32_t
    krand32 (void)
{
	kensure_pcg_init();
	kuint64_t prev_state = pcg_state;
	pcg_state	     = prev_state * PCG32_MULT + pcg_inc;
	// Output function (XSH RR), as per PCG reference
	kuint32_t xorshifted = (kuint32_t) (((prev_state >> 18u) ^ prev_state) >> 27u);
	kuint32_t rot	     = (kuint32_t) (prev_state >> 59u);
	return (kint32_t) ((xorshifted >> rot) | (xorshifted << ((-rot) & 31)));
}
