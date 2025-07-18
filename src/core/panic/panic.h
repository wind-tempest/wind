/* panic.h */

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

#pragma once

#include "core/registers.h"

/* Panic error codes */
#define PANIC_UNKNOWN_ERROR	  0
#define PANIC_DIVISION_BY_ZERO	  1
#define PANIC_DOUBLE_FAULT	  2
#define PANIC_GENERAL_PROTECTION  3
#define PANIC_PAGE_FAULT	  4
#define PANIC_STACK_SEGMENT	  5
#define PANIC_SEGMENT_NOT_PRESENT 6
#define PANIC_INVALID_TSS	  7
#define PANIC_ALIGNMENT_CHECK	  8
#define PANIC_MACHINE_CHECK	  9
#define PANIC_SIMD_EXCEPTION	  10
#define PANIC_VIRTUALIZATION	  11
#define PANIC_CONTROL_PROTECTION  12
#define PANIC_HYPERVISOR	  13
#define PANIC_VMM_COMMUNICATION	  14
#define PANIC_SECURITY		  15
#define PANIC_INVALID_OPCODE	  16

/* Panic function - halts the system with error information */
void
    panic (int code, registers_t *regs);
