// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kfmax.h>
#include <lib/kmath/kmath.h>

double
    kfmax (double x, double y) {
	// Handle NaN cases
	if (x != x)
		return y;
	if (y != y)
		return x;

	return x > y ? x : y;
}
