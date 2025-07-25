// kpanic.c

/*
============================================================================
 Wind Operating System / Tempest Kernel
 Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
============================================================================

 - Overview:
   This source file is part of the Wind (Operating System) and Tempest (Kernel), a fully
   open-source operating system kernel developed to provide a robust, scalable,
   and efficient platform optimized for modern hardware and user-centric freedom.

 - Project Scope:
   Wind/Tempest targets a modular architecture supporting symmetric multiprocessing,
   preemptive multitasking, fine-grained memory management, and extensible driver
   frameworks. The system emphasizes low latency, high throughput, and
   predictable behavior for both user-space applications and kernel subsystems.

 - Licensing Terms:
   The Wind/Tempest source code is licensed under the Liberty Software License (LSL) version 1.0,
   a free software license based on the GNU Affero General Public License v3 (AGPLv3)
   with added provisions to ensure transparency, prevent DRM, and prohibit
   unauthorized AI training uses.

   You are authorized to copy, modify, distribute, and use this software,
   provided that all modifications and derivatives comply with the LSL terms.
   Inclusion of proper attribution and licensing notices is mandatory in redistributions.

 - Warranty and Liability Disclaimer:
   This software is provided "as-is", without any express or implied warranty,
   including but not limited to implied warranties of merchantability, fitness for
   a particular purpose, or non-infringement of third-party rights.
   Under no circumstances shall the authors, contributors, or copyright holders be liable
   for any damages arising directly or indirectly from the use or inability to use
   this software, including data loss, system failures, or other incidental damages.

 - Contribution Guidelines:
   Contributions to the Wind/Tempest project are welcome and encouraged.
   Contributors must agree to license their contributions under the same LSL terms.
   When submitting patches, please ensure they adhere to the project's coding standards,
   include proper documentation, and maintain backward compatibility where possible.

 - Security and Stability:
   The kernel maintains rigorous coding practices to minimize security vulnerabilities.
   Regular code audits, testing, and community reviews are integral to the project's
   ongoing stability and security posture.
   Users and developers should report issues promptly via official channels.

 - Documentation and Support:
   Comprehensive documentation including API references, architecture overviews,
   and development guides are available on the official website:
     https://wind.infernointeractive.win
   Community support can be found through mailing lists, forums, and issue trackers
   hosted on the project's main repository.

 - Additional Notes:
   This kernel is designed to be hardware agnostic, supporting a wide range of
   architectures and devices. Modularity in subsystems allows customization
   for embedded, desktop, and server deployments.

 By using or modifying this code, you agree to uphold the principles of free and
 open-source software, contributing back improvements to foster collective progress.

============================================================================
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

// Get kpanic message based on error code.
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
 * Indicates whether we are already inside a kpanic handler. This prevents
 * recursive panics (e.g., if the kpanic handler itself triggers a fault),
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
	kvideo_puts(s);
}

static void
    dump_registers (registers_t *r) {
	char buf[32];

	pputs(" RAX=");
	kitoa(buf, buf + 30, (long) r->rax, 16, 0);
	pputs(buf);
	pputs(" RBX=");
	kitoa(buf, buf + 30, (long) r->rbx, 16, 0);
	pputs(buf);
	pputs(" RCX=");
	kitoa(buf, buf + 30, (long) r->rcx, 16, 0);
	pputs(buf);
	pputs(" RDX=");
	kitoa(buf, buf + 30, (long) r->rdx, 16, 0);
	pputs(buf);
	pputs("\n");

	pputs(" RSI=");
	kitoa(buf, buf + 30, (long) r->rsi, 16, 0);
	pputs(buf);
	pputs(" RDI=");
	kitoa(buf, buf + 30, (long) r->rdi, 16, 0);
	pputs(buf);
	pputs(" RBP=");
	kitoa(buf, buf + 30, (long) r->rbp, 16, 0);
	pputs(buf);

	pputs(" R8 =");
	kitoa(buf, buf + 30, (long) r->r8, 16, 0);
	pputs(buf);
	pputs(" R9 =");
	kitoa(buf, buf + 30, (long) r->r9, 16, 0);
	pputs(buf);
	pputs(" R10=");
	kitoa(buf, buf + 30, (long) r->r10, 16, 0);
	pputs(buf);
	pputs(" R11=");
	kitoa(buf, buf + 30, (long) r->r11, 16, 0);
	pputs(buf);
	pputs("\n");

	pputs(" R12=");
	kitoa(buf, buf + 30, (long) r->r12, 16, 0);
	pputs(buf);
	pputs(" R13=");
	kitoa(buf, buf + 30, (long) r->r13, 16, 0);
	pputs(buf);
	pputs(" R14=");
	kitoa(buf, buf + 30, (long) r->r14, 16, 0);
	pputs(buf);
	pputs(" R15=");
	kitoa(buf, buf + 30, (long) r->r15, 16, 0);
	pputs(buf);
	pputs("\n");
}

void
    kpanic (int code, registers_t *regs) {
	panic_in_progress = ktrue;
	__asm__ volatile("cli");

	kvideo_clear(0x0000ff);

	const char *error_msg = get_panic_message(code);

	pputs("\n\nOops! Your system crashed\n");
	pputs("Error code: ");
	char buf[16];
	kitoa(buf, buf + 14, code, 10, 0);
	pputs(buf);
	pputs("\n\nError: ");
	pputs(error_msg);
	pputs("\n");

	if ( regs ) {
		pputs("\nRegister dump:\n");
		dump_registers(regs);
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
