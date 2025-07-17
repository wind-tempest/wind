/* ktime.h */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "kstddef.h"
#include "kstdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/* BIOS time structure */
	struct bios_time
	{
		uint8_t	 second;
		uint8_t	 minute;
		uint8_t	 hour;
		uint8_t	 day;
		uint8_t	 month;
		uint16_t year;
		uint8_t	 day_of_week;
	};

	/* Function declarations */
	void	    get_bios_time (struct bios_time *time);
	void	    get_date_string (char *buffer, size_t buffer_size);
	void	    get_time_string (char *buffer, size_t buffer_size);
	void	    get_datetime_string (char *buffer, size_t buffer_size);
	const char *get_day_of_week_string (uint8_t day_of_week);
	void	    print_current_datetime (void);

#ifdef __cplusplus
}
#endif
