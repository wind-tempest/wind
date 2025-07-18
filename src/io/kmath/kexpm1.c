/* kexpm1.c */

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

#include "kexpm1.h"
#include "kmath.h"

double
    kexpm1 (double x)
{
	/* Handle special cases */
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x > 0 ? K_INFINITY : -1.0;

	/* For small values, use Taylor series to avoid loss of precision */
	if ( kfabs(x) < 0.1 )
	{
		double x2 = x * x;
		double x3 = x2 * x;
		double x4 = x3 * x;
		double x5 = x4 * x;
		return x + x2 / 2.0 + x3 / 6.0 + x4 / 24.0 + x5 / 120.0;
	}

	/* For larger values, use exp(x) - 1 */
	return kexp(x) - 1.0;
}
