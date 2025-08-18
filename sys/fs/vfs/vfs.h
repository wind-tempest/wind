// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <lib/kstdio/kstddef.h>

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
