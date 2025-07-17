/* expm1.c */

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

#include "expm1.h"
#include "kmath.h"

double
    expm1 (double x)
{
        /* Handle special cases */
        if ( isnan(x) )
                return nan("");
        if ( isinf(x) )
                return x > 0 ? INFINITY : -1.0;

        /* For small values, use Taylor series to avoid loss of precision */
        if ( fabs(x) < 0.1 )
        {
                double x2 = x * x;
                double x3 = x2 * x;
                double x4 = x3 * x;
                double x5 = x4 * x;
                return x + x2 / 2.0 + x3 / 6.0 + x4 / 24.0 + x5 / 120.0;
        }

        /* For larger values, use exp(x) - 1 */
        return exp(x) - 1.0;
}
