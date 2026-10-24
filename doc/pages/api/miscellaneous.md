---
lastUpdated: 2026-10-24
description: "Miscellaneous LibOSDP C API helpers — logging, versioning, status, metrics, and file transfer utilities supporting OSDP control panel and peripheral device apps."
---
# Miscellaneous

## Debugging and Diagnostics

For how to configure the logger and capture packet/data traces, see the
[Debugging](/libosdp/debugging) guide.

<!-- doxygen:enum:osdp_log_level_e -->

<!-- doxygen:function:osdp_logger_init -->

The logger can route output either through a simple `puts`-style sink or a
richer structured callback.

<!-- doxygen:typedef:osdp_log_puts_fn_t -->

<!-- doxygen:typedef:osdp_log_callback_fn_t -->

<!-- doxygen:function:osdp_set_log_callback -->

<!-- doxygen:function:osdp_get_version -->

<!-- doxygen:function:osdp_get_source_info -->

## Status and Metrics

<!-- doxygen:function:osdp_get_status_mask -->

<!-- doxygen:function:osdp_get_sc_status_mask -->

<!-- doxygen:struct:osdp_metrics:members -->

<!-- doxygen:function:osdp_get_metrics -->

## File Operations

Register a set of file callbacks so LibOSDP can drive an OSDP file transfer
against storage your application controls.

<!-- doxygen:typedef:osdp_file_open_fn_t -->

<!-- doxygen:typedef:osdp_file_read_fn_t -->

<!-- doxygen:typedef:osdp_file_write_fn_t -->

<!-- doxygen:typedef:osdp_file_close_fn_t -->

<!-- doxygen:struct:osdp_file_ops:members -->

<!-- doxygen:function:osdp_file_register_ops -->

<!-- doxygen:enum:osdp_mp_outcome -->

<!-- doxygen:function:osdp_get_file_tx_status -->
