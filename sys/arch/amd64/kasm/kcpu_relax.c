// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "arch/amd64/kasm/kcpu_relax.h"

void
    kcpu_relax (void) {
	__asm__ volatile("pause");
}
