// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "arch/amd64/kasm/koutw.h"

#include "arch/amd64/kasm/kio.h"

void
    koutw (unsigned short port, unsigned short val) {
	__asm__ __volatile__("outw %0, %1" : : "a"(val), "Nd"(port));
}
