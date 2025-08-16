// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kceil.h>
#include <tempest/klibc/kmath/kmath.h>

double
    kceil (double x) {
	if ( x <= 0.0 ) {
		// For negative numbers, truncate
		return (double) ((long long) x);
	} else {
		// For positive numbers, round up
		long long int_part = (long long) x;
		if ( x == (double) int_part ) {
			return x;
		}
		return (double) (int_part + 1);
	}
}

