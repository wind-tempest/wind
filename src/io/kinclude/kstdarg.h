/* stdarg.h */

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

/*
 * stdarg.h - Variable arguments handling (C99 standard)
 * This header provides macros for functions with variable argument lists.
 * Supports GCC/Clang builtins on supported architectures (e.g., x86_64).
 */

#ifdef __cplusplus
extern "C"
{
#endif

/*----------------------------------------------------------------------------
 * Architecture-specific va_list definition
 * For compilers supporting built-in va_list (GCC, Clang)
 *----------------------------------------------------------------------------*/
#if defined(__GNUC__)

        /* va_list type built-in to the compiler */
        typedef __builtin_va_list va_list;

/* Initialize ap to point to first unnamed argument after last named parameter
 */
#define va_start(ap, last) __builtin_va_start((ap), (last))

/* Retrieve next argument of given type */
#define va_arg(ap, type) __builtin_va_arg((ap), type)

/* Clean up the va_list when done */
#define va_end(ap) __builtin_va_end(ap)

/* Copy state of one va_list to another */
#ifndef va_copy
#define va_copy(dest, src) __builtin_va_copy((dest), (src))
#endif

#else
#error "stdarg.h requires GCC or Clang built-in va_list support"
#endif

#ifdef __cplusplus
}
#endif
