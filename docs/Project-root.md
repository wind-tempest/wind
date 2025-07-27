# Wind/Tempest Project Root

## Introduction

This document is talking about Wind (Operating System) and Tempest (Kernel) source code root. Explaning the structure of Wind/Tempest.

```
.github/     - GitHub Actions files for deployment
    ...
boot/        - Default bootloader (Limine) configuration
    limine/  - Limine bootloader files
    ...
disk/        - Virtual hard drive used for filesystem testing
docs/        - Documentation files
    ...
include/     - Public header files
    ...
kernel/      - Kernel source code
    arch/    - Architecture-specific files
    core/    - Core kernel functions (memory manager, entry, kernel panic, etc.)
    drivers/ - Hardware device drivers
    fs/      - Filesystem implementation
    klibc/   - Kernel libc library
    shell/   - Hardcoded shell (placeholder)
license/     - License files
    LICENSE.txt - License in plain text format
    LICENSE.md  - License in markdown format
logo/        - Logo and image files
    ...
tools/       - Build and utility tools
    ...
```