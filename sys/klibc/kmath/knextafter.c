// knextafter.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kmath.h>
#include <tempest/klibc/kmath/knextafter.h>

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
