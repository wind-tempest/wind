// ata.c

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

#include "ata.h"
#include "io/ksignal/kinw.h"
#include "ksignal.h"
#include "kstddef.h"

#define ATA_PRIMARY_IO	 0x1F0
#define ATA_PRIMARY_CTRL 0x3F6

// I/O registers (offsets from base)
#define ATA_REG_DATA	 0x00
#define ATA_REG_ERROR	 0x01
#define ATA_REG_SECCOUNT 0x02
#define ATA_REG_LBA_LO	 0x03
#define ATA_REG_LBA_MID	 0x04
#define ATA_REG_LBA_HI	 0x05
#define ATA_REG_DRIVE	 0x06
#define ATA_REG_STATUS	 0x07
#define ATA_REG_COMMAND	 0x07

// Control register
#define ATA_REG_CONTROL	 0x02 /* base 0x3F6 + 2 == 0x3F8, but 0x3F6 often used */
#define ATA_CONTROL_nIEN 0x02

// Status bits
#define ATA_SR_BSY  0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DRQ  0x08
#define ATA_SR_ERR  0x01

#define ATA_CMD_READ_SECTORS 0x20

static inline void
    io_wait (void) {
	// 400ns delay by reading alternate-status port 4 times
	for ( int i = 0; i < 4; i++ )
		(void) kinb(ATA_PRIMARY_CTRL);
}

// Poll until BSY=0 and either DRQ=1 or ERR=1. Return 0 on ready, -1 on error
static int
    ata_poll (void) {
	kuint8_t status;
	// Initial delay
	io_wait();
	while ( (status = kinb(ATA_PRIMARY_IO + ATA_REG_STATUS)) & ATA_SR_BSY )
		;
	if ( status & ATA_SR_ERR )
		return -1;
	if ( !(status & ATA_SR_DRQ) )
		return -1;
	return 0;
}

int
    ata_pio_read (kuint64_t lba, kuint32_t count, void *buf) {
	if ( count == 0 || buf == KNULL )
		return -1;
	if ( lba > 0x0FFFFFFF ) /* 28-bit limit */
		return -1;

	kuint8_t *ptr = (kuint8_t *) buf;
	for ( kuint32_t i = 0; i < count; i++ ) {
		kuint32_t cur_lba = (kuint32_t) (lba + i);

		// Select drive + LBA bits 24-27
		koutb(ATA_PRIMARY_IO + ATA_REG_DRIVE, 0xE0 | ((cur_lba >> 24) & 0x0F));

		// Sector count
		koutb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT, 1);
		// LBA low/mid/high
		koutb(ATA_PRIMARY_IO + ATA_REG_LBA_LO, (kuint8_t) cur_lba);
		koutb(ATA_PRIMARY_IO + ATA_REG_LBA_MID, (kuint8_t) (cur_lba >> 8));
		koutb(ATA_PRIMARY_IO + ATA_REG_LBA_HI, (kuint8_t) (cur_lba >> 16));

		// Issue READ SECTORS command
		koutb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_SECTORS);

		if ( ata_poll() != 0 )
			return -1;

		// Transfer 256 words (512 bytes)
		for ( int w = 0; w < 256; w++ ) {
			kuint16_t data = kinw(ATA_PRIMARY_IO + ATA_REG_DATA);
			ptr[0]	       = (kuint8_t) data;
			ptr[1]	       = (kuint8_t) (data >> 8);
			ptr += 2;
		}
	}
	return 0;
}
