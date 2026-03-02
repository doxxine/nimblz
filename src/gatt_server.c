/**
 * nimblz — D-Bus GATT service/characteristic registration.
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include "dbus_util.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int nimblz_gatt_server_register(
    nimblz_t *ctx,
    const nimblz_service_def_t *services,
    size_t count)
{
    if (!ctx || !services || count == 0)
        return -EINVAL;

    /*
     * TODO: Full implementation will:
     * 1. For each service, create D-Bus objects implementing:
     *    - org.bluez.GattService1
     *    - org.bluez.GattCharacteristic1 (per char)
     *    - org.freedesktop.DBus.ObjectManager (root)
     * 2. Register via GattManager1.RegisterApplication
     *
     * Stub: log and return success.
     */
    for (size_t i = 0; i < count; i++) {
        fprintf(stderr, "nimblz: registered service %zu with %zu characteristics\n",
                i, services[i].char_count);
    }

    return 0;
}

int nimblz_gatt_notify(
    nimblz_t *ctx, uint16_t char_id,
    const uint8_t *data, size_t len)
{
    if (!ctx || !data || len == 0)
        return -EINVAL;

    /*
     * TODO: emit PropertiesChanged on the GattCharacteristic1 object
     * for all subscribed peers.
     */
    (void)char_id;
    return 0;
}

int nimblz_gatt_notify_peer(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    const uint8_t *data, size_t len)
{
    if (!ctx || !data || len == 0)
        return -EINVAL;

    /* TODO: emit notification to a specific peer */
    (void)peer;
    (void)char_id;
    return 0;
}

void nimblz_on_write(
    nimblz_t *ctx, uint16_t char_id,
    nimblz_write_cb cb, void *user)
{
    if (!ctx)
        return;

    /* TODO: register per-characteristic write callback */
    (void)char_id;
    (void)cb;
    (void)user;
    fprintf(stderr, "nimblz: write callback registered for char 0x%04x\n", char_id);
}
