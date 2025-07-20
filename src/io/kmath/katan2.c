/* katan2.c */

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

#include "katan2.h"
#include "kmath.h"

double
    katan2 (double y, double x) {
	/* Handle special cases */
	if ( kisnan(x) || kisnan(y) )
		return knan("");
	if ( kisinf(x) && kisinf(y) )
		return knan("");

	/* If x is zero */
	if ( x == 0.0 ) {
		if ( y > 0.0 )
			return K_M_PI_2;
		if ( y < 0.0 )
			return -K_M_PI_2;
		return 0.0; /* y is also zero */
	}

	/* If y is zero */
	if ( y == 0.0 ) {
		if ( x > 0.0 )
			return 0.0;
		if ( x < 0.0 )
			return K_M_PI;
		return 0.0;
	}

	/* If x is infinite */
	if ( kisinf(x) ) {
		if ( x > 0.0 )
			return y > 0.0 ? 0.0 : -0.0;
		else
			return y > 0.0 ? K_M_PI : -K_M_PI;
	}

	/* If y is infinite */
	if ( kisinf(y) ) {
		return y > 0.0 ? K_M_PI_2 : -K_M_PI_2;
	}

	/* Normal case: use atan(y/x) with proper quadrant adjustment */
	double result = katan(y / x);

	if ( x < 0.0 ) {
		if ( y >= 0.0 )
			result += K_M_PI;
		else
			result -= K_M_PI;
	}

	return result;
}
