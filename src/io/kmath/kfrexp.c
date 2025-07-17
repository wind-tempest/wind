/* kfrexp.c */

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

#include "kfrexp.h"
#include "kmath.h"

double
    kfrexp (double x, int *exp)
{
	/* Handle special cases */
	if ( kisnan(x) )
	{
		*exp = 0;
		return knan("");
	}
	if ( kisinf(x) )
	{
		*exp = 0;
		return x;
	}
	if ( x == 0.0 )
	{
		*exp = 0;
		return 0.0;
	}

	/* Extract exponent using log2 */
	*exp = (int) kfloor(klog2(kfabs(x))) + 1;

	/* Calculate mantissa: x / 2^exp */
	return x / kpow(2.0, *exp);
}
