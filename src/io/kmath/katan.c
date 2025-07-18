/* katan.c */

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

#include "katan.h"
#include "kmath.h"

double
    katan (double x)
{
	/* Handle special cases */
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x > 0 ? K_M_PI_2 : -K_M_PI_2;
	if ( x == 0.0 )
		return 0.0;

	/* Use series expansion for small values */
	if ( kfabs(x) < 1.0 )
	{
		double x2 = x * x;
		double x3 = x2 * x;
		double x5 = x3 * x2;
		double x7 = x5 * x2;
		double x9 = x7 * x2;
		return x - x3 / 3.0 + x5 / 5.0 - x7 / 7.0 + x9 / 9.0;
	}

	/* For large values, use atan(x) = π/2 - atan(1/x) */
	if ( x > 0 )
		return K_M_PI_2 - katan(1.0 / x);
	else
		return -K_M_PI_2 - katan(1.0 / x);
}
