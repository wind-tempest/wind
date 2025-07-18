/* ksleep.c */

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

#include "ksignal.h"
#include "ksleep.h"
#include "kstddef.h"
#include "kstdint.h"

volatile uint64_t *hpet = (volatile uint64_t *) 0xFED00000;

#define HPET_GEN_CONF	     0x10
#define HPET_MAIN_COUNTER    0xF0
#define HPET_CAP_ID	     0x0
#define HPET_GEN_CONF_ENABLE (1 << 0)

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43

void
    khpet_enable (void)
{
	if ( (hpet[HPET_GEN_CONF / 8] & HPET_GEN_CONF_ENABLE) == 0 )
	{
		hpet[HPET_GEN_CONF / 8] |= HPET_GEN_CONF_ENABLE;
	}
}

/* PIT busy-wait ~1ms per tick */
void
    kpit_wait (int ms)
{
	koutb(PIT_COMMAND, 0x34);

	uint16_t reload = 1193; /* ~1 ms (1193182 Hz / 1000) */
	koutb(PIT_CHANNEL0, (uint8_t) (reload & 0xFF));
	koutb(PIT_CHANNEL0, (uint8_t) ((reload >> 8) & 0xFF));

	for ( int i = 0; i < ms; i++ )
	{
		uint8_t prev = 0xFF, curr = 0;
		do
		{
			curr = kinb(PIT_CHANNEL0);
			if ( curr > prev )
				break;
			prev = curr;
		} while ( curr != 0 );
	}
}

void
    ksleep (int ms)
{
	if ( hpet != NULL )
	{
		khpet_enable();

		uint64_t period_fs = hpet[HPET_CAP_ID / 8] >> 32;
		uint64_t start	   = hpet[HPET_MAIN_COUNTER / 8];

		uint64_t ticks = (uint64_t) ms * 1000000000ULL / (period_fs / 1000);

		while ( (hpet[HPET_MAIN_COUNTER / 8] - start) < ticks )
		{
			kcpu_relax();
		}
	}
	else
	{
		kpit_wait(ms);
	}
}
