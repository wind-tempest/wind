/* ata.h */

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

/* ata.h - Minimal ATA PIO driver (read-only, 28-bit LBA)
 *
 * Provides a single public function that implements the read sector callback
 * expected by the EXT2 driver:
 *      int ata_pio_read(uint64_t lba, uint32_t count, void *buf);
 *
 * The driver accesses the primary IDE channel in PIO mode (I/O ports 0x1F0
 * and 0x3F6) and assumes a master drive. Only 28-bit LBA addressing is
 * supported, which is enough for our test disk image.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

        /* Read @count sectors (512 bytes each) starting at @lba into @buf. Returns 0
         * on success, non-zero on failure. */
        int ata_pio_read (uint64_t lba, uint32_t count, void *buf);

#ifdef __cplusplus
}
#endif
