// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "arch/x86_64/registers.h"

#include <tempest/drivers/driver.h>
#include <tempest/klibc/kstdio/kstdio.h>
#include <tempest/klibc/kstdlib/kitoa.h>
#include <tempest/klibc/kstring/kmemset.h>
#include <tempest/klibc/kunistd/ksleep.h>

// Panic error codes.
#define PANIC_UNKNOWN_ERROR       0
#define PANIC_DIVISION_BY_ZERO    1
#define PANIC_DOUBLE_FAULT        2
#define PANIC_GENERAL_PROTECTION  3
#define PANIC_PAGE_FAULT          4
#define PANIC_STACK_SEGMENT       5
#define PANIC_SEGMENT_NOT_PRESENT 6
#define PANIC_INVALID_TSS         7
#define PANIC_ALIGNMENT_CHECK     8
#define PANIC_MACHINE_CHECK       9
#define PANIC_SIMD_EXCEPTION      10
#define PANIC_VIRTUALIZATION      11
#define PANIC_CONTROL_PROTECTION  12
#define PANIC_HYPERVISOR          13
#define PANIC_VMM_COMMUNICATION   14
#define PANIC_SECURITY            15
#define PANIC_INVALID_OPCODE \
	16  // For some reason, this is every time used instead of the proper ones.

unsigned int seconds_to_reboot = 5;

// Get kpanic message based on error code.
static const char *
    get_panic_message (int code) {
	switch (code) {
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
			return "Invalid opcode";  // I hate you.
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
	char buff[32];

	pputs(" RAX=");
	kitoa(buff, buff + 30, (long) r->rax, 16, 0);
	pputs(buff);
	pputs(" RBX=");
	kitoa(buff, buff + 30, (long) r->rbx, 16, 0);
	pputs(buff);
	pputs(" RCX=");
	kitoa(buff, buff + 30, (long) r->rcx, 16, 0);
	pputs(buff);
	pputs(" RDX=");
	kitoa(buff, buff + 30, (long) r->rdx, 16, 0);
	pputs(buff);
	pputs("\n");

	pputs(" RSI=");
	kitoa(buff, buff + 30, (long) r->rsi, 16, 0);
	pputs(buff);
	pputs(" RDI=");
	kitoa(buff, buff + 30, (long) r->rdi, 16, 0);
	pputs(buff);
	pputs(" RBP=");
	kitoa(buff, buff + 30, (long) r->rbp, 16, 0);
	pputs(buff);

	pputs(" R8 =");
	kitoa(buff, buff + 30, (long) r->r8, 16, 0);
	pputs(buff);
	pputs(" R9 =");
	kitoa(buff, buff + 30, (long) r->r9, 16, 0);
	pputs(buff);
	pputs(" R10=");
	kitoa(buff, buff + 30, (long) r->r10, 16, 0);
	pputs(buff);
	pputs(" R11=");
	kitoa(buff, buff + 30, (long) r->r11, 16, 0);
	pputs(buff);
	pputs("\n");

	pputs(" R12=");
	kitoa(buff, buff + 30, (long) r->r12, 16, 0);
	pputs(buff);
	pputs(" R13=");
	kitoa(buff, buff + 30, (long) r->r13, 16, 0);
	pputs(buff);
	pputs(" R14=");
	kitoa(buff, buff + 30, (long) r->r14, 16, 0);
	pputs(buff);
	pputs(" R15=");
	kitoa(buff, buff + 30, (long) r->r15, 16, 0);
	pputs(buff);
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
	char buff[16];
	kitoa(buff, buff + 14, code, 10, 0);
	pputs(buff);
	pputs("\n\nError: ");
	pputs(error_msg);
	pputs("\n");

	if (regs) {
		pputs("\nRegister dump:\n");
		dump_registers(regs);
	}

	kmemset(buff, 0, sizeof(buff));

	pputs("System will reboot in ");
	kitoa(buff, buff + 14, seconds_to_reboot, 10, 0);
	pputs(buff);
	pputs(" seconds...\n");

	kmemset(buff, 0, sizeof(buff));

	//  ̄\_(ツ)_/ ̄
	for (unsigned int i = seconds_to_reboot; i > 0; i--) {
		pputs("Rebooting in ");
		kitoa(buff, buff + 14, i, 10, 0);
		pputs(buff);
		pputs(" seconds...\n");
		ksleep(1000);
	}

	pputs("Rebooting now...\n");

	// Reboot the system.
	kreboot();

	while (1) {
		__asm__ volatile("hlt");
	}
}

