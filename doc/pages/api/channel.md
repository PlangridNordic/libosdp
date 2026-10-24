---
lastUpdated: 2026-07-15
description: "LibOSDP channel abstraction — implement the send, receive, and flush callbacks that connect the OSDP library to your RS-485 or serial transport."
---
# Communication Channel

LibOSDP uses a *channel* to communicate with devices. It is up to the user to
define a channel and pass it to [`osdp_cp_setup`](/api/control-panel) or
[`osdp_pd_setup`](/api/peripheral-device). A channel is defined as:

```c
struct osdp_channel {
    void *data;
    osdp_read_fn_t recv;
    osdp_write_fn_t send;
    osdp_flush_fn_t flush;
};
```

<!-- doxygen:struct:osdp_channel:members -->

<!-- doxygen:typedef:osdp_write_fn_t -->

<!-- doxygen:typedef:osdp_read_fn_t -->

<!-- doxygen:typedef:osdp_flush_fn_t -->

<!-- doxygen:typedef:osdp_close_fn_t -->
