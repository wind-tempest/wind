/* kmemset.c */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "kstddef.h"
#include "kstdint.h"

void *
    memset (void *s, int c, size_t n)
{
	if ( !s )
		return NULL; // null-check for safety

	unsigned char *byte_ptr = (unsigned char *) s;
	size_t	       i	= 0;

	/* Prepare the word-sized pattern */
	uintptr_t pattern = (unsigned char) c;
	pattern |= pattern << 8;
	pattern |= pattern << 16;
#if UINTPTR_MAX > 0xffffffff
	pattern |= pattern << 32;
#endif

	/* Align to word boundary */
	size_t	  align	   = sizeof(uintptr_t);
	uintptr_t addr	   = (uintptr_t) byte_ptr;
	size_t	  misalign = addr % align;
	size_t	  to_align = misalign ? (align - misalign) : 0;

	/* Fill byte-by-byte until aligned */
	for ( ; i < n && to_align--; ++i )
	{
		byte_ptr[i] = (unsigned char) c;
	}

	/* Fill word-by-word */
	size_t	   words    = (n - i) / align;
	uintptr_t *word_ptr = (uintptr_t *) (byte_ptr + i);
	for ( size_t w = 0; w < words; ++w )
	{
		word_ptr[w] = pattern;
	}

	/* Advance index */
	i += words * align;

	/* Fill remaining bytes */
	for ( ; i < n; ++i )
	{
		byte_ptr[i] = (unsigned char) c;
	}

	return s;
}
