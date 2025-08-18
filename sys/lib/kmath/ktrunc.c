// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kmath.h>
#include <lib/kmath/ktrunc.h>

double
    ktrunc (double x) {
	// Handle special cases
	if (kisnan(x))
		return knan("");
	if (kisinf(x))
		return x;

	// For positive numbers, truncate towards zero
	if (x >= 0.0)
		return kfloor(x);
	else
		return kceil(x);
}
