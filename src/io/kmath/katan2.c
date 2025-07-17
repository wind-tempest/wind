/* katan2.c */

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

#include "katan2.h"
#include "kmath.h"

double
    katan2 (double y, double x)
{
	/* Handle special cases */
	if ( kisnan(x) || kisnan(y) )
		return knan("");
	if ( kisinf(x) && kisinf(y) )
		return knan("");

	/* If x is zero */
	if ( x == 0.0 )
	{
		if ( y > 0.0 )
			return K_M_PI_2;
		if ( y < 0.0 )
			return -K_M_PI_2;
		return 0.0; /* y is also zero */
	}

	/* If y is zero */
	if ( y == 0.0 )
	{
		if ( x > 0.0 )
			return 0.0;
		if ( x < 0.0 )
			return K_M_PI;
		return 0.0;
	}

	/* If x is infinite */
	if ( kisinf(x) )
	{
		if ( x > 0.0 )
			return y > 0.0 ? 0.0 : -0.0;
		else
			return y > 0.0 ? K_M_PI : -K_M_PI;
	}

	/* If y is infinite */
	if ( kisinf(y) )
	{
		return y > 0.0 ? K_M_PI_2 : -K_M_PI_2;
	}

	/* Normal case: use atan(y/x) with proper quadrant adjustment */
	double result = katan(y / x);

	if ( x < 0.0 )
	{
		if ( y >= 0.0 )
			result += K_M_PI;
		else
			result -= K_M_PI;
	}

	return result;
}
