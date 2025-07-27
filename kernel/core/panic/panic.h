// kpanic.h

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#pragma once

#include "arch/x86_64/registers.h"

// Panic error codes
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

// Panic function - halts the system with error information
void
    kpanic (int code, registers_t *regs);
