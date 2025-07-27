// kfloor.c

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include <wt/kmath/kfloor.h>

#include <wt/kmath/kmath.h>

double
    kfloor (double x) {
	if ( x >= 0.0 ) {
		// For positive numbers, truncate
		return (double) ((long long) x);
	} else {
		// For negative numbers, round down
		long long int_part = (long long) x;
		if ( x == (double) int_part ) {
			return x;
		}
		return (double) (int_part - 1);
	}
}
