// kldexp.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kldexp.h>
#include <wt/kmath/kmath.h>

double
    kldexp (double x, int exp)
{
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;
	if ( x == 0.0 )
		return 0.0;

	// Use the relationship: ldexp(x, exp) = x * 2^exp
	return x * kpow(2.0, exp);
}
