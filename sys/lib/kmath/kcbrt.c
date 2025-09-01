// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kcbrt.h>
#include <lib/kmath/kmath.h>

double
    kcbrt (double x) {
	// Handle special cases
	if (kisnan(x))
		return knan("");
	if (kisinf(x))
		return x;
	if (x == 0.0)
		return 0.0;

	// Use Newton's method for cube root
	double y = kfabs(x);
	double guess;

	// Initial guess
	if (y > 1.0)
		guess = y / 3.0;
	else
		guess = y * 2.0 / 3.0;

	// Newton iteration: x_{n+1} = (2*x_n + y/x_n^2) / 3
	for (int i = 0; i < 10; i++) {
		double guess2    = guess * guess;
		double new_guess = (2.0 * guess + y / guess2) / 3.0;
		if (kfabs(new_guess - guess) < 1e-15)
			break;
		guess = new_guess;
	}

	return x < 0.0 ? -guess : guess;
}
