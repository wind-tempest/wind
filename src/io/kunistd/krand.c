/* krand.c */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "kstdbool.h"
#include "kstdint.h"

#define PCG32_MULT     6364136223846793005ULL
#define PCG32_INIT_SEQ 0xDEADBEEFULL

static uint64_t pcg_state  = 0;
static uint64_t pcg_inc	   = 0;
static bool	pcg_inited = false;

static inline uint32_t
    krdtsc32 (void)
{
	uint32_t lo;
	__asm__ volatile("rdtsc" : "=a"(lo)::"edx");
	return lo;
}

void
    ksrand32 (uint64_t seed, uint64_t seq)
{
	pcg_state = 0;
	pcg_inc	  = (seq << 1) | 1;
	/* Advance the state a few times to mix up bits (PCG recommends at least once) */
	for ( int i = 0; i < 3; i++ )
		pcg_state = pcg_state * PCG32_MULT + pcg_inc;
	pcg_state += seed;
	pcg_state  = pcg_state * PCG32_MULT + pcg_inc;
	pcg_inited = true;
}

static void
    kensure_pcg_init (void)
{
	if ( !pcg_inited )
	{
		uint32_t t = krdtsc32();
		ksrand32(t, t ^ PCG32_INIT_SEQ);
	}
}

uint32_t
    k_u_rand32 (void)
{
	kensure_pcg_init();
	uint64_t prev_state = pcg_state;
	pcg_state	    = prev_state * PCG32_MULT + pcg_inc;
	/* Output function (XSH RR), as per PCG reference */
	uint32_t xorshifted = (uint32_t) (((prev_state >> 18u) ^ prev_state) >> 27u);
	uint32_t rot	    = (uint32_t) (prev_state >> 59u);
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

int32_t
    krand32 (void)
{
	kensure_pcg_init();
	uint64_t prev_state = pcg_state;
	pcg_state	    = prev_state * PCG32_MULT + pcg_inc;
	/* Output function (XSH RR), as per PCG reference */
	uint32_t xorshifted = (uint32_t) (((prev_state >> 18u) ^ prev_state) >> 27u);
	uint32_t rot	    = (uint32_t) (prev_state >> 59u);
	return (int32_t) ((xorshifted >> rot) | (xorshifted << ((-rot) & 31)));
}
