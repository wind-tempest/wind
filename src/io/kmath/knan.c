/* nan.c */

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
#include "nan.h"

double
    nan (const char *tagp)
{
        /* Return NaN - the tagp parameter is ignored in this implementation */
        (void) tagp; /* Suppress unused parameter warning */

        /* Return a quiet NaN */
        union
        {
                double             d;
                unsigned long long u;
        } u;

        u.u = 0x7ff8000000000000ULL; /* Quiet NaN */
        return u.d;
}
