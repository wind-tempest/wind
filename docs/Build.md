# Compiling Wind/Tempest

## Prerequisites

- QEMU (for testing in virtual environment)
- NASM (for compiling .ASM files)
- GCC (for compiling .C files)
- Make (for compiling using the automatic way)

## Building

There are several ways to build Wind/Tempest:

### Using Make

To only compile and link Wind/Tempest, run:
```bash
make
```

to compile and link Wind/Tempest and run it in QEMU, run:
```bash
make run
```

### Using Toolchain

> [!IMPORTANT]
> If any .sh file is not executable, run `chmod +x <filename>`

To compile and link Wind/Tempest using toolchain, run:

```bash
./build.sh
```

Or to just compile and link Wind/Tempest, run:
```bash
./build.sh --build-only
```

Or to just run Wind/Tempest, run:
```bash
./build.sh --run-only
```