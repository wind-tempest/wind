<div align="center">
  <h1>Wind/Tempest Project</h1>
  <p>
    <a href="https://github.com/wind-tempest/wind/tree/main">
      <img src="https://img.shields.io/github/languages/top/wind-tempest/wind?logo=c&label=" alt="Most used language">
    </a>
    <a href="LICENSE"><img src="https://img.shields.io/badge/license-LSL-blue.svg" alt="LSL-1.4"></a>
    <a href="docs/Main.md"><img src="https://img.shields.io/badge/docs-available-brightgreen.svg" alt="Docs"></a>
    <img src="https://img.shields.io/badge/platform-amd64-lightgrey.svg" alt="Platform: amd64">
    <a href="https://github.com/wind-tempest/wind/actions/workflows/build.yml?branch=main">
      <img src="https://github.com/wind-tempest/wind/actions/workflows/build.yml/badge.svg?branch=main" alt="Build Status">
    </a>
    <a href="https://app.codacy.com/gh/wind-tempest/wind/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade">
      <img src="https://app.codacy.com/project/badge/Grade/7e4e0ee89b95461baf590c1ab5f96b20" alt="Code Quality">
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

| Placeholder Shell and some commands for example  | Graphics Test using Framebuffer  |
| ------------------------------------------------ | -------------------------------- |
| ![1](share/screenshots/1.png)                    | ![2](share/screenshots/2.png)    |

## Philosophy

- Transparency: Inspect every line.
- Self-reliance: No hand-holding -- patches welcome.
- Simplicity: Features exist because someone coded them.

## Star History

<p align="center">
  <a href="https://star-history.com/#wind-tempest/wind&Date">
    <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=wind-tempest/wind&type=Date&theme=dark" onerror="this.src='https://api.star-history.com/svg?repos=wind-tempest/wind&type=Date'" />
  </a>
</p>

## Contributors

<a href="https://github.com/wind-tempest/wind/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=wind-tempest/wind" />
</a>

## License

Wind/Tempest is licensed under the `Liberty Software License 1.4`. See [LICENSE](LICENSE) for more details.
