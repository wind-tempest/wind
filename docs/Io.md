# IO/OUT in Wind/Tempest

## Introduction

Initially, Wind used custom IO headers. They worked but lacked flexibility and extensibility.

Starting at commit `1f89d5b7b2c89c5e18b553b0dc5e15d189701ff0` Musl LibC was ported to replace the custom IO implementation.

## Current Status

- [x] Basic IO functions implemented via Musl LibC
- [x] Support for standard input/output operations
- [ ] Ported syscall interfaces for reading/writing devices