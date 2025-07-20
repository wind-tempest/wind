/* kexp.c */

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

#include "kexp.h"
#include "kmath.h"

double
    kexp (double x) {
	/* Handle special cases */
	if ( x == 0.0 )
		return 1.0;
	if ( x < -700.0 )
		return 0.0; /* Underflow */
	if ( x > 700.0 )
		return 1.0 / 0.0; /* Overflow */

	/* Use Taylor series: e^x = 1 + x + x^2/2! + x^3/3! + ... */
	double result	 = 1.0;
	double term	 = 1.0;
	double factorial = 1.0;

	for ( int i = 1; i <= 20; i++ ) {
		term *= x;
		factorial *= i;
		result += term / factorial;
	}

	return result;
}
