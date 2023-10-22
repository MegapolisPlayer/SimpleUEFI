# SimpleUEFI
a simple x64 bootable UEFI application written in C

# Functions
- boots up
- exits boot services

# Running
A Linux script is provided, on Windows just change the ISO generation part.

The simplest way is to open VirtualBox (it's free!) and set it up there.
Setting the emulation to use UEFI is difficult on QEMU and VMware is paid software.

# Required tools
- clang - LLVM compiler
- lld - LLVM linker
- xorriso or another ISO making program
- some scripting language (Batch, Bash...)

# Sources
The UEFI headers are provided by https://github.com/yoppeh/efi, edited slightly to work.
The headers are included with the example.
