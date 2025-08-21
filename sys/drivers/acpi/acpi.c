// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kasm/kio.h>
#include <lib/kdebug/kdebug.h>
#include <lib/kstdio/kstddef.h>
#include <lib/kunistd/ksleep.h>

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
	koutw(0x604, 0x2000);   // Port 0x604
	koutw(0xB004, 0x2000);  // Port 0xB004

	ksleep(POWEROFF_TIMEOUT_MS);
	// If poweroff fails, log warning
	kerr("Legacy poweroff failed", "acpi", KNULL);
}

void
    kreboot (void) {
	while (kinb(0x64) & 0x02)
		;           // Wait for keyboard controller ready
	koutb(0x64, 0xFE);  // Send reset command

	ksleep(POWEROFF_TIMEOUT_MS);
	// If reboot fails, log warning
	kerr("Legacy reboot failed", "acpi", KNULL);
}
