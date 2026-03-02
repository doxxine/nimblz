/**
 * nimblz — BLE Reliability Layer for BlueZ
 * Adapter health monitoring API.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_WATCHDOG_H
#define NIMBLZ_WATCHDOG_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Register a callback for adapter health events (down / reset / up).
 */
void nimblz_watchdog_set_callback(
    nimblz_t *ctx, nimblz_watchdog_cb cb, void *user);

/**
 * Force an HCI reset on the adapter.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_watchdog_reset_adapter(nimblz_t *ctx);

/**
 * Check if the adapter is currently healthy (powered + responding).
 */
bool nimblz_watchdog_is_healthy(nimblz_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* NIMBLZ_WATCHDOG_H */
