// kstrnlen.c

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

#include "kstrnlen.h"

#include "kstddef.h"

ksize_t
    kstrnlen (const char *str, ksize_t maxlen) {
	ksize_t len = 0;
	while ( len < maxlen && str[len] != '\0' ) {
		len++;
	}
	return len;
}
