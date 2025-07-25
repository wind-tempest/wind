// kerrno.c

/*
 * ============================================================================
 *  Wind Operating System / Tempest Kernel
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ============================================================================
 *
 *  - Overview:
 *  	This source file is part of the Wind (Operating System) and Tempest (Kernel), a fully
 *  	open-source operating system kernel developed to provide a robust, scalable,
 *  	and efficient platform optimized for modern hardware and user-centric freedom.
 *
 *  - Project Scope:
 *  	Wind/Tempest targets a modular architecture supporting symmetric multiprocessing,
 *  	preemptive multitasking, fine-grained memory management, and extensible driver
 *  	frameworks. The system emphasizes low latency, high throughput, and
 *  	predictable behavior for both user-space applications and kernel subsystems.
 *
 *  - Licensing Terms:
 *  	The Wind/Tempest source code is licensed under the Liberty Software License (LSL) version 1.0,
 *  	a free software license based on the GNU Affero General Public License v3 (AGPLv3)
 *  	with added provisions to ensure transparency, prevent DRM, and prohibit
 *  	unauthorized AI training uses.
 *
 *  You are authorized to copy, modify, distribute, and use this software,
 *  provided that all modifications and derivatives comply with the LSL terms.
 *  Inclusion of proper attribution and licensing notices is mandatory in redistributions.
 *
 *  - Warranty and Liability Disclaimer:
 *  	This software is provided "as-is", without any express or implied warranty,
 *  	including but not limited to implied warranties of merchantability, fitness for
 *  	a particular purpose, or non-infringement of third-party rights.
 *  	Under no circumstances shall the authors, contributors, or copyright holders be liable
 *  	for any damages arising directly or indirectly from the use or inability to use
 *  	this software, including data loss, system failures, or other incidental damages.
 *
 *  - Contribution Guidelines:
 *  	Contributions to the Wind/Tempest project are welcome and encouraged.
 *  	Contributors must agree to license their contributions under the same LSL terms.
 *  	When submitting patches, please ensure they adhere to the project's coding standards,
 *  	include proper documentation, and maintain backward compatibility where possible.
 *
 *  - Security and Stability:
 *  	The kernel maintains rigorous coding practices to minimize security vulnerabilities.
 *  	Regular code audits, testing, and community reviews are integral to the project's
 *  	ongoing stability and security posture.
 *  	Users and developers should report issues promptly via official channels.
 *
 *  - Documentation and Support:
 *  	Comprehensive documentation including API references, architecture overviews,
 *  	and development guides are available on the official website:
 *    	https://wind.infernointeractive.win
 *  	Community support can be found through mailing lists, forums, and issue trackers
 *  	hosted on the project's main repository.
 *
 *  - Additional Notes:
 *  	This kernel is designed to be hardware agnostic, supporting a wide range of
 *  	architectures and devices. Modularity in subsystems allows customization
 *  	for embedded, desktop, and server deployments.
 *
 *  By using or modifying this code, you agree to uphold the principles of free and
 *  open-source software, contributing back improvements to foster collective progress.
 *
 * ============================================================================
 */

#include "kerrno.h"

int kerrno = 0;
