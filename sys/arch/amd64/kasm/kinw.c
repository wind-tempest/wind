// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "arch/amd64/kasm/kinw.h"

kuint16_t
    kinw (kuint16_t port)
{
	kuint16_t val;
	__asm__ __volatile__("inw %1, %0" : "=a"(val) : "Nd"(port));
	return val;
}
