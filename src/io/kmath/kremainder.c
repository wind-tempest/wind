/* remainder.c */

/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "kmath.h"
#include "kremainder.h"

double
    kremainder (double x, double y)
{
        /* Handle special cases */
        if ( kisnan(x) || kisnan(y) )
                return knan("");
        if ( kisinf(x) )
                return knan("");
        if ( y == 0.0 )
                return knan("");
        if ( kisinf(y) )
                return x;

        /* Calculate remainder using fmod and adjust for rounding */
        double r = kfmod(x, y);

        /* Adjust to get remainder in the range [-|y|/2, |y|/2] */
        if ( kfabs(r) > kfabs(y) / 2.0 )
        {
                if ( r > 0 )
                        r -= kfabs(y);
                else
                        r += kfabs(y);
        }

        return r;
}
