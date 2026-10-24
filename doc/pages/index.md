---
lastUpdated: 2026-06-14
title: LibOSDP
description: A cross-platform C library implementing IEC 60839-11-5 OSDP — secure, non-blocking, and dependency-free.
---

# LibOSDP

The **Open Supervised Device Protocol (OSDP)** is an access control
communications standard developed and maintained by the
[Security Industry Association (SIA)](https://www.securityindustry.org/). It was
created to improve [interoperability](/libosdp/compatibility) among access
control and security products — letting readers, keypads, and controllers from
different vendors work together over a common, secure protocol instead of the
older, unsecured Wiegand wiring.

OSDP describes how one Control Panel (CP) talks to one or more Peripheral
Devices (PD) over a two-wire RS-485 multi-drop serial bus, with optional Secure
Channel (SC) for encrypted and authenticated communication. The same protocol
can carry secure data over any stream-based physical channel.

::: tip Read more
The [Protocol Introduction](/protocol/introduction) explains OSDP itself — how
CPs and PDs exchange messages on the wire, the packet structure, and how the
secure channel works.
:::

**LibOSDP** is a cross-platform C library implementing IEC 60839-11-5 OSDP. It
enables secure, reliable communication between Control Panels and Peripheral
Devices, and ships with Rust, Python, and C++ bindings over the core C library.

## Key Features

- **[Secure Channel](/libosdp/secure-channel)** — AES-128 encrypted communication with an optional `OSDP_FLAG_ENFORCE_SECURE` flag for stricter security.
- **Cross-Platform** — Runs on bare-metal, Linux, macOS, and Windows. No runtime memory allocation.
- **Language Bindings** — Rust, Python 3, and C++ bindings over the C library.
- **Non-Blocking Design** — Fully asynchronous. Call `osdp_cp_refresh()` / `osdp_pd_refresh()` at least once every 50 ms.
- **No External Dependencies** — Self-contained for easy cross-compilation to any target.
- **Test Coverage** — Unit tests and integration tests via PyTest.

## Getting Started

See [Build and Install](/libosdp/build-and-install) to build the library, then jump to the [C API Reference](/api/control-panel) — or the [Python bindings](/python/getting-started) — to start integrating.

Running into trouble or planning a deployment? The [debugging guide](/libosdp/debugging) covers logs and packet traces, the [compatibility list](/libosdp/compatibility) shows which devices interoperate, and the [protocol FAQ](/protocol/faq) answers common questions.

## Source

LibOSDP is open source under the [Apache-2.0 License](/license) and hosted on [GitHub](https://github.com/osdp-dev/libosdp).
