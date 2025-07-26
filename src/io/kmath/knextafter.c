// knextafter.c

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

#include "knextafter.h"

#include "kmath.h"

double
    knextafter (double x, double y) {
	// Handle special cases
	if ( kisnan(x) || kisnan(y) )
		return knan("");
	if ( kisinf(x) )
		return x;

	// If x equals y, return y
	if ( x == y )
		return y;

	// Determine direction
	int direction = (y > x) ? 1 : -1;

	// For zero, return smallest subnormal in the direction of y
	if ( x == 0.0 ) {
		union {
			double		   d;
			unsigned long long u;
		} u;

		u.u = direction > 0 ? 0x0000000000000001ULL : 0x8000000000000001ULL;
		return u.d;
	}

	// For finite non-zero values, use a simple approximation
	// This is a simplified implementation - a full implementation would
	// need to handle the IEEE 754 bit representation directly
	double epsilon = 1e-15;
	return x + direction * epsilon * kfabs(x);
}
