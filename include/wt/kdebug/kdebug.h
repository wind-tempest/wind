// kdebug.h

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
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
