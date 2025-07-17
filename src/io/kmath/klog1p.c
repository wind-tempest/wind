/* klog1p.c */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "klog1p.h"
#include "kmath.h"

double
    klog1p (double x)
{
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
	if ( kfabs(x) < 0.1 )
	{
		double x2 = x * x;
		double x3 = x2 * x;
		double x4 = x3 * x;
		double x5 = x4 * x;
		return x - x2 / 2.0 + x3 / 3.0 - x4 / 4.0 + x5 / 5.0;
	}

	/* For larger values, use log(1 + x) */
	return klog(1.0 + x);
}
