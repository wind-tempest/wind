/* kasin.c */

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

#include "kasin.h"
#include "kmath.h"

double
    kasin (double x)
{
	/* Check for invalid input */
	if ( x < -1.0 || x > 1.0 )
		return knan("");

	/* Use Newton's method to find arcsin */
	if ( kfabs(x) < 0.5 )
	{
		double y  = x;
		double y2 = y * y;
		double y3 = y2 * y;
		double y5 = y3 * y2;
		double y7 = y5 * y2;
		return y + y3 / 6.0 + 3.0 * y5 / 40.0 + 5.0 * y7 / 112.0;
	}

	/* For values closer to ±1, use different approach */
	if ( x > 0 )
		return K_M_PI_2 - kasin(ksqrt(1.0 - x * x));
	else
		return -K_M_PI_2 + kasin(ksqrt(1.0 - x * x));
}
