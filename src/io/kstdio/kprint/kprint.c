/* kprint.c */

/*
 * ---------------------------------------------------------------------------
 *  Wind (operating system) / Tempest (kernel)
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ---------------------------------------------------------------------------
 *  This file is part of the Wind/Tempest project, an open-source operating system
 *  and kernel designed to deliver performance, stability, and freedom to users.
 *
 *  - License:
 *  This software is licensed under the GNU Affero General Public License (AGPL)
 *  version 3 or any later version. You may redistribute and/or modify it under
 *  the terms of the AGPL as published by the Free Software Foundation.
 *
 *  - Warranty Disclaimer:
 *  Wind/Tempest is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 *  details.
 *
 *  - Permissions:
 *  > You may modify this file and include your own copyright notices below.
 *  > Forks and contributions are welcome, as long as this license is respected.
 *
 *  - Additional Information:
 *  For more information about the project, visit:
 *  https://wind.infernointeractive.win
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 * ---------------------------------------------------------------------------
 */

#include "drivers/driver.h"
#include "kprint.h"
#include "kstdarg.h"
#include "kstdio.h"
#include "kutoa.h"

/* I think this is... safe? */
void
    kputhex (uint64_t n)
{
	const char *hex = "0123456789ABCDEF";
	char	    buf[17]; // 16 hex digits + null terminator
	buf[16] = '\0';

	for ( int i = 15; i >= 0; --i )
	{
		buf[i] = hex[n & 0xF];
		n >>= 4;
	}

	/* Skip leading zeros */
	int start = 0;
	while ( start < 15 && buf[start] == '0' )
	{
		start++;
	}

	kputs(&buf[start]);
}

void
    kputdec (uint32_t n)
{
	if ( n == 0 )
	{
		kputchar('0');
		return;
	}

	char buf[11];
	int  i = 0;

	while ( n > 0 )
	{
		buf[i++] = (char) ('0' + (n % 10));
		n /= 10;
	}

	while ( --i >= 0 )
	{
		kputchar(buf[i]);
	}
}

void
    kputs (const char *s)
{
	video_puts(s);
	kputchar('\n');
}

int
    kvsnprintf (char *buffer, size_t size, const char *format, va_list args)
{
	char *out = buffer;
	char *end = buffer + size - 1;

	for ( const char *p = format; *p && out < end; ++p )
	{
		if ( *p != '%' )
		{
			*out++ = *p;
			continue;
		}
		++p;

		int left_align = 0;
		if ( *p == '-' )
		{
			left_align = 1;
			++p;
		}
		int width = 0;
		while ( *p >= '0' && *p <= '9' )
		{
			width = width * 10 + (*p++ - '0');
		}

		char temp[32];
		/* Handle length modifier for long long ("ll") */
		int long_long = 0;
		if ( *p == 'l' && *(p + 1) == 'l' )
		{
			long_long = 1;
			p += 2; /* Skip the two 'l's */
		}

		char *t = temp;

		/* Use long_long flag to choose value width */

		switch ( *p )
		{
			case 's':
			{
				const char *s = k_va_arg(args, const char *);
				while ( *s && t < temp + sizeof(temp) - 1 )
				{
					*t++ = *s++;
				}
				break;
			}
			case 'c':
			{
				*t++ = (char) k_va_arg(args, int);
				break;
			}
			case 'd':
			{
				if ( long_long )
				{
					long long	   val = k_va_arg(args, long long);
					unsigned long long uval;
					if ( val < 0 )
					{
						*t++ = '-';
						uval = (unsigned long long) (-val);
					}
					else
					{
						uval = (unsigned long long) val;
					}
					t = kutoa(t,
						  temp + sizeof(temp) - 1,
						  (unsigned long) uval,
						  10,
						  0);
				}
				else
				{
					int	     val = k_va_arg(args, int);
					unsigned int uval;
					if ( val < 0 )
					{
						*t++ = '-';
						uval = (unsigned int) (-val);
					}
					else
					{
						uval = (unsigned int) val;
					}
					t = kutoa(t, temp + sizeof(temp) - 1, uval, 10, 0);
				}
				break;
			}
			case 'u':
			{
				if ( long_long )
				{
					unsigned long long uval =
					    k_va_arg(args, unsigned long long);
					t = kutoa(t,
						  temp + sizeof(temp) - 1,
						  (unsigned long) uval,
						  10,
						  0);
				}
				else
				{
					unsigned int uval = k_va_arg(args, unsigned int);
					t = kutoa(t, temp + sizeof(temp) - 1, uval, 10, 0);
				}
				break;
			}
			case 'x':
			{
				unsigned int uval = k_va_arg(args, unsigned int);
				t		  = kutoa(t, temp + sizeof(temp) - 1, uval, 16, 0);
				break;
			}
			case 'X':
			{
				unsigned int uval = k_va_arg(args, unsigned int);
				t		  = kutoa(t, temp + sizeof(temp) - 1, uval, 16, 1);
				break;
			}
			case '%':
			{
				*t++ = '%';
				break;
			}
			default:
			{
				*t++ = '%';
				*t++ = *p;
				break;
			}
		}
		++p;

		size_t len = (size_t) (t - temp);
		int    pad = width > (int) len ? width - (int) len : 0;
		if ( !left_align )
		{
			while ( pad-- > 0 && out < end )
				*out++ = ' ';
		}
		for ( size_t i = 0; i < len && out < end; ++i )
		{
			*out++ = temp[i];
		}
		if ( left_align )
		{
			while ( pad-- > 0 && out < end )
				*out++ = ' ';
		}
		--p;
	}

	*out = '\0';
	return (int) (out - buffer);
}

int
    ksnprintf (char *buffer, size_t size, const char *format, ...)
{
	va_list args;
	k_va_start(args, format);
	int result = kvsnprintf(buffer, size, format, args);
	k_va_end(args);
	return result;
}

int
    kprintf (const char *format, ...)
{
	va_list args;
	k_va_start(args, format);
	int count = 0;

	for ( const char *p = format; *p; ++p )
	{
		if ( *p != '%' )
		{
			kputchar(*p);
			count++;
			continue;
		}

		p++;

		/* Field width and left alignment */
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
				const char *s	= k_va_arg(args, const char *);
				int	    len = 0;
				const char *t	= s;
				while ( *t++ )
					len++;

				int pad = (width > len) ? (width - len) : 0;

				if ( !left_align )
				{
					for ( int i = 0; i < pad; ++i )
					{
						kputchar(' ');
						count++;
					}
				}

				for ( int i = 0; i < len; ++i )
				{
					kputchar(s[i]);
					count++;
				}

				if ( left_align )
				{
					for ( int i = 0; i < pad; ++i )
					{
						kputchar(' ');
						count++;
					}
				}
				break;
			}

			case 'd':
			{
				int n = k_va_arg(args, int);
				if ( n < 0 )
				{
					kputchar('-');
					count++;
					n = -n;
				}

				kputdec((uint32_t) n);

				int temp = n, digits = 1;
				while ( temp >= 10 )
				{
					temp /= 10;
					digits++;
				}
				count += digits;
				break;
			}

			case 'x':
			{
				unsigned int n = k_va_arg(args, unsigned int);
				kputhex((uint64_t) n);

				unsigned int temp   = n;
				int	     digits = 1;
				while ( temp >= 16 )
				{
					temp /= 16;
					digits++;
				}
				count += digits;
				break;
			}

			case 'c':
			{
				char c = (char) k_va_arg(args, int);
				kputchar(c);
				count++;
				break;
			}

			case '%':
			{
				kputchar('%');
				count++;
				break;
			}

			case 'u':
			{
				unsigned int n = k_va_arg(args, unsigned int);
				kputdec(n);

				unsigned int temp   = n;
				int	     digits = 1;
				while ( temp >= 10 )
				{
					temp /= 10;
					digits++;
				}
				count += digits;
				break;
			}

			default:
				kputchar('%');
				kputchar(*p);
				count += 2;
				break;
		}
	}

	k_va_end(args);
	return count;
}

void
    kputchar (int c)
{
	video_putchar((char) c);
}
