/* acpi.c */

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

#include "dlog.h"
#include "ksignal.h"
#include "ksleep.h"
#include "kstddef.h"

/* Define timeout for poweroff in milliseconds */
#define POWEROFF_TIMEOUT_MS 5000

void
    kpoweroff (void) {
	koutw(0x604, 0x2000);  /* Port 0x604 */
	koutw(0xB004, 0x2000); /* Port 0xB004 */

	ksleep(POWEROFF_TIMEOUT_MS);
	kerror("Legacy poweroff failed. System may not shut down.", KNULL);
}

void
    kreboot (void) {
	while ( kinb(0x64) & 0x02 )
		;	   /* Wait for keyboard controller ready */
	koutb(0x64, 0xFE); /* Send reset command */

	ksleep(POWEROFF_TIMEOUT_MS);
	/* If reboot fails, log warning */
	kerror("Legacy reboot failed. System may not restart.", KNULL);
}
