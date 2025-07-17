/* krint.c */

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
#include "krint.h"

double
    krint (double x)
{
	/* Handle special cases */
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;

	/* Round to nearest integer */
	if ( x >= 0.0 )
	{
		double frac = x - kfloor(x);
		if ( frac < 0.5 )
			return kfloor(x);
		else
			return kceil(x);
	}
	else
	{
		double frac = kceil(x) - x;
		if ( frac < 0.5 )
			return kceil(x);
		else
			return kfloor(x);
	}
}
