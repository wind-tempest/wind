# Wind/Tempest Commit Message Style Guide

This document defines the commit message convention used in the Wind/Tempest project. Following this convention keeps the Git history clean and searchable.

## Commit Structure

```
<subsystem>: Short description (no period)

(Optional longer description explaining what changed, why it changed, and any side effects)
(Or you can add additional parts of change in description)

Signed-off-by: Name <email>
```

## Examples

```
kernel: Fix panic in division by zero

There is no check in start_kernel() and if the value is 0 then we panic. This fixes that.

Signed-off-by: Russian95CrE <russian95@tempestfoundation.org>
```

## Common Subsystems

- `kernel:` core kernel logic
- `arch:` platform-specific code (like x86, arm, etc...)
- `mm:` memory management
- `fs:` file systems
- `drivers:` generic drivers (like Video, Serial, PCI, USB, etc...)
- `boot:` bootloader and early setup
- `build:` build system (Makefiles, scripts)
- `docs:` documentation
- `debug:` debugging tools and logging
- `lib:` shared helper libraries
- `init:` system/kernel initialization
- `ci:` CI (continuous integration)

## Quick Rules

- Use imperative present tense (`fix`, `add`, `remove`, `refactor`)
- Prefix should be lowercase
- Leave a blank line between the title and body
- Do **not** end the title with a period
- Always include a `Signed-off-by` line
