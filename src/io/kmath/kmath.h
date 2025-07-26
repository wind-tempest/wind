// kmath.h

/* ============================================================================
 * Wind Operating System / Tempest Kernel
 * Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ============================================================================
 *
 * - Overview:
 *   This source file is part of the Wind (Operating System) and Tempest (Kernel), a fully
 *   open-source operating system kernel developed to provide a robust, scalable,
 *   and efficient platform optimized for modern hardware and user-centric freedom.
 *
 * - Project Scope:
 *   Wind/Tempest targets a modular architecture supporting symmetric multiprocessing,
 *   preemptive multitasking, fine-grained memory management, and extensible driver
 *   frameworks. The system emphasizes low latency, high throughput, and
 *   predictable behavior for both user-space applications and kernel subsystems.
 *
 * - Licensing Terms:
 * The Wind/Tempest source code is licensed under the Liberty Software License (LSL) version 1.0,
 *   a free software license based on the GNU Affero General Public License v3 (AGPLv3)
 *   with added provisions to ensure transparency, prevent DRM, and prohibit
 *   unauthorized AI training uses.
 *
 *   You are authorized to copy, modify, distribute, and use this software,
 *   provided that all modifications and derivatives comply with the LSL terms.
 *   Inclusion of proper attribution and licensing notices is mandatory in redistributions.
 *
 * - Warranty and Liability Disclaimer:
 *   This software is provided "as-is", without any express or implied warranty,
 *   including but not limited to implied warranties of merchantability, fitness for
 *   a particular purpose, or non-infringement of third-party rights.
 *   Under no circumstances shall the authors, contributors, or copyright holders be liable
 *   for any damages arising directly or indirectly from the use or inability to use
 *   this software, including data loss, system failures, or other incidental damages.
 *
 * - Contribution Guidelines:
 *   Contributions to the Wind/Tempest project are welcome and encouraged.
 *   Contributors must agree to license their contributions under the same LSL terms.
 *   When submitting patches, please ensure they adhere to the project's coding standards,
 *   include proper documentation, and maintain backward compatibility where possible.
 *
 * - Security and Stability:
 *   The kernel maintains rigorous coding practices to minimize security vulnerabilities.
 *   Regular code audits, testing, and community reviews are integral to the project's
 *   ongoing stability and security posture.
 *   Users and developers should report issues promptly via official channels.
 *
 * - Documentation and Support:
 *   Comprehensive documentation including API references, architecture overviews,
 *   and development guides are available on the official website:
 *     https://wind.infernointeractive.win
 *   Community support can be found through mailing lists, forums, and issue trackers
 *   hosted on the project's main repository.
 *
 * - Additional Notes:
 *   This kernel is designed to be hardware agnostic, supporting a wide range of
 *   architectures and devices. Modularity in subsystems allows customization
 *   for embedded, desktop, and server deployments.
 *
 * By using or modifying this code, you agree to uphold the principles of free and
 * open-source software, contributing back improvements to foster collective progress.
 */

#pragma once

#include "kacos.h"
#include "kasin.h"
#include "katan.h"
#include "katan2.h"
#include "kcbrt.h"
#include "kceil.h"
#include "kcopysign.h"
#include "kcos.h"
#include "kexp.h"
#include "kexp2.h"
#include "kexpm1.h"
#include "kfabs.h"
#include "kfloor.h"
#include "kfmax.h"
#include "kfmin.h"
#include "kfmod.h"
#include "kfrexp.h"
#include "khypot.h"
#include "kisfinite.h"
#include "kisinf.h"
#include "kisnan.h"
#include "kldexp.h"
#include "klog.h"
#include "klog10.h"
#include "klog1p.h"
#include "klog2.h"
#include "kmath.h"
#include "kmodf.h"
#include "knan.h"
#include "knearbyint.h"
#include "knextafter.h"
#include "kpow.h"
#include "kremainder.h"
#include "kremquo.h"
#include "krint.h"
#include "kround.h"
#include "ksin.h"
#include "ksqrt.h"
#include "ktan.h"
#include "ktrunc.h"

// Mathematical constants
#define K_M_E	     2.7182818284590452354  /* e */
#define K_M_LOG2E    1.4426950408889634074  /* log_2 e */
#define K_M_LOG10E   0.43429448190325182765 /* log_10 e */
#define K_M_LN2	     0.69314718055994530942 /* log_e 2 */
#define K_M_LN10     2.30258509299404568402 /* log_e 10 */
#define K_M_PI	     3.14159265358979323846 /* pi */
#define K_M_PI_2     1.57079632679489661923 /* pi/2 */
#define K_M_PI_4     0.78539816339744830962 /* pi/4 */
#define K_M_1_PI     0.31830988618379067154 /* 1/pi */
#define K_M_2_PI     0.63661977236758134308 /* 2/pi */
#define K_M_2_SQRTPI 1.12837916709551257390 /* 2/ksqrt(pi) */
#define K_M_SQRT2    1.41421356237309504880 /* ksqrt(2) */
#define K_M_SQRT1_2  0.70710678118654752440 /* 1/ksqrt(2) */

/*
    Just a fucking warning!
    FP_INFINITE: classification macro (integer)
    INFINITY: floating-point constant (IEEE 754 positive infinity)
*/

// Infinity and NaN constants
#define K_INFINITY (1.0 / 0.0)
#define K_NAN	   (0.0 / 0.0)

// Classification macros
#define K_FP_NAN       0
#define K_FP_ZERO      2
#define K_FP_SUBNORMAL 3
#define K_FP_NORMAL    4
