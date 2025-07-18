/* ksin.c */

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

#include "kmath.h"
#include "ksin.h"

double
    ksin (double x)
{
	/* Normalize x to [-π, π] */
	x = kfmod(x, 2.0 * K_M_PI);
	if ( x > K_M_PI )
		x -= 2.0 * K_M_PI;
	if ( x < -K_M_PI )
		x += 2.0 * K_M_PI;

	/* Use Taylor series for small values */
	if ( kfabs(x) < 0.1 )
	{
		double x2 = x * x;
		double x3 = x2 * x;
		double x5 = x3 * x2;
		double x7 = x5 * x2;
		return x - x3 / 6.0 + x5 / 120.0 - x7 / 5040.0;
	}

	/* For larger values, use more terms of Taylor series */
	double result	 = 0.0;
	double term	 = x;
	double x2	 = x * x;
	double factorial = 1.0;
	int    sign	 = 1;

	for ( int i = 1; i <= 10; i++ )
	{
		result += sign * term / factorial;
		term *= x2;
		factorial *= (2 * i) * (2 * i + 1);
		sign = -sign;
	}

	return result;
}
