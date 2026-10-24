---
lastUpdated: 2026-07-15
description: "LibOSDP device interoperability — tested CP/PD pairings, first-party Osprio hardware, and on-bench OSDP conformance testing with shareable compliance reports."
---
# Compatibility

This page lists commercial PD/CP devices with which the LibOSDP counterpart has
been tested (or found not to work).

The data is mostly sourced from users and should be treated with a grain of
salt — both sides of any comparison may evolve across versions and exhibit
varying behavior. Nevertheless, it is a good starting point.

| CP Device        | PD Device        | Status  | Comments                                        |
|------------------|------------------|---------|-------------------------------------------------|
| LibOSDP CP       | LibOSDP PD       | <span class="badge badge-working">Working</span> | Native support; CI ensures this always works    |
| Osprio Pro/Mini  | Osprio Pro/Mini  | <span class="badge badge-working">Working</span> | First-party; both run LibOSDP and interoperate as CP or PD |

## Osprio Devices

[Osprio Pro](https://osdp.dev/products/osprio-pro/) and
[Osprio Mini](https://osdp.dev/products/osprio-mini/) run LibOSDP and receive
first-party support. Each can operate as either a CP or a PD, so they are fully
compatible with each other and among themselves in any CP/PD pairing. That makes
them a dependable known-good reference device when bringing up or interop-testing
another vendor's CP or PD.

## Conformance Testing

Reading an interop table only tells you whether two specific devices happened to
work. To measure *how* compliant a CP or PD actually is — down to the physical
layer — the recommended tool is the
[Conformance Workspace](https://osdp.dev/products/osprio-view/conformance-workspace/)
in Osprio View. It drives a structured test plan against your device on the
bench (and in CI) and produces a detailed, shareable compliance report, so
vendors can run the tests once and exchange the report to understand how well
two devices will interoperate — without having to run every test against each
other's hardware.

If you want to report your own findings, send an email to
support@osdp.dev or open a PR updating this page on GitHub.
