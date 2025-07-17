/* panic.c */

/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/acpi/acpi.h"
#include "core/registers.h"
#include "drivers/serial/serial.h"
#include "drivers/video/video.h"
#include "itoa.h"
#include "sleep.h"
#include "stdio.h"

/* Panic error codes. */
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
#define PANIC_INVALID_OPCODE      16

/* Get panic message based on error code. */
static const char *
    get_panic_message (int code)
{
        switch ( code )
        {
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
                        return "Invalid opcode";
                default:
                        return "Unknown error";
        }
}

/*
 * Indicates whether we are already inside a panic handler. This prevents
 * recursive panics (e.g., if the panic handler itself triggers a fault),
 * which otherwise lead to double-/triple-faults and a silent reboot.
 */
static bool panic_in_progress = false;

void
    panic (int code, registers_t *regs)
{
        panic_in_progress = true;
        __asm__ volatile("cli");

        bool video_ok = is_video_ready();
        video_clear(0x000000);

        const char *error_msg = get_panic_message(code);

        serial_writes("\n\nOops! Your system crashed\n");
        serial_writes("Error code: ");
        char buf[16];
        itoa(buf, buf + 14, code, 10, 0);
        serial_writes(buf);
        serial_writes("\nError: ");
        serial_writes(error_msg);
        serial_writes("\n");

        if ( video_ok )
        {
                video_puts("\n\nOops! Your system crashed\n");
        }
        if ( video_ok )
        {
                video_puts("Error code: ");
        }
        itoa(buf, buf + 14, code, 10, 0);
        video_puts(buf);
        if ( video_ok )
        {
                video_puts("\nError: ");
        }
        if ( video_ok )
        {
                video_puts(error_msg);
        }
        if ( video_ok )
        {
                video_puts("\n\n");
        }

        if ( regs )
        {
                serial_writes("Registers:\n");
                if ( video_ok )
                {
                        video_puts("Registers:\n");
                }
#define PRINT_REG(name)                                                                            \
        itoa(buf, buf + 14, (long) regs->name, 16, 0);                                             \
        serial_writes("  " #name " = 0x");                                                         \
        serial_writes(buf);                                                                        \
        serial_writes("\n");                                                                       \
        video_puts("  " #name " = 0x");                                                            \
        video_puts(buf);                                                                           \
        video_puts("\n");
                PRINT_REG(rax);
                PRINT_REG(rbx);
                PRINT_REG(rcx);
                PRINT_REG(rdx);
                PRINT_REG(rsi);
                PRINT_REG(rdi);
                PRINT_REG(rbp);
                PRINT_REG(r8);
                PRINT_REG(r9);
                PRINT_REG(r10);
                PRINT_REG(r11);
                PRINT_REG(r12);
                PRINT_REG(r13);
                PRINT_REG(r14);
                PRINT_REG(r15);
                PRINT_REG(int_no);
                PRINT_REG(err_code);
                /* Show the instruction pointer (RIP) as the stack top. */
                uint64_t rip = *((uint64_t *) ((uint8_t *) regs + sizeof(registers_t)));
                itoa(buf, buf + 14, (long) rip, 16, 0);
                serial_writes("  RIP = 0x");
                serial_writes(buf);
                serial_writes("\n");
                video_puts("  RIP = 0x");
                video_puts(buf);
                video_puts("\n");
#undef PRINT_REG
        }

        if ( video_ok )
        {
                video_puts("\nSystem will reboot in 5 seconds...\n");
        }
        serial_writes("System will reboot in 5 seconds...\n");

        for ( int i = 5; i > 0; i-- )
        {
                itoa(buf, buf + 14, i, 10, 0);
                if ( video_ok )
                {
                        video_puts("Rebooting in ");
                }
                video_puts(buf);
                if ( video_ok )
                {
                        video_puts(" seconds...\n");
                }
                serial_writes("Rebooting in ");
                serial_writes(buf);
                serial_writes(" seconds...\n");
                sleep(1000);
        }

        if ( video_ok )
        {
                video_puts("Rebooting now...\n");
        }
        serial_writes("Rebooting now...\n");

        /* Reboot the system. */
        reboot();

        /* If reboot fails, halt the system. */
        if ( video_ok )
        {
                video_puts("Reboot failed! System halted.\n");
        }
        serial_writes("Reboot failed! System halted.\n");
        while ( 1 )
        {
                __asm__ volatile("hlt");
        }
}
