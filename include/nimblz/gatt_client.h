/**
 * nimblz — BLE Reliability Layer for BlueZ
 * GATT Client (Central) API.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_GATT_CLIENT_H
#define NIMBLZ_GATT_CLIENT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const uint8_t *filter_uuid;  /* 128-bit UUID to filter, NULL = no filter */
    uint16_t       duration_ms;  /* 0 = scan indefinitely */
} nimblz_scan_params_t;

/**
 * Start BLE scanning. Discovery results are delivered via the callback.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_scan_start(
    nimblz_t *ctx, const nimblz_scan_params_t *params,
    nimblz_scan_cb cb, void *user);

/**
 * Stop scanning.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_scan_stop(nimblz_t *ctx);

/**
 * Connect to a remote peripheral by address.
 * On success, *out_peer receives the peer handle.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_connect(
    nimblz_t *ctx, const nimblz_addr_t *addr,
    nimblz_peer_t *out_peer);

/**
 * Disconnect from a remote peripheral.
 * Returns 0 on success, -ENOENT if peer is not found.
 */
int nimblz_disconnect(nimblz_t *ctx, nimblz_peer_t peer);

/**
 * Subscribe to notifications from a remote characteristic.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_subscribe(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    nimblz_notify_cb cb, void *user);

/**
 * Read a characteristic value from a remote peripheral.
 * On entry *len is the buffer capacity; on exit *len is bytes read.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_read(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    uint8_t *buf, size_t *len);

/**
 * Write a value to a characteristic on a remote peripheral.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_write(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* NIMBLZ_GATT_CLIENT_H */
