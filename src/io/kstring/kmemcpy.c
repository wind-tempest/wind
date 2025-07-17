/* memcpy.c */

/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "kmemcpy.h"
#include "kstddef.h"

void *
    kmemcpy (void *dest, const void *src, size_t n)
{
        unsigned char       *d = dest;
        const unsigned char *s = src;
        for ( size_t i = 0; i < n; ++i )
                d[i] = s[i];
        return dest;
}
