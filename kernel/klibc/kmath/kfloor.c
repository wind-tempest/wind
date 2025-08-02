// kfloor.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
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
