/* klog.c */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "klog.h"
#include "kmath.h"

double
    klog (double x)
{
	/* Handle special cases */
	if ( x <= 0.0 )
	{
		/* Return NaN for non-positive numbers */
		return 0.0 / 0.0;
	}

	if ( x == 1.0 )
		return 0.0;

	/* Use Newton's method to find ln(x) */
	/* We solve: e^y = x, so y = ln(x) */
	double y = 0.0;
	double prev_y;

	/* Initial guess */
	if ( x > 1.0 )
	{
		y = 1.0;
	}
	else
	{
		y = -1.0;
	}

	/* Newton iteration: y_{n+1} = y_n + (x - e^y_n) / e^y_n */
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
