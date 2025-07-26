// kceil.c

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

#include "kceil.h"

#include "kmath.h"

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
