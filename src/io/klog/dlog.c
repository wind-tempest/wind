/* dlog.c */

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

#include "core/kernel.h"
#include "dlog.h"
#include "drivers/serial/serial.h"
#include "kprint.h"
#include "kstdarg.h"
#include "kutoa.h"

void
    kduts (const char *s)
{
	if ( !use_debug )
	{
		return;
	}
	serial_writes(s);
	serial_write('\n');
}

int
    kdebugf (const char *format, ...)
{
	if ( !use_debug )
		return 0;

	va_list args;
	va_start(args, format);
	int count = 0;

	for ( const char *p = format; *p; ++p )
	{
		if ( *p != '%' )
		{
			serial_write(*p);
			count++;
			continue;
		}

		p++;

		int left_align = 0;
		int width      = 0;

		if ( *p == '-' )
		{
			left_align = 1;
			p++;
		}

		while ( *p >= '0' && *p <= '9' )
		{
			width = width * 10 + (*p - '0');
			p++;
		}

		switch ( *p )
		{
			case 's':
			{
				const char *s	= va_arg(args, const char *);
				int	    len = 0;
				const char *t	= s;
				while ( *t++ )
					len++;

				int pad = (width > len) ? (width - len) : 0;

				if ( !left_align )
				{
					for ( int i = 0; i < pad; ++i )
					{
						serial_write(' ');
						count++;
					}
				}

				serial_writes(s);
				count += len;

				if ( left_align )
				{
					for ( int i = 0; i < pad; ++i )
					{
						serial_write(' ');
						count++;
					}
				}
				break;
			}

			case 'd':
			{
				int   n = va_arg(args, int);
				char  buf[12];
				char *ptr = buf;

				if ( n < 0 )
				{
					*ptr++ = '-';
					n      = -n;
				}

				char *end_ptr =
				    kutoa(ptr, buf + sizeof(buf) - 1, (unsigned int) n, 10, 0);
				*end_ptr = '\0';
				serial_writes(buf);
				count += (int) (end_ptr - buf);
				break;
			}

			case 'x':
			{
				unsigned int n = va_arg(args, unsigned int);
				char	     buf[12];
				char	    *end_ptr = kutoa(buf, buf + sizeof(buf) - 1, n, 16, 0);
				*end_ptr	     = '\0';
				serial_writes(buf);
				count += (int) (end_ptr - buf);
				break;
			}
			case 'l':
			{
				/* Handle long/long long modifiers */
				if ( *(p + 1) == 'l' )
				{
					p++; /* Skip second 'l' */
					if ( *(p + 1) == 'x' )
					{
						p++; /* Skip 'x' */
						uint64_t n = va_arg(args, uint64_t);
						char	 buf[20];
						char	*end_ptr =
						    kutoa(buf,
							  buf + sizeof(buf) - 1,
							  (unsigned int) (n & 0xFFFFFFFF),
							  16,
							  0);
						*end_ptr = '\0';
						serial_writes(buf);
						count += (int) (end_ptr - buf);
						break;
					}
				}
				/* Fall through to default for single 'l' */
				goto default_case;
			}

			case 'c':
			{
				char c = (char) va_arg(args, int);
				serial_write(c);
				count++;
				break;
			}

			case '%':
			{
				serial_write('%');
				count++;
				break;
			}

			default:
			default_case:
			{
				serial_write('%');
				serial_write(*p);
				count += 2;
				break;
			}
		}
	}

	va_end(args);
	return count;
}

void
    kcritical (const char *message, const char *extra)
{
	if ( extra == NULL )
	{
		kprintf("[CRITICAL] %s\n", message);
	}
	else
	{
		kprintf("[CRITICAL] %s: %s\n", message, extra);
	}
}

void
    kalert (const char *message, const char *extra)
{
	if ( extra == NULL )
	{
		kprintf("[ALERT] %s\n", message);
	}
	else
	{
		kprintf("[ALERT] %s: %s\n", message, extra);
	}
}

void
    kemerg (const char *message, const char *extra)
{
	if ( extra == NULL )
	{
		kprintf("[EMERGENCY] %s\n", message);
	}
	else
	{
		kprintf("[EMERGENCY] %s: %s\n", message, extra);
	}
}

void
    kwarn (const char *message, const char *extra)
{
	if ( extra == NULL )
	{
		kprintf("[WARN] %s\n", message);
	}
	else
	{
		kprintf("[WARN] %s: %s\n", message, extra);
	}
}

void
    kerror (const char *message, const char *extra)
{
	if ( extra == NULL )
	{
		kprintf("[ERROR] %s\n", message);
	}
	else
	{
		kprintf("[ERROR] %s: %s\n", message, extra);
	}
}

void
    knotice (const char *message, const char *extra)
{
	if ( extra == NULL )
	{
		kprintf("[NOTICE] %s\n", message);
	}
	else
	{
		kprintf("[NOTICE] %s: %s\n", message, extra);
	}
}

void
    kinfo (const char *message, const char *extra)
{
	if ( extra == NULL )
	{
		kprintf("[INFO] %s\n", message);
	}
	else
	{
		kprintf("[INFO] %s: %s\n", message, extra);
	}
}
