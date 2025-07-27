// registers.h

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

#include <wt/kstdio/kstdint.h>

// This struct defines the registers that our ISR/IRQ stubs push to the stack.
// We receive a pointer to this in our C-level interrupt handlers.
typedef struct {
	kuint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	kuint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	kuint64_t int_no, err_code; // These are pushed first by the ISR.
} registers_t;
