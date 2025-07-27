// kctype.h

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

int
    kisupper (int c);
int
    kislower (int c);
int
    kisalpha (int c);
int
    kisdigit (int c);
int
    kisxdigit (int c);
int
    kisalnum (int c);
int
    kisspace (int c);
int
    kisblank (int c);
int
    kiscntrl (int c);
int
    kisgraph (int c);
int
    kisprint (int c);
int
    kispunct (int c);
int
    ktolower (int c);
int
    ktoupper (int c);
