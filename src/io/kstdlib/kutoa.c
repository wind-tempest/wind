/* utoa.c */

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

#include "kcharset.h"
#include "kutoa.h"

char *
    kutoa (char *buf, char *end, unsigned long value, int base, int is_uppercase)
{
        if ( base <= 1 )
        {
                if ( buf < end )
                        *buf = '\0';
                return buf;
        }
        const char *digit_set = is_uppercase ? kdigits_upper : kdigits_lower;
        char       *rev       = buf;

        if ( value == 0 )
        {
                if ( buf < end )
                {
                        *rev++ = '0';
                }
                return rev;
        }

        while ( value != 0 && rev < end )
        {
                *rev++ = digit_set[(unsigned) (value % (unsigned) base)];
                value /= (unsigned) base;
        }

        /* Reverse string */
        for ( char *p1 = buf, *p2 = rev - 1; p1 < p2; p1++, p2-- )
        {
                char tmp = *p1;
                *p1      = *p2;
                *p2      = tmp;
        }

        return rev;
}
