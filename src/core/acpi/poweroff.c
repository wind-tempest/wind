/* poweroff.c */

/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "poweroff.h"
#include "print.h"
#include "signal.h"

void
    poweroff (void)
{
        /* ACPI poweroff (works in QEMU, VirtualBox, and some real hardware) */
        outw(0x604, 0x2000);
        /* Fallback for Bochs/QEMU */
        outw(0xB004, 0x2000);
        /* Infinite loop if poweroff fails */
        puts("Failed! ACPI didn't respond to poweroff.");
}
