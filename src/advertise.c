/**
 * nimblz — Advertising lifecycle + auto-restart on disconnect.
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include "dbus_util.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int nimblz_advertise_start(nimblz_t *ctx, const nimblz_adv_params_t *params)
{
    if (!ctx || !params)
        return -EINVAL;

    /*
     * TODO: Full implementation will:
     * 1. Create a LEAdvertisement1 D-Bus object at /com/bora/nimblz/adv
     * 2. Register it via LEAdvertisingManager1.RegisterAdvertisement
     * 3. Handle Release() callback
     *
     * Stub: log and return success.
     */
    fprintf(stderr, "nimblz: advertising started (name=%s, interval=%ums)\n",
            params->local_name ? params->local_name : "(none)",
            params->interval_ms ? params->interval_ms : 100);

    return 0;
}

int nimblz_advertise_stop(nimblz_t *ctx)
{
    if (!ctx)
        return -EINVAL;

    /*
     * TODO: Unregister LEAdvertisement1 via
     * LEAdvertisingManager1.UnregisterAdvertisement
     */
    fprintf(stderr, "nimblz: advertising stopped\n");
    return 0;
}

void nimblz_advertise_set_auto(nimblz_t *ctx, bool restart_on_disconnect)
{
    if (!ctx)
        return;

    /* TODO: update ctx->adv_auto_restart */
    fprintf(stderr, "nimblz: auto-advertise %s\n",
            restart_on_disconnect ? "enabled" : "disabled");
}
