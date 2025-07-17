/* katan.c */

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
