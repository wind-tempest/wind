# Wind/Tempest - Source code

This is the folder of the kernel source code of Tempest.  Here you will find
all the (horrible) source code I made in this kernel.  The current source code
project root is:

```
arch/		- Architecture-specific code
debug/		- Debugging tools and utilities
drivers/	- Basic drivers for kernel
fs/		- File system support
kern/		- Core of the kernel, like `mm` and `init`
lib/		- Basic kernel-only LibC (may be removed in case the project expands)
shell/		- Placeholder shell for testing (will be removed for user-space testing in future)
```
Keep in mind that this folder (sys/) contains kernel-space code only and NOT the user-space
code.