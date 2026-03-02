/**
 * nimblz — BLE Reliability Layer for BlueZ
 * Advertising lifecycle API.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_ADVERTISE_H
#define NIMBLZ_ADVERTISE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char    *local_name;
    const uint8_t *service_uuids;  /* 128-bit UUIDs, concatenated */
    size_t         uuid_count;
    uint16_t       interval_ms;    /* default: 100 */
} nimblz_adv_params_t;

/**
 * Start BLE advertising. Registers an LEAdvertisement1 object on D-Bus.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_advertise_start(nimblz_t *ctx, const nimblz_adv_params_t *params);

/**
 * Stop advertising. Unregisters the LEAdvertisement1 object.
 * Returns 0 on success, negative errno on failure.
 */
int nimblz_advertise_stop(nimblz_t *ctx);

/**
 * Enable or disable automatic advertising restart after peer disconnect.
 */
void nimblz_advertise_set_auto(nimblz_t *ctx, bool restart_on_disconnect);

#ifdef __cplusplus
}
#endif

#endif /* NIMBLZ_ADVERTISE_H */
