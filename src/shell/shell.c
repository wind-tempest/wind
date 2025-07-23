// shell.c

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
#include "core/memory/kmemory.h"
#include "core/panic/panic.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/serial/serial.h"
#include "drivers/video/video.h"
#include "fs/ext2/ext2.h"
#include "fs/ext2/vfs/vfs.h"
#include "kerrno.h"
#include "kstdio.h"
#include "kstdlib.h"
#include "kstring.h"
#include "ktime.h"
#include "kunistd.h"
#include "shell.h"

/*
	ALL OF THIS IS HARD-CODED FOR NOW!
	I WILL CHANGE TO USER PROGRAM AFTER I GET A BETTER UNDERSTANDING OF THE
	ELF
*/

/*
	16384, 32767 AND 65535 CAUSED THE SYSTEM TO CRASH, IS RECOMMEND TO USE
	512 OR 256! Don't know why. And I will not fix now, but is required to
	fix later.
*/

#define CMD_BUFFER_SIZE 512
#define MAX_HISTORY	128
static char command_history[MAX_HISTORY][CMD_BUFFER_SIZE];
static int  history_count  = 0;
int	    input_overflow = 0;

// Command handler function type
typedef void (*command_func_t)(const char *args);

// Forward declarations for handler functions
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
    cmd_fetch (const char *args);
static void
    cmd_test_circle (const char *args);
static void
    cmd_test_square (const char *args);
static void
    cmd_sleep (const char *args);
static void
    cmd_ls (const char *args);
static void
    cmd_cat (const char *args);
static void
    cmd_fsize (const char *args);
static void
    cmd_pwd (const char *args);
static void
    cmd_history (const char *args);
static void
    cmd_test_graphics (const char *args);
static void
    cmd_time (const char *args);
static void
    cmd_panic (const char *args);
static void
    cmd_cd (const char *args);

// Command table with handler functions
static struct Command {
	const char    *name;
	const char    *description;
	const char    *category;
	command_func_t handler;
} commands[] = {
    // System commands
    {"help", "Show this help message", "System", cmd_help},
    {"clear", "Clear the screen", "System", cmd_clear},
    {"echo", "Echo a message", "System", cmd_echo},
    {"history", "Show the history of commands", "System", cmd_history},

    // System control
    {"poweroff", "Power off the OS", "Control", cmd_poweroff},
    {"reboot", "Reboot the OS", "Control", cmd_reboot},
    {"panic", "Test kernel panic (DANGEROUS!)", "Control", cmd_panic},

    // Information commands
    {"fetch", "View system information", "Info", cmd_fetch},
    {"time", "Show current date and time", "Info", cmd_time},

    // Graphics testing
    {"test_circle", "Test drawing a circle", "Graphics", cmd_test_circle},
    {"test_square", "Test drawing a square", "Graphics", cmd_test_square},
    {"test_graphics", "Test the graphics driver", "Graphics", cmd_test_graphics},

    // Filesystem commands
    {"ls", "List directory", "FS", cmd_ls},
    {"cat", "Read file from filesystem", "FS", cmd_cat},
    {"fsize", "Show file size", "FS", cmd_fsize},
    {"cd", "Change current directory", "FS", cmd_cd},
    {"pwd", "Print current directory", "FS", cmd_pwd},

    // Hardware testing
    {"sleep", "Test the HPET timer", "Hardware", cmd_sleep},
};

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

static void
    handle_command (char *cmd) {
	// Split command and arguments
	char *space = cmd;
	while ( *space && *space != ' ' )
		++space;
	char *args = KNULL;
	if ( *space == ' ' ) {
		*space = '\0';
		args   = space + 1;
		while ( *args == ' ' )
			++args; /* skip extra spaces */
	}
	for ( ksize_t i = 0; i < NUM_COMMANDS; ++i ) {
		if ( kstrcmp(cmd, commands[i].name) == 0 ) {
			commands[i].handler(args);
			return;
		}
	}
	kprintf("Unknown command: '%s'\n", cmd);
}

void
    kshell (void) {
	char cmd_buffer[CMD_BUFFER_SIZE];
	int  cmd_ptr = 0;

	kputs("Copyright (c) 2025, Russian95");
	kputs("Type 'help' for a list of commands.");

	while ( ktrue ) {
		kprintf("\n$[user-not-implemented-yet] ");
		cmd_ptr	       = 0;
		input_overflow = 0;

		while ( ktrue ) {
			char c = (char) getchar();

			if ( c == '\n' ) {
				kputchar('\n');
				if ( input_overflow ) {
					kputs("Error: command too long.");
				} else {
					cmd_buffer[cmd_ptr] = '\0';
					handle_command(cmd_buffer);

					if ( history_count < MAX_HISTORY ) {
						ksize_t len = (ksize_t) kstrlen(cmd_buffer);
						if ( len >= CMD_BUFFER_SIZE )
							len = CMD_BUFFER_SIZE - 1;
						kmemcpy(
						    command_history[history_count],
						    cmd_buffer,
						    len);
						command_history[history_count][len] = '\0';
						history_count++;
					}
				}
				break;
			}

			else if ( c == '\b' ) {
				if ( cmd_ptr > 0 ) {
					cmd_ptr--;
					kputchar('\b');
				}
			}

			else {
				if ( cmd_ptr < CMD_BUFFER_SIZE - 1 ) {
					cmd_buffer[cmd_ptr++] = c;
					kputchar(c);
				} else {
					input_overflow = 1;
					kputchar('\a');
				}
			}
		}
	}
}

static void
    cmd_clear (const char *args) {
	kuint32_t color = 0x000000;

	if ( args != KNULL && *args != '\0' ) {
		int base = 0;
		if ( args[0] == '#' ) {
			args++;
			base = 16;
		}

		kerrno = 0;
		char *end;
		long  val = kstrtol(args, &end, base);

		if ( kerrno == 0 && end != args && val >= 0 && val <= 0xFFFFFF ) {
			color = (kuint32_t) val;
		}
	}
	video_clear(color);
}

static void
    cmd_help (const char *args) {
	(void) args;
	kputs("Available commands:\n");

	// Get unique categories
	const char *categories[10];
	int	    num_categories = 0;

	for ( ksize_t i = 0; i < NUM_COMMANDS; ++i ) {
		int found = 0;
		for ( int j = 0; j < num_categories; ++j ) {
			if ( kstrcmp(commands[i].category, categories[j]) == 0 ) {
				found = 1;
				break;
			}
		}
		if ( !found ) {
			categories[num_categories++] = commands[i].category;
		}
	}

	// Display commands by category
	for ( int cat = 0; cat < num_categories; ++cat ) {
		kprintf("\n[%s]\n", categories[cat]);
		for ( ksize_t i = 0; i < NUM_COMMANDS; ++i ) {
			if ( kstrcmp(commands[i].category, categories[cat]) == 0 ) {
				kprintf(
				    "  %-12s - %s\n", commands[i].name, commands[i].description);
			}
		}
	}
}

static void
    cmd_echo (const char *args) {
	if ( args && *args ) {
		kputs(args);
	} else {
		kputs("Echo...");
		kputs("Use: echo <your message>");
	}
}

static void
    cmd_poweroff (const char *args) {
	(void) args;
	kpoweroff();
}

static void
    cmd_reboot (const char *args) {
	(void) args;
	kreboot();
}

static void
    cmd_fetch (const char *args) {
	(void) args;
	const char *ascii[] = {
	    "@                         @",
	    " @@           @       @@@@",
	    "     @@@@@@@           ",
	    "@                        @@",
	    "   @@@@@@             @ ",
	    "            @@@@@@@@     ",
	    "@                         @",
	    " @     @@@@@@@@@@      @@ "};
	char info[8][96];
	ksnprintf(info[0], sizeof(info[0]), "os:     wind");
	ksnprintf(info[1], sizeof(info[1]), "kernel: tempest");
	extern char cpu_brand_string[49];
	ksnprintf(info[2], sizeof(info[2]), "cpu:    %s", cpu_brand_string);
	if ( fb_width && fb_height && fb_bpp ) {
		ksnprintf(
		    info[3],
		    sizeof(info[3]),
		    "resolution: %ux%u %ubpp",
		    fb_width,
		    fb_height,
		    (unsigned int) fb_bpp);
	} else {
		ksnprintf(info[3], sizeof(info[3]), "resolution: unknown");
	}
	memory_stats_t stats	= memory_get_stats();
	kuint64_t      total_kb = stats.total_physical_pages * 4096 / 1024;
	kuint64_t      used_kb	= stats.used_physical_pages * 4096 / 1024;
	kuint64_t      free_kb	= stats.free_physical_pages * 4096 / 1024;
	kuint64_t      total_mb = total_kb / 1024;
	kuint64_t      used_mb	= used_kb / 1024;
	kuint64_t      free_mb	= free_kb / 1024;
	if ( total_mb >= 1 ) {
		ksnprintf(
		    info[4],
		    sizeof(info[4]),
		    "memory: %llu MB used / %llu MB total (%llu MB free)",
		    used_mb,
		    total_mb,
		    free_mb);
	} else {
		ksnprintf(
		    info[4],
		    sizeof(info[4]),
		    "memory: %llu kB used / %llu kB total (%llu kB free)",
		    used_kb,
		    total_kb,
		    free_kb);
	}
	info[5][0] = '\0';
	info[6][0] = '\0';
	info[7][0] = '\0';

	for ( int i = 0; i < 8; ++i ) {
		if ( info[i][0] )
			kprintf("%-28s  %s\n", ascii[i], info[i]);
		else
			kprintf("%s\n", ascii[i]);
	}
}

static void
    cmd_test_circle (const char *args) {
	(void) args;

	// Safety check for division by zero
	if ( fb_width == 0 || fb_height == 0 ) {
		kputs("Error: Invalid framebuffer dimensions");
		return;
	}

	kuint32_t width_center	= fb_width / 2;
	kuint32_t height_center = fb_height / 2;
	kuint32_t color		= k_u_rand32() & 0xFFFFFF;
	video_draw_circle((int) width_center, (int) (height_center), 100, color);
}

static void
    cmd_test_square (const char *args) {
	(void) args;

	// Safety check for division by zero
	if ( fb_width == 0 || fb_height == 0 ) {
		kputs("Error: Invalid framebuffer dimensions");
		return;
	}

	kuint32_t width_center	= fb_width / 2;
	kuint32_t height_center = fb_height / 2;
	kuint32_t color		= k_u_rand32() & 0xFFFFFF;
	video_draw_square((int) width_center, (int) height_center, 100, color);
}

static void
    cmd_sleep (const char *args) {
	if ( args && *args ) {
		ksleep(katoi(args));
	} else {
		kputs("Sleep...");
		kputs("Use: sleep <time in milliseconds>");
	}
}

// Callback used by kext2_list to print each entry
static void
    ls_print_cb (const char *name, kuint8_t file_type) {
	(void) file_type;
	kputs(name);
}

static void
    list_dir_path (const char *path) {
	int rc = kext2_list(path, ls_print_cb);
	if ( rc != 0 ) {
		kprintf("ls: cannot access %s (err %d)\n", path, rc);
	}
}

static void
    cmd_ls (const char *args) {
	// If no path given use current working directory
	const char *path = (args && *args) ? args : KNULL;
	char	    buf[256];
	if ( !path ) {
		vfs_getcwd(buf, sizeof(buf));
		path = buf;
	} else if ( path[0] != '/' ) {
		vfs_resolve(path, buf, sizeof(buf));
		path = buf;
	}
	list_dir_path(path);
}

// Change current working directory
static void
    cmd_cd (const char *args) {
	const char *path = (args && *args) ? args : "/";
	int	    rc	 = vfs_chdir(path);
	if ( rc != 0 ) {
		kprintf("cd: cannot access %s (err %d)\n", path, rc);
	}
}

static void
    cmd_cat (const char *args) {
	if ( !args || *args == '\0' ) {
		kputs("Usage: cat <path>");
		return;
	}
	char	    abs_path[256];
	const char *path = args;
	if ( path[0] != '/' ) {
		vfs_resolve(path, abs_path, sizeof(abs_path));
		path = abs_path;
	}
	ext2_file_t file;
	int	    rc = kext2_open(path, &file);
	if ( rc != 0 ) {
		kprintf("cat: cannot open %s (err %d)\n", path, rc);
		return;
	}
	char buf[512];
	int  read;
	while ( (read = kext2_read(&file, buf, sizeof(buf) - 1)) > 0 ) {
		buf[read] = '\0';
		kputs(buf);
	}
}

static void
    cmd_fsize (const char *args) {
	if ( !args || *args == '\0' ) {
		kputs("Usage: fsize <path>");
		return;
	}
	char	    abs_path[256];
	const char *path = args;
	if ( path[0] != '/' ) {
		vfs_resolve(path, abs_path, sizeof(abs_path));
		path = abs_path;
	}
	ext2_file_t file;
	int	    rc = kext2_open(path, &file);
	if ( rc != 0 ) {
		kprintf("fsize: cannot open %s (err %d)\n", path, rc);
		return;
	}
	kuint64_t size = (((kuint64_t) file.inode.dir_acl_or_size_high) << 32) | file.inode.size_lo;
	kprintf("%s: %llu bytes\n", args, size);
}

static void
    cmd_pwd (const char *args) {
	(void) args;
	char buf[256];
	vfs_getcwd(buf, sizeof(buf));
	kputs(buf);
}

static void
    cmd_history (const char *args) {
	(void) args;

	if ( history_count == 0 ) {
		kputs("No commands in history.");
		return;
	}

	for ( int i = 0; i < history_count; i++ ) {
		kprintf("%2d - %s\n", i + 1, command_history[i]);
	}
}

static void
    cmd_test_graphics (const char *args) {
	(void) args;

	// Safety check for division by zero
	if ( fb_width == 0 || fb_height == 0 ) {
		kputs("Error: Invalid framebuffer dimensions");
		return;
	}

	kuint8_t  circle_diff = k_u_rand32() & 0xFF;
	kuint32_t circle_x    = fb_width / 2;
	kuint32_t circle_y    = fb_height / 2;
	kuint32_t color	      = k_u_rand32() & 0xFFFFFF;
	video_draw_circle((int) circle_x, (int) circle_y, 100, color);

	// First square: left
	color		    = k_u_rand32() & 0xFFFFFF;
	kuint32_t square1_x = circle_x - circle_diff;
	kuint32_t square1_y = circle_y;
	video_draw_square((int) square1_x, (int) square1_y, 100, color);

	// Second square: right
	color		    = k_u_rand32() & 0xFFFFFF;
	kuint32_t square2_x = circle_x + circle_diff;
	kuint32_t square2_y = circle_y;
	video_draw_square((int) square2_x, (int) square2_y, 100, color);

	ksleep(5000);
}

static void
    cmd_time (const char *args) {
	(void) args;

	// Buffer for date and time strings
	char date_buffer[16];
	char time_buffer[16];

	// Retrieve date and time strings
	kget_date_string(date_buffer, sizeof(date_buffer));
	kget_time_string(time_buffer, sizeof(time_buffer));

	// Display formatted date and time
	kprintf("Date: %s\n", date_buffer);
	kprintf("Time: %s\n", time_buffer);
}

static void
    cmd_panic (const char *args) {
	if ( args == KNULL || *args == '\0' ) {
		kputs("Usage: panic <error_code>");
		kputs("Error codes: 0-15 (0=unknown, 1=div_by_zero, etc.)");
		return;
	}

	int code = katoi(args);
	if ( code < 0 || code > 15 ) {
		kputs("Error code must be between 0 and 15");
		return;
	}

	// Trigger the panic
	panic(code, KNULL);
}
