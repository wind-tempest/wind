/* panic.h */

/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "core/registers.h"

/* Panic error codes */
#define PANIC_UNKNOWN_ERROR       0
#define PANIC_DIVISION_BY_ZERO    1
#define PANIC_DOUBLE_FAULT        2
#define PANIC_GENERAL_PROTECTION  3
#define PANIC_PAGE_FAULT          4
#define PANIC_STACK_SEGMENT       5
#define PANIC_SEGMENT_NOT_PRESENT 6
#define PANIC_INVALID_TSS         7
#define PANIC_ALIGNMENT_CHECK     8
#define PANIC_MACHINE_CHECK       9
#define PANIC_SIMD_EXCEPTION      10
#define PANIC_VIRTUALIZATION      11
#define PANIC_CONTROL_PROTECTION  12
#define PANIC_HYPERVISOR          13
#define PANIC_VMM_COMMUNICATION   14
#define PANIC_SECURITY            15
#define PANIC_INVALID_OPCODE      16

/* Panic function - halts the system with error information */
void
    panic (int code, registers_t *regs);
