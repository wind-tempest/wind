// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#ifdef _STDARG_H
#	error \
	    "Do NOT include <stdarg.h> before kstdarg.h! Use only kstdarg.h in this project."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ----------------------------------------------------------------------------
 * Architecture-specific va_list definition
 * For compilers supporting built-in va_list (GCC, Clang)
 *----------------------------------------------------------------------------
 */
#if defined(__GNUC__)

// va_list type built-in to the compiler
typedef __builtin_va_list va_list;

// Initialize ap to point to first unnamed argument after last named parameter
#	define k_va_start(ap, last) __builtin_va_start((ap), (last))

// Retrieve next argument of given type
#	define k_va_arg(ap, type) __builtin_va_arg((ap), type)

// Clean up the va_list when done
#	define k_va_end(ap) __builtin_va_end(ap)

// Copy state of one va_list to another
#	ifndef k_va_copy
#		define k_va_copy(dest, src) __builtin_va_copy((dest), (src))
#	endif

#else
#	error "stdarg.h requires GCC or Clang built-in va_list support"
#endif

#ifdef __cplusplus
}
#endif
