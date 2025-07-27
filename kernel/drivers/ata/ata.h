// ata.h

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

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Read @count sectors (512 bytes each) starting at @lba into @buf. Returns 0
 * on success, non-zero on failure.
 */
int
    ata_pio_read (kuint64_t lba, kuint32_t count, void *buf);

#ifdef __cplusplus
}
#endif
