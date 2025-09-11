// SPDX-License-Identifier: LSL-2.0
/*
 * -- BEGIN LICENSE HEADER --
 * The Wind/Tempest Project
 *
 * File:        sys/drivers/ata/ata.h
 * Author(s):   Russian95 <russian95@tempestfoundation.org>
 *              (https://github.com/Russian95CrE)
 * Maintainer:  Tempest Foundation <development@tempestfoundation.org>
 * Link:        https://wtsrc.tempestfoundation.org
 *
 * Copyright (C) 2025 Tempest Foundation
 * Licensed under the Liberty Software License, Version 2.0
 * -- END OF LICENSE HEADER --
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
