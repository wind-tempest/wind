/* vfs.h */

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

#include "stddef.h"

/*
 * Very small VFS helpers for current working directory handling.
 * We only track a single global CWD at the moment (one task / shell).
 */

/* Change the current working directory.
 * Accepts absolute or relative path. Returns 0 on success, negative error code otherwise. */
int
    vfs_chdir (const char *path);

/* Retrieve the canonical absolute current working directory. */
void
    vfs_getcwd (char *out, ksize_t size);

/* Resolve an arbitrary path (absolute or relative) into an absolute canonical path. */
void
    vfs_resolve (const char *path, char *out, ksize_t size);

/* Canonicalise path: resolves '.', '..', duplicate slashes; assumes input absolute or produced by vfs_resolve */
void
    vfs_normalize_path (const char *path, char *out, ksize_t size);
