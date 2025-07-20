/* kstdarg.h */

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

#pragma once

#ifdef _STDARG_H
#	error "Do NOT include <stdarg.h> before kstdarg.h! Use only kstdarg.h in this project."
#endif

#ifdef __cplusplus
extern "C" {
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
#	define k_va_start(ap, last) __builtin_va_start((ap), (last))

/* Retrieve next argument of given type */
#	define k_va_arg(ap, type) __builtin_va_arg((ap), type)

/* Clean up the va_list when done */
#	define k_va_end(ap) __builtin_va_end(ap)

/* Copy state of one va_list to another */
#	ifndef k_va_copy
#		define k_va_copy(dest, src) __builtin_va_copy((dest), (src))
#	endif

#else
#	error "stdarg.h requires GCC or Clang built-in va_list support"
#endif

#ifdef __cplusplus
}
#endif
