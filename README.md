<div align="center">

  <img src="logo/wide.png" alt="Wind Logo (Wide Version)" width="400">
  <h1>Wind / Tempest</h1>
  <!-- Badges -->
  <p>
    <a href="https://github.com/wind-tempest/wind/tree/main/src">
      <img src="https://img.shields.io/github/languages/top/wind-tempest/wind?logo=c&label=" alt="Most used language">
    </a>
    <a href="LICENSE"><img src="https://img.shields.io/badge/license-LSL-blue.svg" alt="License: LSL v1.0"></a>
    <a href="docs/Main.md"><img src="https://img.shields.io/badge/docs-available-brightgreen.svg" alt="Docs"></a>
    <img src="https://img.shields.io/badge/platform-x86__64-lightgrey.svg" alt="Platform: x86_64">
    <a href="https://github.com/wind-tempest/wind/actions/workflows/build.yml?branch=main">
      <img src="https://github.com/wind-tempest/wind/actions/workflows/build.yml/badge.svg?branch=main" alt="Build Status">
    </a>
  </p>
</div>

> A minimal OS in POSIX standards, built for modern machines, built from scratch, built to last.

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

This OS/Kernel is released under the Liberty Software License (v1.1). Please see [LICENSE](LICENSE) or [LICENSE.txt](license/LICENSE.txt).