// kfmin.c

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

#include <wt/kmath/kfmin.h>
#include <wt/kmath/kmath.h>

double
    kfmin (double x, double y) {
	// Handle NaN cases
	if ( x != x )
		return y;
	if ( y != y )
		return x;

	return x < y ? x : y;
}
