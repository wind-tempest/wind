// panic.c

/*
 * ============================================================================
 *  Wind Operating System / Tempest Kernel
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ============================================================================
 *
 *  Overview:
 *  This source file is part of the Wind OS and Tempest Kernel project, a fully
 *  open-source operating system kernel developed to provide a robust, scalable,
 *  and efficient platform optimized for modern hardware and user-centric freedom.
 *
 *  Project Scope:
 *  Wind/Tempest targets a modular architecture supporting symmetric multiprocessing,
 *  preemptive multitasking, fine-grained memory management, and extensible driver
 *  frameworks. The system emphasizes low latency, high throughput, and
 *  predictable behavior for both user-space applications and kernel subsystems.
 *
 *  Licensing Terms:
 *  The Wind/Tempest source code is licensed under the GNU Affero General Public
 *  License (AGPL) version 3 or any later version, published by the Free Software Foundation.
 *  This license guarantees that all derivative works remain open and freely
 *  available, particularly addressing network use cases to enforce sharing.
 *
 *  You are authorized to copy, modify, distribute, and use this software,
 *  provided that all modifications and derivatives comply with the AGPL terms.
 *  Inclusion of proper attribution and licensing notices is mandatory in redistributions.
 *
 *  Warranty and Liability Disclaimer:
 *  This software is provided "as-is", without any express or implied warranty,
 *  including but not limited to implied warranties of merchantability, fitness for
 *  a particular purpose, or non-infringement of third-party rights.
 *  Under no circumstances shall the authors, contributors, or copyright holders be liable
 *  for any damages arising directly or indirectly from the use or inability to use
 *  this software, including data loss, system failures, or other incidental damages.
 *
 *  Contribution Guidelines:
 *  Contributions to the Wind/Tempest project are welcome and encouraged.
 *  Contributors must agree to license their contributions under the same AGPL terms.
 *  When submitting patches, please ensure they adhere to the project's coding standards,
 *  include proper documentation, and maintain backward compatibility where possible.
 *
 *  Security and Stability:
 *  The kernel maintains rigorous coding practices to minimize security vulnerabilities.
 *  Regular code audits, testing, and community reviews are integral to the project's
 *  ongoing stability and security posture.
 *  Users and developers should report issues promptly via official channels.
 *
 *  Documentation and Support:
 *  Comprehensive documentation including API references, architecture overviews,
 *  and development guides are available on the official website:
 *    https://wind.infernointeractive.win
 *  Community support can be found through mailing lists, forums, and issue trackers
 *  hosted on the project's main repository.
 *
 *  Additional Notes:
 *  This kernel is designed to be hardware agnostic, supporting a wide range of
 *  architectures and devices. Modularity in subsystems allows customization
 *  for embedded, desktop, and server deployments.
 *
 *  By using or modifying this code, you agree to uphold the principles of free and
 *  open-source software, contributing back improvements to foster collective progress.
 *
 *  A copy of the full GNU Affero General Public License should accompany this software.
 *  If you have not received it, it can be obtained at:
 *    https://www.gnu.org/licenses/agpl-3.0.html
 *
 * ============================================================================
 */

#include "core/acpi/acpi.h"
#include "core/cpu/registers.h"
#include "drivers/serial/serial.h"
#include "drivers/video/video.h"
#include "kitoa.h"
#include "ksleep.h"
#include "kstdio.h"

// Panic error codes.
#define PANIC_UNKNOWN_ERROR	  0
#define PANIC_DIVISION_BY_ZERO	  1
#define PANIC_DOUBLE_FAULT	  2
#define PANIC_GENERAL_PROTECTION  3
#define PANIC_PAGE_FAULT	  4
#define PANIC_STACK_SEGMENT	  5
#define PANIC_SEGMENT_NOT_PRESENT 6
#define PANIC_INVALID_TSS	  7
#define PANIC_ALIGNMENT_CHECK	  8
#define PANIC_MACHINE_CHECK	  9
#define PANIC_SIMD_EXCEPTION	  10
#define PANIC_VIRTUALIZATION	  11
#define PANIC_CONTROL_PROTECTION  12
#define PANIC_HYPERVISOR	  13
#define PANIC_VMM_COMMUNICATION	  14
#define PANIC_SECURITY		  15
#define PANIC_INVALID_OPCODE                                                                       \
	16 // For some reason, this is every time used instead of the proper ones.

// Get panic message based on error code.
static const char *
    get_panic_message (int code) {
	switch ( code ) {
		case PANIC_DIVISION_BY_ZERO:
			return "Division by zero";
		case PANIC_DOUBLE_FAULT:
			return "Double fault";
		case PANIC_GENERAL_PROTECTION:
			return "General protection fault";
		case PANIC_PAGE_FAULT:
			return "Page fault";
		case PANIC_STACK_SEGMENT:
			return "Stack segment fault";
		case PANIC_SEGMENT_NOT_PRESENT:
			return "Segment not present";
		case PANIC_INVALID_TSS:
			return "Invalid TSS";
		case PANIC_ALIGNMENT_CHECK:
			return "Alignment check";
		case PANIC_MACHINE_CHECK:
			return "Machine check";
		case PANIC_SIMD_EXCEPTION:
			return "SIMD exception";
		case PANIC_VIRTUALIZATION:
			return "Virtualization exception";
		case PANIC_CONTROL_PROTECTION:
			return "Control protection exception";
		case PANIC_HYPERVISOR:
			return "Hypervisor injection exception";
		case PANIC_VMM_COMMUNICATION:
			return "VMM communication exception";
		case PANIC_SECURITY:
			return "Security exception";
		case PANIC_INVALID_OPCODE:
			return "Invalid opcode"; // I hate you.
		default:
			return "Unknown error";
	}
}

/*
 * Indicates whether we are already inside a panic handler. This prevents
 * recursive panics (e.g., if the panic handler itself triggers a fault),
 * which otherwise lead to double-/triple-faults and a silent reboot.
 */
static kbool panic_in_progress = kfalse;

/*
 * Why did I make this function? Well...
 * I don't want the code to repeat so much.
 */
void
    pputs (const char *s) {
	serial_writes(s);
	kputs(s);
}

void
    panic (int code, registers_t *regs) {
	panic_in_progress = ktrue;
	__asm__ volatile("cli");

	kvideo_clear(0x0000ff);

	const char *error_msg = get_panic_message(code);

	pputs("\n\nOops! Your system crashed\n");
	pputs("Error code: ");
	char buf[16];
	kitoa(buf, buf + 14, code, 10, 0);
	pputs(buf);
	pputs("\nError: ");
	pputs(error_msg);
	pputs("\n");

	if ( regs ) {
		// TODO: Print registers.
	}

	pputs("System will reboot in 5 seconds...\n");

	for ( int i = 5; i > 0; i-- ) {
		pputs("Rebooting in ");
		pputs(buf);
		pputs(" seconds...\n");
		ksleep(1000);
	}

	pputs("Rebooting now...\n");

	// Reboot the system.
	kreboot();

	// If reboot fails, halt the system.
	pputs("Reboot failed! System halted.\n");
	while ( 1 ) {
		__asm__ volatile("hlt");
	}
}
