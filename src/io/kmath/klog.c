/* klog.c */

/*
 * ---------------------------------------------------------------------------
 *  Wind (operating system) / Tempest (kernel)
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ---------------------------------------------------------------------------
 *  This file is part of the Wind/Tempest project, an open-source operating system
 *  and kernel designed to deliver performance, stability, and freedom to users.
 *
 *  - License:
 *  This software is licensed under the GNU Affero General Public License (AGPL)
 *  version 3 or any later version. You may redistribute and/or modify it under
 *  the terms of the AGPL as published by the Free Software Foundation.
 *
 *  - Warranty Disclaimer:
 *  Wind/Tempest is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 *  details.
 *
 *  - Permissions:
 *  > You may modify this file and include your own copyright notices below.
 *  > Forks and contributions are welcome, as long as this license is respected.
 *
 *  - Additional Information:
 *  For more information about the project, visit:
 *  https://wind.infernointeractive.win
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 * ---------------------------------------------------------------------------
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
