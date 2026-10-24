---
lastUpdated: 2026-07-15
description: "Cross-compile LibOSDP for embedded and bare-metal targets using the lean Make build, with toolchain, platform-porting, and dependency guidance for constrained systems."
---
# Cross Compiling

LibOSDP is written in C and does not depend on any other libraries. You can
compile it to practically any platform (including Windows). Follow the
cross-compilation best practice for your platform. This document gives some
ideas on how to do this but is not exhaustive. For the standard (non-cross)
build, see [Build and Install](/libosdp/build-and-install) first.

## Using CMake

LibOSDP can be compiled with a cross-compiler by passing a toolchain file to
CMake via `-DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-file.cmake`.

If your toolchain is installed at `/opt/toolchain/armv8l-linux-gnueabihf/` and
the sysroot is at `/opt/toolchain/armv8l-linux-gnueabihf/sysroot`, the
`toolchain-file.cmake` should look like this:

```cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# specify the cross compiler and sysroot
set(TOOLCHAIN_INST_PATH /opt/toolchain/armv8l-linux-gnueabihf)
set(CMAKE_C_COMPILER    ${TOOLCHAIN_INST_PATH}/bin/armv8l-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER  ${TOOLCHAIN_INST_PATH}/bin/armv8l-linux-gnueabihf-g++)
set(CMAKE_SYSROOT       ${TOOLCHAIN_INST_PATH}/sysroot)

# don't search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search for libraries and headers in the target directories only
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```

Place the toolchain file in a common path (such as where the toolchain is
installed) and reference it from your build directory:

```sh
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/opt/toolchain/armv8l-linux-gnueabihf/toolchain-file.cmake ..
make
```

## Using Make

Use the `--cross-compile` flag in `configure.sh` and then invoke make:

```sh
./configure.sh --cross-compile arm-none-
make
make DESTDIR=/opt/arm-none-sysroot/ install
```

## Porting to a New Platform

LibOSDP itself is portable C, but it relies on a small set of platform
primitives supplied by its `utils/` submodule (c-utils). On mainstream targets
(Linux, macOS, Windows) and the built-in platforms (Arduino, Zephyr, Mynewt)
these are provided automatically. For an unrecognized or bare-metal target, you
provide them yourself:

- **Entropy** — `rand_u32()`. This backs the secure channel nonces, so it must
  be a cryptographically strong source (see
  [Random Number Generation](/libosdp/secure-channel#random-number-generation)).
  The default is `__weak`, so you can override it with your MCU's hardware TRNG.
  On unknown bare-metal targets the symbol is left undefined on purpose, so a
  missing entropy source becomes a link error rather than a silent weak PRNG.
- **Time** — a monotonic clock (`gettimeofday()` / `usec_now()` / `millis_now()`),
  used for OSDP's protocol timing. Wire this to your platform's tick source.
- **Logging** — an output hook (a `puts`-style function) passed to the logger,
  plus a working `printf` from your C runtime.
- **Endianness** — auto-detected in `utils/byteorder.h`; configure it if your
  target is not recognized.

The declarations and platform-detection macros live in
`utils/include/utils/utils.h`; the reference implementations you extend or
override are in `utils/src/utils.c`.
