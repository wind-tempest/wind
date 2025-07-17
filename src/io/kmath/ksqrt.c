/* sqrt.c */

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
#include "sqrt.h"

double
    sqrt (double x)
{
        if ( x < 0.0 )
        {
                /* Return NaN for negative numbers */
                return 0.0 / 0.0;
        }

        if ( x == 0.0 || x == 1.0 )
        {
                return x;
        }

        /* Newton-Raphson method for square root */
        double guess = x / 2.0;
        double prev_guess;

        do
        {
                prev_guess = guess;
                guess      = (guess + x / guess) / 2.0;
        } while ( fabs(guess - prev_guess) > 1e-15 );

        return guess;
}
