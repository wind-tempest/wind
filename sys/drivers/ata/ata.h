// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <lib/kstdio/kstdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern struct Ata ata;

struct Ata {
	void (*io_wait)(void);
	int (*poll)(void);
	int (*pio_read)(kuint64_t lba, kuint32_t count, void *buf);
};

#ifdef __cplusplus
}
#endif
