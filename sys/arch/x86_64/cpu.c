// cpu.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "cpu.h"

#include "init/kernel.h"

#include <wt/kstdio/kstddef.h>
#include <wt/kstdio/kstdint.h>
#include <wt/kstring/kmemcpy.h>

#define CPUID_EXT_BASE 0x80000002

char cpu_brand_string[CPU_BRAND_STRING_LEN + 1] = "Unknown CPU";

static inline void
    cpuid (kuint32_t eax, kuint32_t ecx, kuint32_t *restrict regs) {
	__asm__ volatile("cpuid"
			 : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
			 : "a"(eax), "c"(ecx));
}

void
    kcpu_init_brand (void) {
	kuint32_t regs[4];
	cpuid(0x80000000, 0, regs);
	if ( regs[0] < 0x80000004 )
		return;
	char *brand = cpu_brand_string;
	for ( kuint32_t i = 0; i < CPU_BRAND_STRING_LEN / 16; ++i ) {
		cpuid(CPUID_EXT_BASE + i, 0, regs);
		for ( int j = 0; j < 4; ++j ) {
			kmemcpy(brand + i * 16 + j * 4, &regs[j], sizeof(kuint32_t));
		}
	}

	cpu_brand_string[CPU_BRAND_STRING_LEN] = '\0';
}
