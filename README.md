<div align="center">
  <h1>Wind / Tempest</h1>
  <!-- Badges -->
  <p>
    <a href="https://github.com/wind-tempest/wind/tree/main">
      <img src="https://img.shields.io/github/languages/top/wind-tempest/wind?logo=c&label=" alt="Most used language">
    </a>
    <a href="LICENSE"><img src="https://img.shields.io/badge/license-LSL-blue.svg" alt="License: LSL1.3"></a>
    <a href="docs/Main.md"><img src="https://img.shields.io/badge/docs-available-brightgreen.svg" alt="Docs"></a>
    <img src="https://img.shields.io/badge/platform-amd64-lightgrey.svg" alt="Platform: amd64">
    <a href="https://github.com/wind-tempest/wind/actions/workflows/build.yml?branch=main">
      <img src="https://github.com/wind-tempest/wind/actions/workflows/build.yml/badge.svg?branch=main" alt="Build Status">
    </a>
  </p>
</div>

Wind/Tempest is a barebones POSIX-like OS. Built from scratch, for amd64.

## Core Features

- **Boot:** Limine + Multiboot2
- **Display:** Framebuffer text & test patterns
- **Input:** Keyboard (US only for now)
- **Shell:** `inish` (basic commands)
- **FS:** Ext2

## Documentation

See [`docs/`](docs/Main.md).

## Screenshots

<p align="center">
  <img src="share/screenshots/1.png" alt="yes" width="300">
  <img src="share/screenshots/2.png" alt="no" width="300">
</p>

## Why Wind?

- No bloat.
- No abstraction.
- No excuses.

## Philosophy

- Transparency: Inspect every line.
- Self-reliance: No hand-holding -- patches welcome.
- Simplicity: Features exist because someone coded them.

## License

Wind/Tempest is licensed under the Liberty Software License 1.3. See [LICENSE](LICENSE) for more details.
