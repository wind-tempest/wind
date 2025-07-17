/* hypot.c */

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

#include "khypot.h"
#include "kmath.h"

double
    khypot (double x, double y)
{
        /* Handle special cases */
        if ( isinf(x) || isinf(y) )
        {
                return 1.0 / 0.0; /* Return infinity */
        }

        if ( isnan(x) || isnan(y) )
        {
                return 0.0 / 0.0; /* Return NaN */
        }

        /* Use the formula: sqrt(x^2 + y^2) */
        return sqrt(x * x + y * y);
}
