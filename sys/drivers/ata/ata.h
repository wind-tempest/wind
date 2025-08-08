// ata.h

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
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
