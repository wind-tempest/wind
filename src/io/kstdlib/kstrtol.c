/* strtol.c */

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

#include "kctype.h"
#include "kerrno.h"
#include "klimits.h"
#include "kstrtol.h"

long
    kstrtol (const char *nptr, char **endptr, int base)
{
        const char *s   = nptr;
        long        acc = 0;
        int         c;
        int         neg = 0;
        long        cutoff;
        int         cutlim;

        /* skip whitespace */
        while ( kisspace((unsigned char) *s) )
                s++;

        /* sign */
        if ( *s == '-' )
        {
                neg = 1;
                s++;
        }
        else if ( *s == '+' )
        {
                s++;
        }

        /* detect base if 0 */
        if ( base == 0 )
        {
                if ( *s == '0' )
                {
                        if ( s[1] == 'x' || s[1] == 'X' )
                        {
                                base = 16;
                                s += 2;
                        }
                        else
                        {
                                base = 8;
                                s++;
                        }
                }
                else
                {
                        base = 10;
                }
        }
        else if ( base == 16 )
        {
                if ( s[0] == '0' && (s[1] == 'x' || s[1] == 'X') )
                {
                        s += 2;
                }
        }

        /* compute cutoff for overflow */
        cutoff = neg ? LONG_MIN : LONG_MAX;
        cutlim = (int) (cutoff % base);
        cutoff /= base;
        if ( cutlim < 0 )
        {
                cutlim += base;
                cutoff += 1;
        }

        /* convert digits */
        for ( ;; s++ )
        {
                c = (unsigned char) *s;
                if ( kisdigit(c) )
                        c -= '0';
                else if ( kisalpha(c) )
                        c = ktoupper(c) - 'A' + 10;
                else
                        break;
                if ( c >= base )
                        break;

                /* check overflow */
                if ( neg )
                {
                        if ( acc < cutoff || (acc == cutoff && c > cutlim) )
                        {
                                acc   = LONG_MIN;
                                errno = ERANGE;
                                neg   = 0; /* to avoid negating again */
                        }
                        else
                        {
                                acc *= base;
                                acc -= c;
                        }
                }
                else
                {
                        if ( acc > cutoff || (acc == cutoff && c > cutlim) )
                        {
                                acc   = LONG_MAX;
                                errno = ERANGE;
                                neg   = 0; /* to avoid negating again */
                        }
                        else
                        {
                                acc *= base;
                                acc += c;
                        }
                }
        }

        if ( endptr )
                *endptr = (char *) (acc == 0 && s == nptr ? nptr : s);

        return acc;
}
