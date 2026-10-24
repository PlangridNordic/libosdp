---
lastUpdated: 2026-07-18
description: "LibOSDP event structure reference — the osdp_event data structures and enums a peripheral device uses to report card reads, key presses, biometrics, status, and notifications to the CP."
---
# Application Events

LibOSDP exposes the following structures through `osdp.h`. All events are
carried in a single union wrapper:

<!-- doxygen:struct:osdp_event:members -->

The `type` field selects the active union member and is one of `osdp_event_type`:

<!-- doxygen:enum:osdp_event_type -->

Below are the structures for each event type. They are delivered to the CP with
[`osdp_pd_submit_event`](/api/peripheral-device).

## Key Press Event

<!-- doxygen:struct:osdp_event_keypress:members -->

<!-- doxygen:define:OSDP_EVENT_KEYPRESS_MAX_DATALEN -->

## Card Read Event

<!-- doxygen:struct:osdp_event_cardread:members -->

<!-- doxygen:enum:osdp_event_cardread_format_e -->

<!-- doxygen:define:OSDP_EVENT_CARDREAD_MAX_DATALEN -->

## Manufacturer Specific Reply Event

<!-- doxygen:struct:osdp_event_mfgrep:members -->

<!-- doxygen:define:OSDP_EVENT_MFGREP_MAX_DATALEN -->

## Manufacturer Specific Status Event

<!-- doxygen:struct:osdp_event_mfgstat:members -->

<!-- doxygen:define:OSDP_EVENT_MFGSTAT_MAX_DATALEN -->

## Biometric Events

<!-- doxygen:struct:osdp_event_bioreadr:members -->

<!-- doxygen:struct:osdp_event_biomatchr:members -->

<!-- doxygen:define:OSDP_EVENT_BIOREADR_MAX_TEMPLATE_LEN -->

A `REPLY_BIOREADR` template normally has to fit in one packet. Setting
`OSDP_FLAG_BIOREADR_MULTIPART` on **both** roles lets LibOSDP split a larger
template across fragments — a non-conforming, opt-in extension. See
[Multi-part Messages](/protocol/multipart#the-special-case-biometric-read-replies).

## Status Report Request Event

<!-- doxygen:struct:osdp_status_report:members -->

<!-- doxygen:enum:osdp_status_report_type -->

## Notifications

When enabled with `OSDP_FLAG_ENABLE_NOTIFICATION`, LibOSDP reports internal
state changes to the application through these structures.

<!-- doxygen:struct:osdp_notification:members -->

<!-- doxygen:enum:osdp_notification_type -->
