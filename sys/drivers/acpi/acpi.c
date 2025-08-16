// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kasm/kio.h>
#include <tempest/klibc/kdebug/kdebug.h>
#include <tempest/klibc/kstdio/kstddef.h>
#include <tempest/klibc/kunistd/ksleep.h>

/*
 * This is not ACPI yet, this is a placeholder for ACPI.
 * Since I will polish the Tempest kernel before I decide
 * to lose my precious time on ACPI.
 */

// ? Maybe use uACPI?

// Define timeout for poweroff in milliseconds
#define POWEROFF_TIMEOUT_MS 5000

void
    kpoweroff (void) {
	koutw(0x604, 0x2000);	// Port 0x604
	koutw(0xB004, 0x2000);	// Port 0xB004

	ksleep(POWEROFF_TIMEOUT_MS);
	// If poweroff fails, log warning
	kerror("Legacy poweroff failed", "acpi", KNULL);
}

void
    kreboot (void) {
	while ( kinb(0x64) & 0x02 )
		;	    // Wait for keyboard controller ready
	koutb(0x64, 0xFE);  // Send reset command

	ksleep(POWEROFF_TIMEOUT_MS);
	// If reboot fails, log warning
	kerror("Legacy reboot failed", "acpi", KNULL);
}
