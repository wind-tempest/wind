/* kcbrt.c */

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

#include "kcbrt.h"
#include "kmath.h"

double
    kcbrt (double x) {
	/* Handle special cases */
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;
	if ( x == 0.0 )
		return 0.0;

	/* Use Newton's method for cube root */
	double y = kfabs(x);
	double guess;

	/* Initial guess */
	if ( y > 1.0 )
		guess = y / 3.0;
	else
		guess = y * 2.0 / 3.0;

	/* Newton iteration: x_{n+1} = (2*x_n + y/x_n^2) / 3 */
	for ( int i = 0; i < 10; i++ ) {
		double guess2	 = guess * guess;
		double new_guess = (2.0 * guess + y / guess2) / 3.0;
		if ( kfabs(new_guess - guess) < 1e-15 )
			break;
		guess = new_guess;
	}

	return x < 0.0 ? -guess : guess;
}
