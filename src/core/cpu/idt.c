// idt.c

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

#include "core/panic/panic.h"
#include "idt.h"
#include "ksignal.h"
#include "kstdio.h"

// PIC (Programmable Interrupt Controller) ports.
#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI	  0x20 /* End-of-Interrupt command. */

// External declarations for ISR and IRQ handlers.
extern void
    isr0 (void);
extern void
    isr1 (void);
extern void
    isr2 (void);
extern void
    isr3 (void);
extern void
    isr4 (void);
extern void
    isr5 (void);
extern void
    isr6 (void);
extern void
    isr7 (void);
extern void
    isr8 (void);
extern void
    isr9 (void);
extern void
    isr10 (void);
extern void
    isr11 (void);
extern void
    isr12 (void);
extern void
    isr13 (void);
extern void
    isr14 (void);
extern void
    isr15 (void);
extern void
    isr16 (void);
extern void
    isr17 (void);
extern void
    isr18 (void);
extern void
    isr19 (void);
extern void
    isr20 (void);
extern void
    isr21 (void);
extern void
    isr22 (void);
extern void
    isr23 (void);
extern void
    isr24 (void);
extern void
    isr25 (void);
extern void
    isr26 (void);
extern void
    isr27 (void);
extern void
    isr28 (void);
extern void
    isr29 (void);
extern void
    isr30 (void);
extern void
    isr31 (void);

extern void
    irq0 (void);
extern void
    irq1 (void);
extern void
    irq2 (void);
extern void
    irq3 (void);
extern void
    irq4 (void);
extern void
    irq5 (void);
extern void
    irq6 (void);
extern void
    irq7 (void);
extern void
    irq8 (void);
extern void
    irq9 (void);
extern void
    irq10 (void);
extern void
    irq11 (void);
extern void
    irq12 (void);
extern void
    irq13 (void);
extern void
    irq14 (void);
extern void
    irq15 (void);

extern void *isr_stub_table[];

// Array of C-level interrupt handlers.
static irq_handler_t irq_handlers[16];

void
    register_irq_handler (int irq, irq_handler_t handler) {
	irq_handlers[irq] = handler;
}

// IDT entry structure.
struct idt_entry {
	kuint16_t base_lo;
	kuint16_t sel;
	kuint8_t  ist;
	kuint8_t  flags;
	kuint16_t base_mid;
	kuint32_t base_hi;
	kuint32_t reserved;
} __attribute__((packed));

// IDT pointer structure.
struct idt_ptr {
	kuint16_t limit;
	kuint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr	idtp;

// Set up an IDT entry.
static void
    idt_set_gate (kuint8_t num, kuint64_t base, kuint16_t sel, kuint8_t flags) {
	idt[num].base_lo  = (kuint16_t) (base & 0xFFFF);
	idt[num].base_mid = (kuint16_t) ((base >> 16) & 0xFFFF);
	idt[num].base_hi  = (kuint32_t) ((base >> 32) & 0xFFFFFFFF);
	idt[num].sel	  = sel;
	idt[num].ist	  = 0;
	idt[num].flags	  = flags;
	idt[num].reserved = 0;
}

// Default C-level handlers.
void
    isr_handler (registers_t *regs) {
	// Map interrupt numbers to panic codes.
	int panic_code = PANIC_UNKNOWN_ERROR;

	switch ( regs->int_no ) {
		case 0:
			panic_code = PANIC_DIVISION_BY_ZERO;
			break;
		case 6:
			panic_code = PANIC_INVALID_OPCODE; /* Invalid opcode */
			break;
		case 8:
			panic_code = PANIC_DOUBLE_FAULT;
			break;
		case 13:
			panic_code = PANIC_GENERAL_PROTECTION;
			break;
		case 14:
			panic_code = PANIC_PAGE_FAULT;
			break;
		case 12:
			panic_code = PANIC_STACK_SEGMENT;
			break;
		case 11:
			panic_code = PANIC_SEGMENT_NOT_PRESENT;
			break;
		case 10:
			panic_code = PANIC_INVALID_TSS;
			break;
		case 17:
			panic_code = PANIC_ALIGNMENT_CHECK;
			break;
		case 18:
			panic_code = PANIC_MACHINE_CHECK;
			break;
		case 19:
			panic_code = PANIC_SIMD_EXCEPTION;
			break;
		case 20:
			panic_code = PANIC_VIRTUALIZATION;
			break;
		case 21:
			panic_code = PANIC_CONTROL_PROTECTION;
			break;
		case 28:
			panic_code = PANIC_HYPERVISOR;
			break;
		case 29:
			panic_code = PANIC_VMM_COMMUNICATION;
			break;
		case 30:
			panic_code = PANIC_SECURITY;
			break;
		default:
			panic_code = PANIC_UNKNOWN_ERROR;
			break;
	}

	// Call panic with the appropriate error code and registers.
	panic(panic_code, regs);
}

void
    irq_handler (registers_t *regs) {
	// If a custom handler is registered, call it.
	if ( regs->int_no >= 32 && regs->int_no <= 47 ) {
		irq_handler_t handler = irq_handlers[regs->int_no - 32];
		if ( handler ) {
			handler(regs);
		}
	}

	// Send End-of-Interrupt (EOI) to the PICs.
	if ( regs->int_no >= 40 ) {
		koutb(PIC2_CMD, PIC_EOI); /* EOI to slave PIC. */
	}
	koutb(PIC1_CMD, PIC_EOI); /* EOI to master PIC. */
}

void
    idt_init (void) {
	// Set up the IDT pointer.
	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base  = (kuint64_t) &idt;

	// Remap the PIC.
	koutb(PIC1_CMD, 0x11);
	koutb(PIC2_CMD, 0x11);
	koutb(PIC1_DATA, 0x20); /* Master PIC vector offset. */
	koutb(PIC2_DATA, 0x28); /* Slave PIC vector offset. */
	koutb(PIC1_DATA, 0x04); /* Tell Master PIC there is a slave PIC at IRQ2. */
	koutb(PIC2_DATA, 0x02); /* Tell Slave PIC its cascade identity. */
	koutb(PIC1_DATA, 0x01); /* 8086/88 (MCS-80/85) mode. */
	koutb(PIC2_DATA, 0x01);
	koutb(PIC1_DATA, 0x0);
	koutb(PIC2_DATA, 0x0);

	// Set up ISR entries.
	idt_set_gate(0, (kuint64_t) isr0, 0x08, 0x8E);
	idt_set_gate(1, (kuint64_t) isr1, 0x08, 0x8E);
	idt_set_gate(2, (kuint64_t) isr2, 0x08, 0x8E);
	idt_set_gate(3, (kuint64_t) isr3, 0x08, 0x8E);
	idt_set_gate(4, (kuint64_t) isr4, 0x08, 0x8E);
	idt_set_gate(5, (kuint64_t) isr5, 0x08, 0x8E);
	idt_set_gate(6, (kuint64_t) isr6, 0x08, 0x8E);
	idt_set_gate(7, (kuint64_t) isr7, 0x08, 0x8E);
	idt_set_gate(8, (kuint64_t) isr8, 0x08, 0x8E);
	idt_set_gate(9, (kuint64_t) isr9, 0x08, 0x8E);
	idt_set_gate(10, (kuint64_t) isr10, 0x08, 0x8E);
	idt_set_gate(11, (kuint64_t) isr11, 0x08, 0x8E);
	idt_set_gate(12, (kuint64_t) isr12, 0x08, 0x8E);
	idt_set_gate(13, (kuint64_t) isr13, 0x08, 0x8E);
	idt_set_gate(14, (kuint64_t) isr14, 0x08, 0x8E);
	idt_set_gate(15, (kuint64_t) isr15, 0x08, 0x8E);
	idt_set_gate(16, (kuint64_t) isr16, 0x08, 0x8E);
	idt_set_gate(17, (kuint64_t) isr17, 0x08, 0x8E);
	idt_set_gate(18, (kuint64_t) isr18, 0x08, 0x8E);
	idt_set_gate(19, (kuint64_t) isr19, 0x08, 0x8E);
	idt_set_gate(20, (kuint64_t) isr20, 0x08, 0x8E);
	idt_set_gate(21, (kuint64_t) isr21, 0x08, 0x8E);
	idt_set_gate(22, (kuint64_t) isr22, 0x08, 0x8E);
	idt_set_gate(23, (kuint64_t) isr23, 0x08, 0x8E);
	idt_set_gate(24, (kuint64_t) isr24, 0x08, 0x8E);
	idt_set_gate(25, (kuint64_t) isr25, 0x08, 0x8E);
	idt_set_gate(26, (kuint64_t) isr26, 0x08, 0x8E);
	idt_set_gate(27, (kuint64_t) isr27, 0x08, 0x8E);
	idt_set_gate(28, (kuint64_t) isr28, 0x08, 0x8E);
	idt_set_gate(29, (kuint64_t) isr29, 0x08, 0x8E);
	idt_set_gate(30, (kuint64_t) isr30, 0x08, 0x8E);
	idt_set_gate(31, (kuint64_t) isr31, 0x08, 0x8E);

	// Set up IRQ entries.
	idt_set_gate(32, (kuint64_t) irq0, 0x08, 0x8E);
	idt_set_gate(33, (kuint64_t) irq1, 0x08, 0x8E);
	idt_set_gate(34, (kuint64_t) irq2, 0x08, 0x8E);
	idt_set_gate(35, (kuint64_t) irq3, 0x08, 0x8E);
	idt_set_gate(36, (kuint64_t) irq4, 0x08, 0x8E);
	idt_set_gate(37, (kuint64_t) irq5, 0x08, 0x8E);
	idt_set_gate(38, (kuint64_t) irq6, 0x08, 0x8E);
	idt_set_gate(39, (kuint64_t) irq7, 0x08, 0x8E);
	idt_set_gate(40, (kuint64_t) irq8, 0x08, 0x8E);
	idt_set_gate(41, (kuint64_t) irq9, 0x08, 0x8E);
	idt_set_gate(42, (kuint64_t) irq10, 0x08, 0x8E);
	idt_set_gate(43, (kuint64_t) irq11, 0x08, 0x8E);
	idt_set_gate(44, (kuint64_t) irq12, 0x08, 0x8E);
	idt_set_gate(45, (kuint64_t) irq13, 0x08, 0x8E);
	idt_set_gate(46, (kuint64_t) irq14, 0x08, 0x8E);
	idt_set_gate(47, (kuint64_t) irq15, 0x08, 0x8E);

	// Load the IDT.
	__asm__ volatile("lidt %0" : : "m"(idtp));
}
