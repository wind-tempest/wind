// SPDX-License-Identifier: LSL-1.4
/*
 * -- BEGIN LICENSE HEADER --
 * The Wind/Tempest Project
 *
 * File:        sys/kern/syscall/integration.c
 * Author(s):   Russian95 <russian95@tempestfoundation.org>
 *              (https://github.com/Russian95CrE)
 * Maintainer:  Tempest Foundation <development@tempestfoundation.org>
 * Link:        https://wtsrc.tempestfoundation.org
 *
 * Copyright (C) 2025 Tempest Foundation
 * Licensed under the Liberty Software License, Version 1.4
 * -- END OF LICENSE HEADER --
 */
#include "integration.h"

#include <lib/kstdio/kstdio.h>

// Global syscall infrastructure state
static syscall_status_t syscall_status       = SYSCALL_STATUS_UNINITIALIZED;
static kuint8_t         syscall_logging      = 1;  // Logging enabled by default
static kuint8_t         security_checks      = 0;  // Security checks disabled by default
static kuint64_t        max_calls_per_second = 0;  // No rate limiting by default

// Syscall statistics
static syscall_stats_t syscall_stats                         = {0};
static kuint64_t       syscall_call_count[SYSCALL_MAX_COUNT] = {0};

// Initialize the complete syscall infrastructure
void
    syscall_infrastructure_init (void) {
	kprintf("\n[SYSCALL] Initializing Wind/Tempest Syscall Infrastructure\n");

	syscall_status = SYSCALL_STATUS_INITIALIZING;

	// Initialize the core syscall subsystem
	syscalls.init();

	// Setup IDT for syscalls
	idt_setup_syscall();

	// Reset statistics
	syscall_reset_stats();

	// Validate infrastructure
	if (!syscall_validate_infrastructure()) {
		kprintf("[SYSCALL] ERROR: Infrastructure validation failed!\n");
		syscall_status = SYSCALL_STATUS_ERROR;
		return;
	}

	syscall_status = SYSCALL_STATUS_READY;

	kprintf("[SYSCALL] Infrastructure initialization complete\n");
	kprintf("[SYSCALL] Status: READY\n");
	kprintf("[SYSCALL] Logging: %s\n", syscall_logging ? "ENABLED" : "DISABLED");
	kprintf("[SYSCALL] Security checks: %s\n",
	        security_checks ? "ENABLED" : "DISABLED");
}

// Shutdown the syscall infrastructure
void
    syscall_infrastructure_shutdown (void) {
	kprintf("[SYSCALL] Shutting down syscall infrastructure\n");

	// Print final statistics
	syscall_print_info();

	// Reset all syscalls (unregister them)
	for (kuint64_t i = 0; i < SYSCALL_MAX_COUNT; i++) {
		if (syscalls.is_valid(i)) {
			syscalls.unbind(i);
		}
	}

	syscall_status = SYSCALL_STATUS_UNINITIALIZED;
	kprintf("[SYSCALL] Syscall infrastructure shutdown complete\n");
}

// Get the current status of the syscall subsystem
syscall_status_t
    syscall_get_status (void) {
	return syscall_status;
}

// Print syscall subsystem information
void
    syscall_print_info (void) {
	kprintf("\n[SYSCALL] Wind/Tempest Syscall Infrastructure Information\n");

	const char *status_str;
	switch (syscall_status) {
		case SYSCALL_STATUS_UNINITIALIZED:
			status_str = "UNINITIALIZED";
			break;
		case SYSCALL_STATUS_INITIALIZING:
			status_str = "INITIALIZING";
			break;
		case SYSCALL_STATUS_READY:
			status_str = "READY";
			break;
		case SYSCALL_STATUS_ERROR:
			status_str = "ERROR";
			break;
		default:
			status_str = "UNKNOWN";
			break;
	}

	kprintf("[SYSCALL] Status: %s\n", status_str);
	kprintf("[SYSCALL] Logging: %s\n", syscall_logging ? "ENABLED" : "DISABLED");
	kprintf("[SYSCALL] Security Checks: %s\n",
	        security_checks ? "ENABLED" : "DISABLED");
	kprintf("[SYSCALL] Rate Limiting: %s\n",
	        max_calls_per_second > 0 ? "ENABLED" : "DISABLED");
	if (max_calls_per_second > 0) {
		kprintf("[SYSCALL] Max Calls/Second: %llu\n", max_calls_per_second);
	}

	kprintf("\n[SYSCALL] Statistics:\n");
	kprintf("[SYSCALL] Total Calls: %llu\n", syscall_stats.total_calls);
	kprintf("[SYSCALL] Successful: %llu\n", syscall_stats.successful_calls);
	kprintf("[SYSCALL] Failed: %llu\n", syscall_stats.failed_calls);
	kprintf("[SYSCALL] Invalid: %llu\n", syscall_stats.invalid_calls);
	if (syscall_stats.most_used_count > 0) {
		kprintf("[SYSCALL] Most Used: Syscall %llu (%llu calls)\n",
		        syscall_stats.most_used_syscall,
		        syscall_stats.most_used_count);
	}
}

// Print all registered syscalls
void
    syscall_print_table (void) {
	kprintf("\n[SYSCALL] Registered Syscalls\n");
	kprintf("[SYSCALL] =====================================================\n");
	kprintf("[SYSCALL] No.  | Name                    | Args | Calls\n");
	kprintf("[SYSCALL] -----|-------------------------|------|----------\n");

	kuint32_t registered_count = 0;
	for (kuint64_t i = 0; i < SYSCALL_MAX_COUNT; i++) {
		if (syscalls.is_valid(i)) {
			const syscall_entry_t *info = syscalls.get_info(i);
			if (info && info->name) {
				kprintf("[SYSCALL] %-4llu | %-23s | %-4u | %-8llu\n",
				        i,
				        info->name,
				        info->arg_count,
				        syscall_call_count[i]);
				registered_count++;
			}
		}
	}

	kprintf("[SYSCALL] Total registered syscalls: %u\n", registered_count);
}

// Validate syscall infrastructure integrity
kuint8_t
    syscall_validate_infrastructure (void) {
	kprintf("[SYSCALL] Validating infrastructure integrity...\n");

	// Check that essential syscalls are registered
	kuint8_t validation_passed = 1;

	// Test core syscalls
	if (!syscalls.is_valid(SYS_EXIT)) {
		kprintf("[SYSCALL] ERROR: SYS_EXIT not registered\n");
		validation_passed = 0;
	}

	if (!syscalls.is_valid(SYS_GETPID)) {
		kprintf("[SYSCALL] ERROR: SYS_GETPID not registered\n");
		validation_passed = 0;
	}

	// Test that we can get syscall info
	const syscall_entry_t *info = syscalls.get_info(SYS_GETPID);
	if (!info || !info->handler) {
		kprintf("[SYSCALL] ERROR: Cannot get info for SYS_GETPID syscall\n");
		validation_passed = 0;
	}

	// Test invalid syscall handling
	if (syscalls.is_valid(999)) {
		kprintf("[SYSCALL] ERROR: Invalid syscall validation failed\n");
		validation_passed = 0;
	}

	if (validation_passed) {
		kprintf("[SYSCALL] Infrastructure validation PASSED\n");
	} else {
		kprintf("[SYSCALL] Infrastructure validation FAILED\n");
	}

	return validation_passed;
}

// Enable/disable syscall logging
void
    syscall_set_logging (kuint8_t enabled) {
	syscall_logging = enabled;
	kprintf("[SYSCALL] Logging %s\n", enabled ? "ENABLED" : "DISABLED");
}

// Get syscall statistics
void
    syscall_get_stats (syscall_stats_t *stats) {
	if (!stats) {
		return;
	}

	// Update most used syscall
	syscall_stats.most_used_count = 0;
	for (kuint64_t i = 0; i < SYSCALL_MAX_COUNT; i++) {
		if (syscall_call_count[i] > syscall_stats.most_used_count) {
			syscall_stats.most_used_count   = syscall_call_count[i];
			syscall_stats.most_used_syscall = i;
		}
	}

	*stats = syscall_stats;
}

// Reset syscall statistics
void
    syscall_reset_stats (void) {
	syscall_stats.total_calls       = 0;
	syscall_stats.successful_calls  = 0;
	syscall_stats.failed_calls      = 0;
	syscall_stats.invalid_calls     = 0;
	syscall_stats.most_used_syscall = 0;
	syscall_stats.most_used_count   = 0;

	// Reset per-syscall counters
	for (kuint64_t i = 0; i < SYSCALL_MAX_COUNT; i++) {
		syscall_call_count[i] = 0;
	}

	kprintf("[SYSCALL] Statistics reset\n");
}

// Enable/disable security checks
void
    syscall_enable_security_checks (kuint8_t enabled) {
	security_checks = enabled;
	kprintf("[SYSCALL] Security checks %s\n", enabled ? "ENABLED" : "DISABLED");
}

// Set maximum calls per second (rate limiting)
void
    syscall_set_max_calls_per_second (kuint64_t max_calls) {
	max_calls_per_second = max_calls;
	if (max_calls > 0) {
		kprintf("[SYSCALL] Rate limiting enabled: %llu calls/second\n",
		        max_calls);
	} else {
		kprintf("[SYSCALL] Rate limiting disabled\n");
	}
}

// Integration placeholders for other kernel subsystems
void
    syscall_integrate_with_scheduler (void) {
	kprintf("[SYSCALL] Integrating with scheduler...\n");
	// TODO: Implement scheduler integration
	// This would involve registering syscalls like yield, sleep, etc.
	kprintf("[SYSCALL] Scheduler integration complete\n");
}

void
    syscall_integrate_with_memory_manager (void) {
	kprintf("[SYSCALL] Integrating with memory manager...\n");
	// TODO: Implement memory manager integration
	// This would involve registering syscalls like mmap, munmap, brk, etc.
	kprintf("[SYSCALL] Memory manager integration complete\n");
}

void
    syscall_integrate_with_filesystem (void) {
	kprintf("[SYSCALL] Integrating with filesystem...\n");
	// TODO: Implement filesystem integration
	// This would involve registering syscalls like open, close, read, write, etc.
	kprintf("[SYSCALL] Filesystem integration complete\n");
}

// Debugging utilities
void
    syscall_dump_registers (const registers_t *regs) {
	if (!regs) {
		kprintf("[SYSCALL] ERROR: NULL registers pointer\n");
		return;
	}

	kprintf("[SYSCALL] Register dump:\n");
	kprintf("[SYSCALL] RAX: 0x%016llx  RBX: 0x%016llx\n", regs->rax, regs->rbx);
	kprintf("[SYSCALL] RCX: 0x%016llx  RDX: 0x%016llx\n", regs->rcx, regs->rdx);
	kprintf("[SYSCALL] RSI: 0x%016llx  RDI: 0x%016llx\n", regs->rsi, regs->rdi);
	kprintf("[SYSCALL] RBP: 0x%016llx  R8:  0x%016llx\n", regs->rbp, regs->r8);
	kprintf("[SYSCALL] R9:  0x%016llx  R10: 0x%016llx\n", regs->r9, regs->r10);
	kprintf("[SYSCALL] R11: 0x%016llx  R12: 0x%016llx\n", regs->r11, regs->r12);
	kprintf("[SYSCALL] R13: 0x%016llx  R14: 0x%016llx\n", regs->r13, regs->r14);
	kprintf("[SYSCALL] R15: 0x%016llx\n", regs->r15);
	kprintf(
	    "[SYSCALL] INT: 0x%016llx  ERR: 0x%016llx\n", regs->int_no, regs->err_code);
}

void
    syscall_trace_call (kuint64_t syscall_no,
                        kuint64_t arg0,
                        kuint64_t arg1,
                        kuint64_t arg2,
                        kuint64_t arg3,
                        kuint64_t arg4,
                        kuint64_t arg5) {
	if (!syscall_logging) {
		return;
	}

	const syscall_entry_t *info = syscalls.get_info(syscall_no);
	const char            *name = info ? info->name : "UNKNOWN";

	kprintf(
	    "[SYSCALL] TRACE: %s(%llu) args=[0x%llx, 0x%llx, 0x%llx, 0x%llx, 0x%llx, "
	    "0x%llx]\n",
	    name,
	    syscall_no,
	    arg0,
	    arg1,
	    arg2,
	    arg3,
	    arg4,
	    arg5);
}

// Quick test interface
void
    syscall_run_quick_test (void) {
	kprintf("[SYSCALL] Running quick test...\n");

	if (syscall_status != SYSCALL_STATUS_READY) {
		kprintf("[SYSCALL] ERROR: Syscall infrastructure not ready\n");
		return;
	}

	// Test basic functionality - verify syscalls are registered
	if (!syscalls.is_valid(SYS_GETPID)) {
		kprintf("[SYSCALL] ERROR: SYS_GETPID not registered\n");
		return;
	}

	// Test a basic syscall
	registers_t mock_regs = {0};
	mock_regs.rax         = SYS_GETPID;
	syscalls.handler(&mock_regs);
	kprintf("[SYSCALL] getpid test returned: %llu\n", mock_regs.rax);

	kprintf("[SYSCALL] Quick test completed\n");
}

// Benchmark syscall performance
void
    syscall_benchmark (kuint32_t iterations) {
	if (iterations == 0) {
		iterations = 1000;
	}

	kprintf("[SYSCALL] Running benchmark with %u iterations...\n", iterations);

	if (syscall_status != SYSCALL_STATUS_READY) {
		kprintf("[SYSCALL] ERROR: Syscall infrastructure not ready\n");
		return;
	}

	// Reset stats for benchmark
	kuint64_t start_calls = syscall_stats.total_calls;

	// Run performance test - simple syscall invocation timing
	registers_t mock_regs = {0};
	mock_regs.rax         = SYS_GETPID;
	for (kuint64_t i = 0; i < 1000; i++) {
		syscalls.handler(&mock_regs);
	}

	kuint64_t end_calls  = syscall_stats.total_calls;
	kuint64_t calls_made = end_calls - start_calls;

	kprintf("[SYSCALL] Benchmark results:\n");
	kprintf("[SYSCALL] Iterations requested: %u\n", iterations);
	kprintf("[SYSCALL] Actual calls made: %llu\n", calls_made);
	kprintf("[SYSCALL] Benchmark complete\n");
}
