/* kstdint.h */

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

#ifdef _STDINT_H
#error "Do NOT include <stdint.h> before kstdint.h! Use only kstdint.h in this project."
#endif

/* Int */
typedef signed char int8_t;
typedef short	    int16_t;
typedef int	    int32_t;
typedef long long   int64_t;

/* Unsigned Int */
typedef unsigned char	   uint8_t;
typedef unsigned short	   uint16_t;
typedef unsigned int	   uint32_t;
typedef unsigned long long uint64_t;

/* Native sizes */
typedef long long	   intptr_t;
typedef unsigned long long uintptr_t;

/* Maximum value for uintptr_t */
#define UINTPTR_MAX 18446744073709551615ULL
