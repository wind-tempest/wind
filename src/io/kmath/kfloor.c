/* floor.c */

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

#include "kfloor.h"
#include "kmath.h"

double
    kfloor (double x)
{
        if ( x >= 0.0 )
        {
                /* For positive numbers, truncate */
                return (double) ((long long) x);
        }
        else
        {
                /* For negative numbers, round down */
                long long int_part = (long long) x;
                if ( x == (double) int_part )
                {
                        return x;
                }
                return (double) (int_part - 1);
        }
}
