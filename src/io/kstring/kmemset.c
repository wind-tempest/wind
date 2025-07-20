/* kmemset.c */

/*
 * ---------------------------------------------------------------------------
 *  Wind (operating system) / Tempest (kernel)
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ---------------------------------------------------------------------------
 *  This file is part of the Wind/Tempest project, an open-source operating system
 *  and kernel designed to deliver performance, stability, and freedom to users.
 *
 *  - License:
 *  This software is licensed under the GNU Affero General Public License (AGPL)
 *  version 3 or any later version. You may redistribute and/or modify it under
 *  the terms of the AGPL as published by the Free Software Foundation.
 *
 *  - Warranty Disclaimer:
 *  Wind/Tempest is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 *  details.
 *
 *  - Permissions:
 *  > You may modify this file and include your own copyright notices below.
 *  > Forks and contributions are welcome, as long as this license is respected.
 *
 *  - Additional Information:
 *  For more information about the project, visit:
 *  https://wind.infernointeractive.win
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 * ---------------------------------------------------------------------------
 */

#include "kstddef.h"
#include "kstdint.h"

void *
    memset (void *s, int c, ksize_t n) {
	if ( !s )
		return KNULL; // null-check for safety

	unsigned char *byte_ptr = (unsigned char *) s;
	ksize_t	       i	= 0;

	/* Prepare the word-sized pattern */
	kuintptr_t pattern = (unsigned char) c;
	pattern |= pattern << 8;
	pattern |= pattern << 16;
#if KUINTPTR_MAX > 0xffffffff
	pattern |= pattern << 32;
#endif

	/* Align to word boundary */
	ksize_t	   align    = sizeof(kuintptr_t);
	kuintptr_t addr	    = (kuintptr_t) byte_ptr;
	ksize_t	   misalign = addr % align;
	ksize_t	   to_align = misalign ? (align - misalign) : 0;

	/* Fill byte-by-byte until aligned */
	for ( ; i < n && to_align--; ++i ) {
		byte_ptr[i] = (unsigned char) c;
	}

	/* Fill word-by-word */
	ksize_t	    words    = (n - i) / align;
	kuintptr_t *word_ptr = (kuintptr_t *) (byte_ptr + i);
	for ( ksize_t w = 0; w < words; ++w ) {
		word_ptr[w] = pattern;
	}

	/* Advance index */
	i += words * align;

	/* Fill remaining bytes */
	for ( ; i < n; ++i ) {
		byte_ptr[i] = (unsigned char) c;
	}

	return s;
}
