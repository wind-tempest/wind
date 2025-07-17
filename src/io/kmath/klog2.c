/* klog2.c */

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

#include "klog2.h"
#include "kmath.h"

double
    klog2 (double x)
{
	/* Handle special cases */
	if ( kisnan(x) )
		return knan("");
	if ( x <= 0.0 )
		return knan("");

	/* Use the relationship: log2(x) = ln(x) / ln(2) */
	return klog(x) / K_M_LN2;
}
