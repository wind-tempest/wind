// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kmath.h>
#include <tempest/klibc/kmath/ktrunc.h>

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
