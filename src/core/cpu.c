/* cpu.c */

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

#include "kernel.h"
#include <stddef.h>
#include <stdint.h>

char cpu_brand_string[49] = "Unknown CPU";

static inline void
    cpuid (uint32_t eax, uint32_t ecx, uint32_t *regs)
{
	__asm__ volatile("cpuid"
			 : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
			 : "a"(eax), "c"(ecx));
}

void
    cpu_init_brand (void)
{
	uint32_t regs[4];
	char	*brand = cpu_brand_string;
	for ( uint32_t i = 0; i < 3; ++i )
	{
		cpuid(0x80000002 + i, 0, regs);
		for ( int j = 0; j < 4; ++j )
		{
			*(uint32_t *) (brand + i * 16 + j * 4) = regs[j];
		}
	}
	cpu_brand_string[48] = '\0';
}
