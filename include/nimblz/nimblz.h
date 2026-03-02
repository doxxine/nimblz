/**
 * nimblz — BLE Reliability Layer for BlueZ
 *
 * Single-include public API. Wraps BlueZ/D-Bus and presents a NimBLE-like
 * interface with automatic reconnect, advertising lifecycle, multi-connection,
 * dual-role support, and chip-specific quirk handling.
 *
 * Usage:
 *   #include <nimblz/nimblz.h>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_H
#define NIMBLZ_H

#include "types.h"
#include "advertise.h"
#include "connection.h"
#include "gatt_server.h"
#include "gatt_client.h"
#include "watchdog.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the nimblz library.
 * Connects to D-Bus, finds the BLE adapter, and starts internal state.
 * Pass NULL for config to use defaults.
 *
 * Returns the library context, or NULL on failure.
 */
nimblz_t *nimblz_init(const nimblz_config_t *config);

/**
 * Destroy the library context and release all resources.
 * Stops advertising, disconnects peers, unregisters D-Bus objects.
 */
void nimblz_destroy(nimblz_t *ctx);

/**
 * Run the GLib main loop (blocking). Returns when nimblz_stop() is called
 * or the adapter is unrecoverably lost.
 * Returns 0 on clean exit, negative errno on error.
 */
int nimblz_run(nimblz_t *ctx);

/**
 * Run a single iteration of the main loop.
 * timeout_ms: max time to block waiting for events (-1 = indefinite).
 * Returns 0 on success, negative errno on error.
 */
int nimblz_run_once(nimblz_t *ctx, int timeout_ms);

/**
 * Signal the main loop to exit. Safe to call from callbacks or signal handlers.
 */
void nimblz_stop(nimblz_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* NIMBLZ_H */
