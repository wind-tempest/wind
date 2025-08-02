// ktrunc.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/ktrunc.h>

double
    ktrunc (double x) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;

	// For positive numbers, truncate towards zero
	if ( x >= 0.0 )
		return kfloor(x);
	else
		return kceil(x);
}
