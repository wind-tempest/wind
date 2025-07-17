/* kremquo.c */

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
#include "kremquo.h"

double
    kremquo (double x, double y, int *quo)
{
	/* Handle special cases */
	if ( kisnan(x) || kisnan(y) )
	{
		*quo = 0;
		return knan("");
	}
	if ( kisinf(x) )
	{
		*quo = 0;
		return knan("");
	}
	if ( y == 0.0 )
	{
		*quo = 0;
		return knan("");
	}
	if ( kisinf(y) )
	{
		*quo = 0;
		return x;
	}

	/* Calculate quotient */
	*quo = (int) kround(x / y);

	/* Calculate remainder */
	double r = x - *quo * y;

	/* Adjust to get remainder in the range [-|y|/2, |y|/2] */
	if ( kfabs(r) > kfabs(y) / 2.0 )
	{
		if ( r > 0 )
		{
			r -= kfabs(y);
			(*quo)++;
		}
		else
		{
			r += kfabs(y);
			(*quo)--;
		}
	}

	return r;
}
