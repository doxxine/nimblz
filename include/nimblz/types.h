/**
 * nimblz — BLE Reliability Layer for BlueZ
 * Common types and callback signatures.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_TYPES_H
#define NIMBLZ_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque library context */
typedef struct nimblz nimblz_t;

/* Peer handle (analogous to NimBLE conn_handle) */
typedef uint32_t nimblz_peer_t;

/* BLE address */
typedef struct {
    uint8_t addr[6];
    uint8_t type;  /* 0 = public, 1 = random */
} nimblz_addr_t;

/* Library configuration */
typedef struct {
    const char *adapter;        /* "hci0" — NULL = auto-detect first adapter   */
    bool        auto_advertise; /* restart advertising on disconnect (def: true) */
    bool        auto_cleanup;   /* remove stale D-Bus devices (def: true)       */
    uint16_t    watchdog_sec;   /* adapter health-check interval, 0=off (def: 30) */
} nimblz_config_t;

/* GATT characteristic property flags */
#define NIMBLZ_PROP_READ   (1 << 0)
#define NIMBLZ_PROP_WRITE  (1 << 1)
#define NIMBLZ_PROP_NOTIFY (1 << 2)

/* Watchdog events */
#define NIMBLZ_WD_ADAPTER_DOWN  1
#define NIMBLZ_WD_ADAPTER_RESET 2
#define NIMBLZ_WD_ADAPTER_UP    3

/* ---- Callback signatures ------------------------------------------------ */

typedef void (*nimblz_connect_cb)(
    nimblz_t *ctx, nimblz_peer_t peer, void *user);

typedef void (*nimblz_disconnect_cb)(
    nimblz_t *ctx, nimblz_peer_t peer, int reason, void *user);

typedef void (*nimblz_write_cb)(
    nimblz_t *ctx, uint16_t char_id,
    const uint8_t *data, size_t len, void *user);

typedef void (*nimblz_notify_cb)(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    const uint8_t *data, size_t len, void *user);

typedef void (*nimblz_scan_cb)(
    nimblz_t *ctx, const nimblz_addr_t *addr, int8_t rssi,
    const uint8_t *adv_data, size_t adv_len, void *user);

typedef void (*nimblz_watchdog_cb)(
    nimblz_t *ctx, const char *adapter, int event, void *user);

#ifdef __cplusplus
}
#endif

#endif /* NIMBLZ_TYPES_H */
