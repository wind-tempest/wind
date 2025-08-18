// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kfmod.h>
#include <lib/kmath/kmath.h>

double
    kfmod (double x, double y) {
	if (y == 0.0) {
		// Return NaN for division by zero
		return 0.0 / 0.0;
	}

	if (x == 0.0) {
		return 0.0;
	}

	// Calculate remainder using: x - y * floor(x/y)
	double quotient = x / y;
	double int_part = kfloor(quotient);
	return x - y * int_part;
}
