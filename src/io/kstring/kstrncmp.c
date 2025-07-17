/* strncmp.c */

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

#include "kstrncmp.h"
#include "stddef.h"

int
    kstrncmp (const char *s1, const char *s2, size_t n)
{
        for ( size_t i = 0; i < n; i++ )
        {
                if ( s1[i] != s2[i] )
                {
                        return (unsigned char) s1[i] - (unsigned char) s2[i];
                }
                if ( s1[i] == '\0' )
                {
                        return 0;
                }
        }
        return 0;
}
