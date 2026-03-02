/**
 * nimblz — BLE Reliability Layer for BlueZ
 * GATT Server (Peripheral) API.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_GATT_SERVER_H
#define NIMBLZ_GATT_SERVER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GATT characteristic definition */
typedef struct {
    uint16_t id;       /* characteristic ID (matches GATT emitter @gattChar) */
    uint8_t  props;    /* NIMBLZ_PROP_READ | NIMBLZ_PROP_WRITE | NIMBLZ_PROP_NOTIFY */
    size_t   max_len;  /* max value size in bytes */
} nimblz_char_def_t;

/* GATT service definition */
typedef struct {
    const uint8_t            uuid[16];    /* 128-bit service UUID */
    const nimblz_char_def_t *chars;
    size_t                   char_count;
} nimblz_service_def_t;

/**
 * Register GATT services with BlueZ via D-Bus (GattManager1).
 * The service tree is exposed as ObjectManager objects under
 * /com/bora/nimblz/service_N.
 *
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_gatt_server_register(
    nimblz_t *ctx,
    const nimblz_service_def_t *services,
    size_t count);

/**
 * Send a notification to all subscribed peers for a characteristic.
 * Returns 0 on success, -ENOENT if char_id is not registered.
 */
int nimblz_gatt_notify(
    nimblz_t *ctx, uint16_t char_id,
    const uint8_t *data, size_t len);

/**
 * Send a notification to a specific peer.
 * Returns 0 on success, -ENOENT if char_id or peer is not found.
 */
int nimblz_gatt_notify_peer(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    const uint8_t *data, size_t len);

/**
 * Register a callback for writes to a specific characteristic.
 */
void nimblz_on_write(
    nimblz_t *ctx, uint16_t char_id,
    nimblz_write_cb cb, void *user);

#ifdef __cplusplus
}
#endif

#endif /* NIMBLZ_GATT_SERVER_H */
