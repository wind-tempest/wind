// kremquo.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/kremquo.h>

double
    kremquo (double x, double y, int *quo)
{
	// Handle special cases
	if ( kisnan(x) || kisnan(y) )
		{
			*quo = 0;
			return knan("");
		}
	if ( kisinf(x) )
		{
			*quo = 0;
			return knan("");
		}
	if ( y == 0.0 )
		{
			*quo = 0;
			return knan("");
		}
	if ( kisinf(y) )
		{
			*quo = 0;
			return x;
		}

	// Calculate quotient
	*quo = (int) kround(x / y);

	// Calculate remainder
	double r = x - *quo * y;

	// Adjust to get remainder in the range [-|y|/2, |y|/2]
	if ( kfabs(r) > kfabs(y) / 2.0 )
		{
			if ( r > 0 )
				{
					r -= kfabs(y);
					(*quo)++;
				}
			else
				{
					r += kfabs(y);
					(*quo)--;
				}
		}

	return r;
}
