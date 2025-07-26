// kfmod.c

// SPDX-License-Identifier: LSL-1.1
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.1 (LSL-1.1).
 * For more information, see <https://wind.infernointeractive.win>.
 */

// SPDX-License-Identifier: LSL-1.0

#include "kfmod.h"

#include "kmath.h"

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
