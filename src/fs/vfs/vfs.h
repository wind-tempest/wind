/* vfs.h */

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
    vfs_getcwd (char *out, size_t size);

/* Resolve an arbitrary path (absolute or relative) into an absolute canonical path. */
void
    vfs_resolve (const char *path, char *out, size_t size);

/* Canonicalise path: resolves '.', '..', duplicate slashes; assumes input absolute or produced by vfs_resolve */
void
    vfs_normalize_path (const char *path, char *out, size_t size);
