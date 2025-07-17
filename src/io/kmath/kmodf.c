/* modf.c */

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
#include "kmodf.h"

double
    kmodf (double x, double *iptr)
{
        /* Handle special cases */
        if ( kisnan(x) )
        {
                *iptr = knan("");
                return knan("");
        }
        if ( kisinf(x) )
        {
                *iptr = x;
                return 0.0;
        }

        /* Extract integer part */
        if ( x >= 0.0 )
                *iptr = kfloor(x);
        else
                *iptr = kceil(x);

        /* Return fractional part */
        return x - *iptr;
}
