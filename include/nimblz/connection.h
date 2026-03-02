/**
 * nimblz — BLE Reliability Layer for BlueZ
 * Connection management API.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_CONNECTION_H
#define NIMBLZ_CONNECTION_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Register a callback invoked when a remote device connects.
 */
void nimblz_on_connect(nimblz_t *ctx, nimblz_connect_cb cb, void *user);

/**
 * Register a callback invoked when a remote device disconnects.
 */
void nimblz_on_disconnect(nimblz_t *ctx, nimblz_disconnect_cb cb, void *user);

/**
 * Get list of currently connected peers.
 * On entry *count is the capacity of the peers array.
 * On exit *count is the number of peers written.
 * Returns 0 on success, -ENOBUFS if the array is too small.
 */
int nimblz_get_peers(nimblz_t *ctx, nimblz_peer_t *peers, size_t *count);

/**
 * Get the BLE address of a connected peer.
 * Returns 0 on success, -ENOENT if peer is not found.
 */
int nimblz_get_peer_addr(nimblz_t *ctx, nimblz_peer_t peer, nimblz_addr_t *addr);

/**
 * Get the RSSI of a connected peer.
 * Returns 0 on success, -ENOENT if peer is not found.
 */
int nimblz_get_peer_rssi(nimblz_t *ctx, nimblz_peer_t peer, int8_t *rssi);

#ifdef __cplusplus
}
#endif

#endif /* NIMBLZ_CONNECTION_H */
