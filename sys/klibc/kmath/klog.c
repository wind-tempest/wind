// klog.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/klog.h>
#include <wt/kmath/kmath.h>

double
    klog (double x)
{
	// Handle special cases
	if ( x <= 0.0 )
	{
		// Return NaN for non-positive numbers
		return 0.0 / 0.0;
	}

	if ( x == 1.0 )
		return 0.0;

	// Use Newton's method to find ln(x)
	// We solve: e^y = x, so y = ln(x)
	double y = 0.0;
	double prev_y;

	// Initial guess
	if ( x > 1.0 )
	{
		y = 1.0;
	}
	else
	{
		y = -1.0;
	}

	// Newton iteration: y_{n+1} = y_n + (x - e^y_n) / e^y_n
	for ( int i = 0; i < 10; i++ )
	{
		prev_y	     = y;
		double exp_y = kexp(y);
		y	     = y + (x - exp_y) / exp_y;

		if ( kfabs(y - prev_y) < 1e-15 )
		{
			break;
		}
	}

	return y;
}
