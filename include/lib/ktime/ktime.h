// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <lib/kstdio/kstddef.h>
#include <lib/kstdio/kstdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// BIOS time structure
struct bios_time {
	kuint8_t  second;
	kuint8_t  minute;
	kuint8_t  hour;
	kuint8_t  day;
	kuint8_t  month;
	kuint16_t year;
	kuint8_t  day_of_week;
};

void
    kget_bios_time (struct bios_time *time);
void
    kget_date_string (char *buffer, ksize_t buffer_size);
void
    kget_time_string (char *buffer, ksize_t buffer_size);
void
    kget_datetime_string (char *buffer, ksize_t buffer_size);
const char *
    kget_day_of_week_string (kuint8_t day_of_week);

#ifdef __cplusplus
}
#endif
