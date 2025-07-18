/* krand.c */

/*
 * ---------------------------------------------------------------------------
 *  Wind (operating system) / Tempest (kernel)
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ---------------------------------------------------------------------------
 *  This file is part of the Wind/Tempest project, an open-source operating system
 *  and kernel designed to deliver performance, stability, and freedom to users.
 *
 *  - License:
 *  This software is licensed under the GNU Affero General Public License (AGPL)
 *  version 3 or any later version. You may redistribute and/or modify it under
 *  the terms of the AGPL as published by the Free Software Foundation.
 *
 *  - Warranty Disclaimer:
 *  Wind/Tempest is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 *  details.
 *
 *  - Permissions:
 *  > You may modify this file and include your own copyright notices below.
 *  > Forks and contributions are welcome, as long as this license is respected.
 *
 *  - Additional Information:
 *  For more information about the project, visit:
 *  https://wind.infernointeractive.win
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 * ---------------------------------------------------------------------------
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
