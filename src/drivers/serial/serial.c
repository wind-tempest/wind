/* serial.c */

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

#include "kitoa.h"
#include "ksignal.h"
#include "kstdbool.h"
#include "serial.h"

#define PORT 0x3f8 // COM1
bool is_serial_available;

bool
    serial_available (void);

void
    serial_init (void)
{
        /* First check if serial port is available. */
        is_serial_available = serial_available();

        if ( !is_serial_available )
        {
                return; // Exit if no serial port detected.
        }

        koutb(PORT + 1, 0x00); // Disable all interrupts.
        koutb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor).
        koutb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
        koutb(PORT + 1, 0x00); //                  (hi byte)
        koutb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit.
        koutb(PORT + 2,
              0xC7);           // Enable FIFO, clear them, with 14-byte threshold.
        koutb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set.
}

static int
    is_transmit_empty ()
{
        return kinb(PORT + 5) & 0x20;
}

void
    serial_write (char a)
{
        if ( !is_serial_available || !a )
                return; // Exit if serial not available or character is null.
        while ( is_transmit_empty() == 0 )
                ;
        outb(PORT, (unsigned char) a);
}

void
    serial_writes (const char *s)
{
        if ( !is_serial_available || !s )
                return; // Exit if serial not available or string is null.
        while ( *s )
        {
                serial_write(*s++);
        }
}

void
    serial_write_int (int i)
{
        if ( !is_serial_available )
                return; // Exit if serial not available.
        char buf[16];
        kitoa(buf, buf + 15, i, 10, 0);
        serial_writes(buf);
}

bool
    serial_available (void)
{
        /* Save original values. */
        unsigned char original_lcr = kinb(PORT + 3);
        unsigned char original_mcr = kinb(PORT + 4);

        /* Set up loopback mode. */
        koutb(PORT + 3, 0x80); // Enable DLAB.
        koutb(PORT + 4, 0x10); // Enable loopback mode.

        /* Test value to write and read back. */
        unsigned char test_value = 0xAA;

        /* Write test value. */
        koutb(PORT, test_value);

        /* Small delay to ensure data is processed. */
        for ( volatile int i = 0; i < 1000; i++ )
        {
                /* Yes. */
        }

        /* Read back the value. */
        unsigned char read_value = kinb(PORT);

        /* Restore original values. */
        koutb(PORT + 3, original_lcr);
        koutb(PORT + 4, original_mcr);

        /* If read value matches test value, port is available. */
        return (read_value == test_value);
}
