/* klog1p.c */

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

#include "klog1p.h"
#include "kmath.h"

double
    klog1p (double x) {
	/* Handle special cases */
	if ( kisnan(x) )
		return knan("");
	if ( x < -1.0 )
		return knan("");
	if ( x == -1.0 )
		return -K_INFINITY;
	if ( kisinf(x) )
		return x > 0 ? K_INFINITY : knan("");

	/* For small values, use Taylor series to avoid loss of precision */
	if ( kfabs(x) < 0.1 ) {
		double x2 = x * x;
		double x3 = x2 * x;
		double x4 = x3 * x;
		double x5 = x4 * x;
		return x - x2 / 2.0 + x3 / 3.0 - x4 / 4.0 + x5 / 5.0;
	}

	/* For larger values, use log(1 + x) */
	return klog(1.0 + x);
}
