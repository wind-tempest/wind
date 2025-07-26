// kmemset.c

// SPDX-License-Identifier: LSL-1.1
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.1 (LSL-1.1).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include "kstddef.h"
#include "kstdint.h"

void *
    kmemset (void *s, int c, ksize_t n) {
	if ( !s )
		return KNULL; // null-check for safety

	unsigned char *byte_ptr = (unsigned char *) s;
	ksize_t	       i	= 0;

	// Prepare the word-sized pattern
	kuintptr_t pattern = (unsigned char) c;
	pattern |= pattern << 8;
	pattern |= pattern << 16;
#if KUINTPTR_MAX > 0xffffffff
	pattern |= pattern << 32;
#endif

	// Align to word boundary
	ksize_t	   align    = sizeof(kuintptr_t);
	kuintptr_t addr	    = (kuintptr_t) byte_ptr;
	ksize_t	   misalign = addr % align;
	ksize_t	   to_align = misalign ? (align - misalign) : 0;

	// Fill byte-by-byte until aligned
	for ( ; i < n && to_align--; ++i ) {
		byte_ptr[i] = (unsigned char) c;
	}

	// Fill word-by-word
	ksize_t	    words    = (n - i) / align;
	kuintptr_t *word_ptr = (kuintptr_t *) (byte_ptr + i);
	for ( ksize_t w = 0; w < words; ++w ) {
		word_ptr[w] = pattern;
	}

	// Advance index
	i += words * align;

	// Fill remaining bytes
	for ( ; i < n; ++i ) {
		byte_ptr[i] = (unsigned char) c;
	}

	return s;
}
