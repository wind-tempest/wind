// ata.h

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
