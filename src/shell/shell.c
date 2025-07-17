/* shell.c */

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
#include "core/panic/panic.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/serial/serial.h"
#include "drivers/video/video.h"
#include "errno.h"
#include "fs/ext2/ext2.h"
#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "unistd.h"

/*
        ALL OF THIS IS HARD-CODED FOR NOW!

        I WILL CHANGE TO USER PROGRAM AFTER I GET A BETTER UNDERSTANDING OF THE
        KERNEL EXECUTION PROGRAM.
*/

/*
        16384, 32767 AND 65535 CAUSED THE SYSTEM TO CRASH, IS RECOMMEND TO USE
        512 OR 256
*/

#define CMD_BUFFER_SIZE 512
#define MAX_HISTORY     128
static char command_history[MAX_HISTORY][CMD_BUFFER_SIZE];
static int  history_count  = 0;
int         input_overflow = 0;

/* Command handler function type */
typedef void (*command_func_t)(const char *args);

/* Forward declarations for handler functions */
static void
    cmd_help (const char *args);
static void
    cmd_clear (const char *args);
static void
    cmd_echo (const char *args);
static void
    cmd_poweroff (const char *args);
static void
    cmd_reboot (const char *args);
static void
    cmd_vfetch (const char *args);
static void
    cmd_is_serial_available (const char *args);
static void
    cmd_dfetch (const char *args);
static void
    cmd_test_circle (const char *args);
static void
    cmd_test_square (const char *args);
static void
    cmd_sleep (const char *args);
static void
    cmd_test_caps (const char *args);
static void
    ls_cb (const char *name, uint8_t type);
static void
    cmd_lsroot (const char *args);
static void
    cmd_cat (const char *args);
static void
    cmd_history (const char *args);
static void
    cmd_test_graphics (const char *args);
static void
    cmd_time (const char *args);
static void
    cmd_panic (const char *args);

/* Command table with handler functions */
static struct Command
{
        const char    *name;
        const char    *description;
        const char    *category;
        command_func_t handler;
} commands[] = {
    /* System commands */
    {"help", "Show this help message", "System", cmd_help},
    {"clear", "Clear the screen", "System", cmd_clear},
    {"echo", "Echo a message", "System", cmd_echo},
    {"history", "Show the history of commands", "System", cmd_history},

    /* System control */
    {"poweroff", "Power off the OS", "Control", cmd_poweroff},
    {"reboot", "Reboot the OS", "Control", cmd_reboot},
    {"panic", "Test kernel panic (DANGEROUS!)", "Control", cmd_panic},

    /* Information commands */
    {"vfetch", "View system information", "Info", cmd_vfetch},
    {"is_serial_available", "Check if serial is available", "Info", cmd_is_serial_available},
    {"dfetch", "View monitor/display information", "Info", cmd_dfetch},
    {"time", "Show current date and time", "Info", cmd_time},

    /* Graphics testing */
    {"test_circle", "Test drawing a circle", "Graphics", cmd_test_circle},
    {"test_square", "Test drawing a square", "Graphics", cmd_test_square},
    {"test_graphics", "Test the graphics driver", "Graphics", cmd_test_graphics},

    /* Filesystem commands */
    {"lsroot", "List root directory of EXT2 disk", "FS", cmd_lsroot},
    {"cat", "Read file from EXT2", "FS", cmd_cat},

    /* Hardware testing */
    {"sleep", "Test the HPET timer", "Hardware", cmd_sleep},

    /* Testing commands */
    {"test_CAPS", "Test the CAPS LOCK key", "Testing", cmd_test_caps},
};

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

static void
    handle_command (char *cmd)
{
        /* Split command and arguments */
        char *space = cmd;
        while ( *space && *space != ' ' )
                ++space;
        char *args = NULL;
        if ( *space == ' ' )
        {
                *space = '\0';
                args   = space + 1;
                while ( *args == ' ' )
                        ++args; /* skip extra spaces */
        }
        for ( size_t i = 0; i < NUM_COMMANDS; ++i )
        {
                if ( strcmp(cmd, commands[i].name) == 0 )
                {
                        commands[i].handler(args);
                        return;
                }
        }
        printf("Unknown command: '%s'\n", cmd);
}

void
    shell (void)
{
        char cmd_buffer[CMD_BUFFER_SIZE];
        int  cmd_ptr = 0;

        puts("Copyright (c) 2025, Russian95");
        puts("Type 'help' for a list of commands.");

        while ( true )
        {
                printf("\n$[user-not-implemented-yet] ");
                cmd_ptr        = 0;
                input_overflow = 0;

                while ( true )
                {
                        char c = (char) getchar();

                        if ( c == '\n' )
                        {
                                putchar('\n');
                                if ( input_overflow )
                                {
                                        puts("Error: command too long.");
                                }
                                else
                                {
                                        cmd_buffer[cmd_ptr] = '\0';
                                        handle_command(cmd_buffer);

                                        if ( history_count < MAX_HISTORY )
                                        {
                                                size_t len = strlen(cmd_buffer);
                                                if ( len >= CMD_BUFFER_SIZE )
                                                        len = CMD_BUFFER_SIZE - 1;
                                                memcpy(command_history[history_count],
                                                       cmd_buffer,
                                                       len);
                                                command_history[history_count][len] = '\0';
                                                history_count++;
                                        }
                                }
                                break;
                        }

                        else if ( c == '\b' )
                        {
                                if ( cmd_ptr > 0 )
                                {
                                        cmd_ptr--;
                                        putchar('\b');
                                }
                        }

                        else
                        {
                                if ( cmd_ptr < CMD_BUFFER_SIZE - 1 )
                                {
                                        cmd_buffer[cmd_ptr++] = c;
                                        putchar(c);
                                }
                                else
                                {
                                        input_overflow = 1;
                                        putchar('\a');
                                }
                        }
                }
        }
}

static void
    cmd_clear (const char *args)
{
        uint32_t color = 0x000000;

        if ( args != NULL && *args != '\0' )
        {
                int base = 0;
                if ( args[0] == '#' )
                {
                        args++;
                        base = 16;
                }

                errno = 0;
                char *end;
                long  val = strtol(args, &end, base);

                if ( errno == 0 && end != args && val >= 0 && val <= 0xFFFFFF )
                {
                        color = (uint32_t) val;
                }
        }
        video_clear(color);
}

static void
    cmd_help (const char *args)
{
        (void) args;
        puts("Available commands:\n");

        /* Get unique categories */
        const char *categories[10];
        int         num_categories = 0;

        for ( size_t i = 0; i < NUM_COMMANDS; ++i )
        {
                int found = 0;
                for ( int j = 0; j < num_categories; ++j )
                {
                        if ( strcmp(commands[i].category, categories[j]) == 0 )
                        {
                                found = 1;
                                break;
                        }
                }
                if ( !found )
                {
                        categories[num_categories++] = commands[i].category;
                }
        }

        /* Display commands by category */
        for ( int cat = 0; cat < num_categories; ++cat )
        {
                printf("\n[%s]\n", categories[cat]);
                for ( size_t i = 0; i < NUM_COMMANDS; ++i )
                {
                        if ( strcmp(commands[i].category, categories[cat]) == 0 )
                        {
                                printf("  %-12s - %s\n", commands[i].name, commands[i].description);
                        }
                }
        }
        putchar('\n');
}

static void
    cmd_echo (const char *args)
{
        if ( args && *args )
        {
                puts(args);
        }
        else
        {
                puts("Echo...");
                puts("Use: echo <your message>");
        }
}

static void
    cmd_poweroff (const char *args)
{
        (void) args;
        poweroff();
}

static void
    cmd_reboot (const char *args)
{
        (void) args;
        reboot();
}

static void
    cmd_vfetch (const char *args)
{
        (void) args;
        putchar('\n');
        puts(" __          __  _               _ ");
        puts(" \\ \\        / / (_)             | |");
        puts("  \\ \\  /\\  / /   _   _ __     __| |");
        puts("   \\ \\/  \\/ /   | | | '_ \\   / _` |");
        puts("    \\  /\\  /    | | | | | | | (_| |");
        puts("     \\/  \\/     |_| |_| |_|  \\__,_|");
        putchar('\n');
        printf("OS: Wind\n");
        printf("Kernel: Tempest\n");
}

static void
    cmd_is_serial_available (const char *args)
{
        (void) args;
        printf("Is serial available: %s\n", serial_available() ? "Yes" : "No");
}

static void
    cmd_dfetch (const char *args)
{
        (void) args;
        putchar('\n');
        puts(":@%%%%%%%%%%%%@-");
        puts(":@           .@-");
        puts(":@           .@-");
        puts(":@           .@-");
        puts(":@............@-");
        puts(":#%%%%%@@%%%%%#:");
        puts("    =%%@@%%+    ");
        putchar('\n');
        printf("Screen width: %d\n", fb_width);
        printf("Screen height: %d\n", fb_height);
        printf("Screen pitch: %d\n", fb_pitch);
        printf("Screen BPP: %d\n", fb_bpp);
}

static void
    cmd_test_circle (const char *args)
{
        (void) args;

        /* Safety check for division by zero */
        if ( fb_width == 0 || fb_height == 0 )
        {
                puts("Error: Invalid framebuffer dimensions");
                return;
        }

        uint32_t width_center  = fb_width / 2;
        uint32_t height_center = fb_height / 2;
        uint32_t color         = u_rand32() & 0xFFFFFF;
        video_draw_circle((int) width_center, (int) (height_center), 100, color);
}

static void
    cmd_test_square (const char *args)
{
        (void) args;

        /* Safety check for division by zero */
        if ( fb_width == 0 || fb_height == 0 )
        {
                puts("Error: Invalid framebuffer dimensions");
                return;
        }

        uint32_t width_center  = fb_width / 2;
        uint32_t height_center = fb_height / 2;
        uint32_t color         = u_rand32() & 0xFFFFFF;
        video_draw_square((int) width_center, (int) height_center, 100, color);
}

static void
    cmd_sleep (const char *args)
{
        if ( args && *args )
        {
                sleep(atoi(args));
        }
        else
        {
                puts("Sleep...");
                puts("Use: sleep <time in milliseconds>");
        }
}

static void
    cmd_test_caps (const char *args)
{
        (void) args;
        puts("You tested! Shell can do caps lock now!");
}

static void
    cmd_lsroot (const char *args)
{
        (void) args;
        ext2_list_root(ls_cb);
}

static void
    cmd_cat (const char *args)
{
        if ( !args || *args == '\0' )
        {
                puts("Usage: cat <path>");
                return;
        }
        ext2_file_t file;
        int         rc = ext2_open(args, &file);
        if ( rc != 0 )
        {
                printf("cat: cannot open %s (err %d)\n", args, rc);
                return;
        }
        char buf[512];
        int  read;
        while ( (read = ext2_read(&file, buf, sizeof(buf) - 1)) > 0 )
        {
                buf[read] = '\0';
                puts(buf);
        }
}

static void
    cmd_history (const char *args)
{
        (void) args;

        if ( history_count == 0 )
        {
                puts("No commands in history.");
                return;
        }

        for ( int i = 0; i < history_count; i++ )
        {
                printf("%2d -> %s\n", i + 1, command_history[i]);
        }
}

static void
    cmd_test_graphics (const char *args)
{
        (void) args;

        /* Safety check for division by zero */
        if ( fb_width == 0 || fb_height == 0 )
        {
                puts("Error: Invalid framebuffer dimensions");
                return;
        }

        uint8_t  circle_diff = u_rand32() & 0xFF;
        uint32_t circle_x    = fb_width / 2;
        uint32_t circle_y    = fb_height / 2;
        uint32_t color       = u_rand32() & 0xFFFFFF;
        video_draw_circle((int) circle_x, (int) circle_y, 100, color);

        /* First square: left */
        color              = u_rand32() & 0xFFFFFF;
        uint32_t square1_x = circle_x - circle_diff;
        uint32_t square1_y = circle_y;
        video_draw_square((int) square1_x, (int) square1_y, 100, color);

        /* Second square: right */
        color              = u_rand32() & 0xFFFFFF;
        uint32_t square2_x = circle_x + circle_diff;
        uint32_t square2_y = circle_y;
        video_draw_square((int) square2_x, (int) square2_y, 100, color);

        sleep(5000);
}

static void
    cmd_time (const char *args)
{
        (void) args;

        /* Buffer for date and time strings */
        char date_buffer[16];
        char time_buffer[16];

        /* Retrieve date and time strings */
        get_date_string(date_buffer, sizeof(date_buffer));
        get_time_string(time_buffer, sizeof(time_buffer));

        /* Display formatted date and time */
        printf("Date: %s\n", date_buffer);
        printf("Time: %s\n", time_buffer);
}

/* Callback for ext2_list_root */
static void
    ls_cb (const char *name, uint8_t type)
{
        (void) type;
        puts(name);
}

static void
    cmd_panic (const char *args)
{
        if ( args == NULL || *args == '\0' )
        {
                puts("Usage: panic <error_code>");
                puts("Error codes: 0-15 (0=unknown, 1=div_by_zero, etc.)");
                return;
        }

        int code = atoi(args);
        if ( code < 0 || code > 15 )
        {
                puts("Error code must be between 0 and 15");
                return;
        }

        /* Trigger the panic */
        panic(code, NULL);
}
