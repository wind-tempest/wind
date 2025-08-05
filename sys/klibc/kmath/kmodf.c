// kmodf.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/kmodf.h>

double
    kmodf (double x, double *iptr)
{
	// Handle special cases
	if ( kisnan(x) )
	{
		*iptr = knan("");
		return knan("");
	}
	if ( kisinf(x) )
	{
		*iptr = x;
		return 0.0;
	}

	// Extract integer part
	if ( x >= 0.0 )
		*iptr = kfloor(x);
	else
		*iptr = kceil(x);

	// Return fractional part
	return x - *iptr;
}
