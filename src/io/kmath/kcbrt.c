/* cbrt.c */

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

#include "cbrt.h"
#include "kmath.h"

double
    cbrt (double x)
{
        /* Handle special cases */
        if ( isnan(x) )
                return nan("");
        if ( isinf(x) )
                return x;
        if ( x == 0.0 )
                return 0.0;

        /* Use Newton's method for cube root */
        double y     = fabs(x);
        double guess = y;

        /* Initial guess */
        if ( y > 1.0 )
                guess = y / 3.0;
        else
                guess = y * 2.0 / 3.0;

        /* Newton iteration: x_{n+1} = (2*x_n + y/x_n^2) / 3 */
        for ( int i = 0; i < 10; i++ )
        {
                double guess2    = guess * guess;
                double new_guess = (2.0 * guess + y / guess2) / 3.0;
                if ( fabs(new_guess - guess) < 1e-15 )
                        break;
                guess = new_guess;
        }

        return x < 0.0 ? -guess : guess;
}
