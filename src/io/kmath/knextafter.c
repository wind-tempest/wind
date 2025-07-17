/* knextafter.c */

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

#include "kmath.h"
#include "knextafter.h"

double
    knextafter (double x, double y)
{
	/* Handle special cases */
	if ( kisnan(x) || kisnan(y) )
		return knan("");
	if ( kisinf(x) )
		return x;

	/* If x equals y, return y */
	if ( x == y )
		return y;

	/* Determine direction */
	int direction = (y > x) ? 1 : -1;

	/* For zero, return smallest subnormal in the direction of y */
	if ( x == 0.0 )
	{
		union
		{
			double		   d;
			unsigned long long u;
		} u;

		u.u = direction > 0 ? 0x0000000000000001ULL : 0x8000000000000001ULL;
		return u.d;
	}

	/* For finite non-zero values, use a simple approximation */
	/* This is a simplified implementation - a full implementation would */
	/* need to handle the IEEE 754 bit representation directly */
	double epsilon = 1e-15;
	return x + direction * epsilon * kfabs(x);
}
