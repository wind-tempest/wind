/* reboot.c */

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

#include "kprint.h"
#include "ksignal.h"
#include "reboot.h"

void
    kreboot (void)
{
	/* Wait until the keyboard controller is ready. */
	while ( kinb(0x64) & 0x02 )
		;
	/* Send the reset command */
	koutb(0x64, 0xFE);

	/* If it fails, just warn the user. */
	kputs("Failed! Keyboard controller didn't respond to reset.");
}
