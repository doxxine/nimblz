/**
 * nimblz — Multi-peer tracking, stale cache cleanup, disconnect handling.
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include "dbus_util.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

void nimblz_on_connect(nimblz_t *ctx, nimblz_connect_cb cb, void *user)
{
    if (!ctx)
        return;

    /* TODO: store callback in ctx internal state */
    (void)cb;
    (void)user;
    fprintf(stderr, "nimblz: connect callback registered\n");
}

void nimblz_on_disconnect(nimblz_t *ctx, nimblz_disconnect_cb cb, void *user)
{
    if (!ctx)
        return;

    /* TODO: store callback in ctx internal state */
    (void)cb;
    (void)user;
    fprintf(stderr, "nimblz: disconnect callback registered\n");
}

int nimblz_get_peers(nimblz_t *ctx, nimblz_peer_t *peers, size_t *count)
{
    if (!ctx || !peers || !count)
        return -EINVAL;

    /* TODO: copy from internal peer list */
    *count = 0;
    return 0;
}

int nimblz_get_peer_addr(nimblz_t *ctx, nimblz_peer_t peer, nimblz_addr_t *addr)
{
    if (!ctx || !addr)
        return -EINVAL;

    /* TODO: look up peer in internal list */
    (void)peer;
    return -ENOENT;
}

int nimblz_get_peer_rssi(nimblz_t *ctx, nimblz_peer_t peer, int8_t *rssi)
{
    if (!ctx || !rssi)
        return -EINVAL;

    /*
     * TODO: read RSSI from BlueZ Device1 property.
     * BlueZ path: /org/bluez/hci0/dev_XX_XX_XX_XX_XX_XX → "RSSI"
     */
    (void)peer;
    return -ENOENT;
}
