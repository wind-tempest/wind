// vfs.h

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#pragma once

#include <wt/kstdio/kstddef.h>

/*
 * Very small VFS helpers for current working directory handling.
 * We only track a single global CWD at the moment (one task / shell).
 */

/*
 * Change the current working directory.
 * Accepts absolute or relative path. Returns 0 on success, negative error code otherwise.
 */
int
    vfs_chdir (const char *path);

// Retrieve the canonical absolute current working directory.
void
    vfs_getcwd (char *out, ksize_t size);

// Resolve an arbitrary path (absolute or relative) into an absolute canonical path.
void
    vfs_resolve (const char *path, char *out, ksize_t size);

// Canonicalise path: resolves '.', '..', duplicate slashes; assumes input absolute or produced by vfs_resolve
void
    vfs_normalize_path (const char *path, char *out, ksize_t size);
