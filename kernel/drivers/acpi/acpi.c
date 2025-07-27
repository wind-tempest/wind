// acpi.c

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include <wt/kdebug.h>
#include <wt/ksignal.h>
#include <wt/ksleep.h>
#include <wt/kstddef.h>

// Define timeout for poweroff in milliseconds
#define POWEROFF_TIMEOUT_MS 5000

void
    kpoweroff (void) {
	koutw(0x604, 0x2000);  // Port 0x604
	koutw(0xB004, 0x2000); // Port 0xB004

	ksleep(POWEROFF_TIMEOUT_MS);
	// If poweroff fails, log warning
	kerror("Legacy poweroff failed. System may not shut down.", KNULL);
}

void
    kreboot (void) {
	while ( kinb(0x64) & 0x02 )
		;	   // Wait for keyboard controller ready
	koutb(0x64, 0xFE); // Send reset command

	ksleep(POWEROFF_TIMEOUT_MS);
	// If reboot fails, log warning
	kerror("Legacy reboot failed. System may not restart.", KNULL);
}
