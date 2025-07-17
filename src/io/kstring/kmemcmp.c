/* memcmp.c */

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

#include "kmemcmp.h"

int
    kmemcmp (const void *s1, const void *s2, unsigned long n)
{
        const unsigned char *a = (const unsigned char *) s1;
        const unsigned char *b = (const unsigned char *) s2;
        for ( unsigned long i = 0; i < n; ++i )
        {
                if ( a[i] != b[i] )
                        return (int) a[i] - (int) b[i];
        }
        return 0;
}
