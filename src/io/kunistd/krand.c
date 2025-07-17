/* rand.c */

/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "kstdbool.h"
#include "kstdint.h"

static uint64_t pcg_state;
static uint64_t pcg_inc;
static bool     pcg_inited = false;

static inline uint32_t
    krdtsc32 (void)
{
        uint32_t lo;
        asm volatile("rdtsc" : "=a"(lo)::"edx");
        return lo;
}

void
    ksrand32 (uint64_t seed, uint64_t seq)
{
        pcg_state = 0;
        pcg_inc   = (seq << 1) | 1;
        for ( int i = 0; i < 3; i++ )
                pcg_state = pcg_state * 6364136223846793005ULL + pcg_inc;
        pcg_state += seed;
        pcg_state  = pcg_state * 6364136223846793005ULL + pcg_inc;
        pcg_inited = true;
}

static void
    kensure_pcg_init (void)
{
        if ( !pcg_inited )
        {
                uint32_t t = krdtsc32();
                ksrand32(t, t ^ 0xDEADBEEFULL);
        }
}

uint32_t
    krand32 (void)
{
        kensure_pcg_init();
        uint64_t old   = pcg_state;
        pcg_state      = old * 6364136223846793005ULL + pcg_inc;
        uint32_t xorsh = (uint32_t) (((old >> 18) ^ old) >> 27);
        uint32_t rot   = (uint32_t) (old >> 59);
        return (xorsh >> rot) | (xorsh << ((-rot) & 31));
}

int32_t
    krand32 (void)
{
        return (int32_t) krand32();
}
