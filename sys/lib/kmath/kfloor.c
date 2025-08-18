// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kfloor.h>
#include <lib/kmath/kmath.h>

double
    kfloor (double x) {
	if (x >= 0.0) {
		// For positive numbers, truncate
		return (double) ((long long) x);
	} else {
		// For negative numbers, round down
		long long int_part = (long long) x;
		if (x == (double) int_part) {
			return x;
		}
		return (double) (int_part - 1);
	}
}
