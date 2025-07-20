/* ksleep.c */

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

#include "ksignal.h"
#include "ksleep.h"
#include "kstddef.h"
#include "kstdint.h"

volatile kuint64_t *hpet = (volatile kuint64_t *) 0xFED00000;

#define HPET_GEN_CONF	     0x10
#define HPET_MAIN_COUNTER    0xF0
#define HPET_CAP_ID	     0x0
#define HPET_GEN_CONF_ENABLE (1 << 0)

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43

void
    khpet_enable (void) {
	if ( (hpet[HPET_GEN_CONF / 8] & HPET_GEN_CONF_ENABLE) == 0 ) {
		hpet[HPET_GEN_CONF / 8] |= HPET_GEN_CONF_ENABLE;
	}
}

/* PIT busy-wait ~1ms per tick */
void
    kpit_wait (int ms) {
	koutb(PIT_COMMAND, 0x34);

	kuint16_t reload = 1193; /* ~1 ms (1193182 Hz / 1000) */
	koutb(PIT_CHANNEL0, (kuint8_t) (reload & 0xFF));
	koutb(PIT_CHANNEL0, (kuint8_t) ((reload >> 8) & 0xFF));

	for ( int i = 0; i < ms; i++ ) {
		kuint8_t prev = 0xFF, curr = 0;
		do {
			curr = kinb(PIT_CHANNEL0);
			if ( curr > prev )
				break;
			prev = curr;
		} while ( curr != 0 );
	}
}

void
    ksleep (int ms) {
	if ( hpet != KNULL ) {
		khpet_enable();

		kuint64_t period_fs = hpet[HPET_CAP_ID / 8] >> 32;
		kuint64_t start	    = hpet[HPET_MAIN_COUNTER / 8];

		kuint64_t ticks = (kuint64_t) ms * 1000000000ULL / (period_fs / 1000);

		while ( (hpet[HPET_MAIN_COUNTER / 8] - start) < ticks ) {
			kcpu_relax();
		}
	} else {
		kpit_wait(ms);
	}
}
