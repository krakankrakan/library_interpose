# Libinterposer: Linux library function call interposer

This repository contains a simple function call interposer. It can be used to route all function calls from a binary through a created library. As an example, these trampolines can be used to inject breakpoints into all function calls. Libinterposer creates a preloadable library which resolves all functions to be called via dlsym.

## Getting Started

These instructions will set up all prerequisites to use libinterposer.

### Prerequisites

Libinterposer relies on gcc. The following command installs gcc on Ubuntu:

```
sudo apt-get install gcc
```

## Usage

To use libinterposer, it has to analyze the targeted binary first in order to create the interposer library.

```
./create_interpose_lib.sh path_to_binary
```

After creation, the interposer library lies in the build folder. It can be preloaded via LD_PRELOAD into any process.

```
LD_PRELOAD=/home/krakan/git/library_interpose/build/libintercept.so path_to_binary
```

However, this technique is not very reliable. Since the preloaded library is loaded first into the process memory space, the dynamic linker will find all symbols exported by the preloaded library first (before their actual definition). But other loaded libraries also rely on the actual functions. For example, a program using calloc cannot be used with libinterposer since dlsym relies on this function. This would result in a cyclis dependency.

## Example
As an example, the test binary in the test folder can be used.
```
./create_interpose_lib.sh test/test
LD_PRELOAD=/home/krakan/git/library_interpose/build/libintercept.so test/test
mv build/symbols ./symbols
```
This will lead to the following output:
```
Initializing libinterposer...
Reading symbol file...

puts

Done.
Done resolving original function addresses for trampolines.
Hello World!
```
