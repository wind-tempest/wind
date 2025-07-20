/* kpow.c */

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
#include "kpow.h"

double
    kpow (double x, double y) {
	/* Handle special cases */
	if ( x == 0.0 ) {
		if ( y > 0.0 )
			return 0.0;
		if ( y == 0.0 )
			return 1.0;
		/* y < 0: return infinity */
		return 1.0 / 0.0;
	}

	if ( x == 1.0 )
		return 1.0;

	if ( y == 0.0 )
		return 1.0;
	if ( y == 1.0 )
		return x;
	if ( y == 2.0 )
		return x * x;
	if ( y == 0.5 )
		return ksqrt(x);

	/* For integer powers, use repeated multiplication */
	if ( y == (int) y && y > 0 && y < 100 ) {
		double result = 1.0;
		for ( int i = 0; i < (int) y; i++ ) {
			result *= x;
		}
		return result;
	}

	/* For negative integer powers */
	if ( y == (int) y && y < 0 && y > -100 ) {
		double result = 1.0;
		for ( int i = 0; i < -(int) y; i++ ) {
			result /= x;
		}
		return result;
	}

	/* General case: use exp(y * log(x)) */
	return kexp(y * klog(x));
}
