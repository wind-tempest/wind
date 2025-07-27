// idt.h

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#pragma once

#include "registers.h"

void
    idt_init (void);

typedef void (*irq_handler_t)(registers_t *);
void
    register_irq_handler (int irq, irq_handler_t handler);
