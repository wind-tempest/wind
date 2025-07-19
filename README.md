![Wind Logo (Wide Version)](logo/wind_wide.png)

# Wind / Tempest

<!-- Badges -->
<p align="center">
  <a href="https://github.com/wind-tempest/wind/tree/main/src">
    <img src="https://img.shields.io/github/languages/top/wind-tempest/wind?logo=c&label=" alt="Most used language">
  </a>
  <a href="https://www.gnu.org/licenses/agpl-3.0"><img src="https://img.shields.io/badge/license-AGPLv3-blue.svg" alt="License: AGPL v3"></a>
  <a href="docs/Main.md"><img src="https://img.shields.io/badge/docs-available-brightgreen.svg" alt="Docs"></a>
  <img src="https://img.shields.io/badge/platform-x86__64-lightgrey.svg" alt="Platform: x86_64">
  <a href="https://github.com/wind-tempest/wind/actions/workflows/build.yml?branch=main">
    <img src="https://github.com/wind-tempest/wind/actions/workflows/build.yml/badge.svg?branch=main" alt="Build Status">
  </a>
</p>


> A minimal OS for modern machines, built from scratch, built to last.

**Wind** is the operating system.  
**Tempest** is the kernel powering it.

It runs on `x86_64`, has no legacy baggage, and doesn't pretend to be user-friendly.  
You break it, you fix it.

## Quick Facts

- **Boot:** Limine + Multiboot2  
- **Display:** Framebuffer text & test patterns  
- **Input:** Keyboard (US only for now)  
- **Shell:** `inish` (basic commands)
- **FS:** Ext2

## Docs

All docs live in [`docs/`](docs/Main.md). Read them first.

## Why Wind?

- **No bloat.** Just what you need.  
- **No magic.** Every bit is visible and hackable.  
- **No excuses.** Missing features? Write the code.

## Build

Requirements:

- `x86_64-elf` cross-compiler  
- `nasm`

## Run

Supported targets:

- QEMU
- Bochs
- Real hardware (good luck)

Must support Multiboot2/Limine.

## Philosophy

- Transparency: Inspect every line.
- Self-reliance: No hand-holding -- patches welcome.
- Simplicity: Features exist because someone coded them.

## License

This OS/Kernel is released under the GNU Affero General Public License (v3). Please see [LICENSE](LICENSE.md).