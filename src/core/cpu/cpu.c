// cpu.c

// SPDX-License-Identifier: LSL-1.0
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.0 (LSL-1.0).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include "core/entry/kernel.h"
#include "kstddef.h"
#include "kstdint.h"

char cpu_brand_string[49] = "Unknown CPU";

static inline void
    cpuid (kuint32_t eax, kuint32_t ecx, kuint32_t *regs) {
	__asm__ volatile("cpuid"
			 : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
			 : "a"(eax), "c"(ecx));
}

void
    kcpu_init_brand (void) {
	kuint32_t regs[4];
	char	 *brand = cpu_brand_string;
	for ( kuint32_t i = 0; i < 3; ++i ) {
		cpuid(0x80000002 + i, 0, regs);
		for ( int j = 0; j < 4; ++j ) {
			*(kuint32_t *) (brand + i * 16 + j * 4) = regs[j];
		}
	}
	cpu_brand_string[48] = '\0';
}
