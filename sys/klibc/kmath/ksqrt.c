// ksqrt.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/ksqrt.h>

double
    ksqrt (double x)
{
	if ( x < 0.0 )
		{
			// Return NaN for negative numbers
			return 0.0 / 0.0;
		}

	if ( x == 0.0 || x == 1.0 )
		{
			return x;
		}

	// Newton-Raphson method for square root
	double guess = x / 2.0;
	double prev_guess;

	do
		{
			prev_guess = guess;
			guess	   = (guess + x / guess) / 2.0;
		}
	while ( kfabs(guess - prev_guess) > 1e-15 );

	return guess;
}
