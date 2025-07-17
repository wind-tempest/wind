/* print.h */

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

#pragma once

#include "kstdarg.h"
#include "kstddef.h"
#include "kstdint.h"

void
    kputhex (uint64_t n);
void
    kputdec (uint32_t n);
void
    kputs (const char *s);
int
    kvsnprintf (char *buffer, size_t size, const char *format, va_list args);
int
    ksnprintf (char *buffer, size_t size, const char *format, ...);
int
    kprintf (const char *format, ...);
void
    kputchar (int c);
