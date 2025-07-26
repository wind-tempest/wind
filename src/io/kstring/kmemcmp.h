// kmemcmp.h

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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int
    kmemcmp (const void *s1, const void *s2, unsigned long n);

#ifdef __cplusplus
}
#endif
