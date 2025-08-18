// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

void
    kduts (const char *s);
int
    kdebugf (const char *format, ...);
void
    kcritical (const char *message, const char *subsystem, const char *extra);
void
    kalert (const char *message, const char *subsystem, const char *extra);
void
    kemerg (const char *message, const char *subsystem, const char *extra);
void
    kwarn (const char *message, const char *subsystem, const char *extra);
void
    kerror (const char *message, const char *subsystem, const char *extra);
void
    knotice (const char *message, const char *subsystem, const char *extra);
void
    kinfo (const char *message, const char *subsystem, const char *extra);

