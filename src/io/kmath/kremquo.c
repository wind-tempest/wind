/* kremquo.c */

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
#include "kremquo.h"

double
    kremquo (double x, double y, int *quo) {
	/* Handle special cases */
	if ( kisnan(x) || kisnan(y) ) {
		*quo = 0;
		return knan("");
	}
	if ( kisinf(x) ) {
		*quo = 0;
		return knan("");
	}
	if ( y == 0.0 ) {
		*quo = 0;
		return knan("");
	}
	if ( kisinf(y) ) {
		*quo = 0;
		return x;
	}

	/* Calculate quotient */
	*quo = (int) kround(x / y);

	/* Calculate remainder */
	double r = x - *quo * y;

	/* Adjust to get remainder in the range [-|y|/2, |y|/2] */
	if ( kfabs(r) > kfabs(y) / 2.0 ) {
		if ( r > 0 ) {
			r -= kfabs(y);
			(*quo)++;
		} else {
			r += kfabs(y);
			(*quo)--;
		}
	}

	return r;
}
