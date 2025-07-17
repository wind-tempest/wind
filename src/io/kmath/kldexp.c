/* ldexp.c */

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

#include "kldexp.h"
#include "kmath.h"

double
    kldexp (double x, int exp)
{
        /* Handle special cases */
        if ( kisnan(x) )
                return knan("");
        if ( kisinf(x) )
                return x;
        if ( x == 0.0 )
                return 0.0;

        /* Use the relationship: ldexp(x, exp) = x * 2^exp */
        return x * kpow(2.0, exp);
}
