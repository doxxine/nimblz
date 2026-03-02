/**
 * nimblz — Adapter health monitor, HCI reset on failure.
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include "dbus_util.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

void nimblz_watchdog_set_callback(
    nimblz_t *ctx, nimblz_watchdog_cb cb, void *user)
{
    if (!ctx)
        return;

    /* TODO: store in ctx->wd_cb / ctx->wd_user */
    (void)cb;
    (void)user;
    fprintf(stderr, "nimblz: watchdog callback registered\n");
}

int nimblz_watchdog_reset_adapter(nimblz_t *ctx)
{
    if (!ctx)
        return -EINVAL;

    /*
     * TODO: Full implementation will:
     * 1. Power off adapter via D-Bus
     * 2. Wait 500ms
     * 3. Power on adapter via D-Bus
     * 4. Re-apply quirks
     * 5. Fire NIMBLZ_WD_ADAPTER_RESET callback
     */
    fprintf(stderr, "nimblz: adapter reset requested\n");
    return 0;
}

bool nimblz_watchdog_is_healthy(nimblz_t *ctx)
{
    if (!ctx)
        return false;

    /*
     * TODO: Check adapter Powered property via D-Bus
     * and verify we can read adapter properties.
     */
    return true;
}
