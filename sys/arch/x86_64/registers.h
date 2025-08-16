// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <tempest/klibc/kstdio/kstdint.h>

// This struct defines the registers that our ISR/IRQ stubs push to the stack.
// We receive a pointer to this in our C-level interrupt handlers.
typedef struct {
	kuint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	kuint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	kuint64_t int_no, err_code;  // These are pushed first by the ISR.
} registers_t;
