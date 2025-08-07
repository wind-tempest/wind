// kfmin.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
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

