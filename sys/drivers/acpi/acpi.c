// acpi.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kasm/kio.h>
#include <wt/kdebug/kdebug.h>
#include <wt/kstdio/kstddef.h>
#include <wt/kunistd/ksleep.h>

/*
 * THIS IS NOT ACPI YET! THIS IS A PLACEHOLDER FOR ACPI
 * SINCE I WILL POLISH THE TEMPEST KERNEL BEFORE I DECIDE
 * TO LOSE MY PRECIOUS TIME ON ACPI.
 */

// Define timeout for poweroff in milliseconds
#define POWEROFF_TIMEOUT_MS 5000

void
    kpoweroff (void) {
	koutw(0x604, 0x2000);	// Port 0x604
	koutw(0xB004, 0x2000);	// Port 0xB004

	ksleep(POWEROFF_TIMEOUT_MS);
	// If poweroff fails, log warning
	kerror("Legacy poweroff failed. System may not shut down.", "acpi", KNULL);
}

void
    kreboot (void) {
	while ( kinb(0x64) & 0x02 )
		;	    // Wait for keyboard controller ready
	koutb(0x64, 0xFE);  // Send reset command

	ksleep(POWEROFF_TIMEOUT_MS);
	// If reboot fails, log warning
	kerror("Legacy reboot failed. System may not restart.", "acpi", KNULL);
}
