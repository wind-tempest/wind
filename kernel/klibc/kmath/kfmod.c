// kfmod.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kfmod.h>
#include <wt/kmath/kmath.h>

double
    kfmod (double x, double y) {
	if ( y == 0.0 ) {
		// Return NaN for division by zero
		return 0.0 / 0.0;
	}

	if ( x == 0.0 ) {
		return 0.0;
	}

	// Calculate remainder using: x - y * floor(x/y)
	double quotient = x / y;
	double int_part = kfloor(quotient);
	return x - y * int_part;
}
