/* atan2.c */

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

#include "atan2.h"
#include "kmath.h"

double
    atan2 (double y, double x)
{
        /* Handle special cases */
        if ( isnan(x) || isnan(y) )
                return nan("");
        if ( isinf(x) && isinf(y) )
                return nan("");

        /* If x is zero */
        if ( x == 0.0 )
        {
                if ( y > 0.0 )
                        return M_PI_2;
                if ( y < 0.0 )
                        return -M_PI_2;
                return 0.0; /* y is also zero */
        }

        /* If y is zero */
        if ( y == 0.0 )
        {
                if ( x > 0.0 )
                        return 0.0;
                if ( x < 0.0 )
                        return M_PI;
                return 0.0;
        }

        /* If x is infinite */
        if ( isinf(x) )
        {
                if ( x > 0.0 )
                        return y > 0.0 ? 0.0 : -0.0;
                else
                        return y > 0.0 ? M_PI : -M_PI;
        }

        /* If y is infinite */
        if ( isinf(y) )
        {
                return y > 0.0 ? M_PI_2 : -M_PI_2;
        }

        /* Normal case: use atan(y/x) with proper quadrant adjustment */
        double result = atan(y / x);

        if ( x < 0.0 )
        {
                if ( y >= 0.0 )
                        result += M_PI;
                else
                        result -= M_PI;
        }

        return result;
}
