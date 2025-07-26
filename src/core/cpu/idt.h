// idt.h

// SPDX-License-Identifier: LSL-1.1
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.1 (LSL-1.1).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#pragma once

#include "registers.h"

void
    idt_init (void);

typedef void (*irq_handler_t)(registers_t *);
void
    register_irq_handler (int irq, irq_handler_t handler);
