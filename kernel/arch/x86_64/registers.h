// registers.h

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
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
