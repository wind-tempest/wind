// vfs.h

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#pragma once

#include "kstddef.h"

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
