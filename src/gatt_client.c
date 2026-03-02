/**
 * nimblz — Scan, connect, subscribe, read from remote peripherals.
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include "dbus_util.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int nimblz_scan_start(
    nimblz_t *ctx, const nimblz_scan_params_t *params,
    nimblz_scan_cb cb, void *user)
{
    if (!ctx || !cb)
        return -EINVAL;

    /*
     * TODO: Full implementation will:
     * 1. Set discovery filter via Adapter1.SetDiscoveryFilter
     * 2. Call Adapter1.StartDiscovery
     * 3. Subscribe to InterfacesAdded signals for Device1 objects
     * 4. Deliver results via cb
     */
    (void)params;
    (void)user;
    fprintf(stderr, "nimblz: scanning started\n");
    return 0;
}

int nimblz_scan_stop(nimblz_t *ctx)
{
    if (!ctx)
        return -EINVAL;

    /* TODO: Call Adapter1.StopDiscovery */
    fprintf(stderr, "nimblz: scanning stopped\n");
    return 0;
}

int nimblz_connect(
    nimblz_t *ctx, const nimblz_addr_t *addr,
    nimblz_peer_t *out_peer)
{
    if (!ctx || !addr || !out_peer)
        return -EINVAL;

    /*
     * TODO: Full implementation will:
     * 1. Build Device1 path from address
     * 2. Call Device1.Connect
     * 3. Wait for ServicesResolved = true
     * 4. Assign peer handle, add to peer list
     */
    fprintf(stderr, "nimblz: connect to %02x:%02x:%02x:%02x:%02x:%02x\n",
            addr->addr[5], addr->addr[4], addr->addr[3],
            addr->addr[2], addr->addr[1], addr->addr[0]);
    *out_peer = 0;
    return 0;
}

int nimblz_disconnect(nimblz_t *ctx, nimblz_peer_t peer)
{
    if (!ctx)
        return -EINVAL;

    /* TODO: Call Device1.Disconnect for the peer */
    (void)peer;
    return 0;
}

int nimblz_subscribe(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    nimblz_notify_cb cb, void *user)
{
    if (!ctx || !cb)
        return -EINVAL;

    /*
     * TODO: Find GattCharacteristic1 path, call StartNotify,
     * subscribe to PropertiesChanged signal.
     */
    (void)peer;
    (void)char_id;
    (void)user;
    return 0;
}

int nimblz_read(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    uint8_t *buf, size_t *len)
{
    if (!ctx || !buf || !len)
        return -EINVAL;

    /* TODO: Call GattCharacteristic1.ReadValue */
    (void)peer;
    (void)char_id;
    *len = 0;
    return 0;
}

int nimblz_write(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    const uint8_t *data, size_t len)
{
    if (!ctx || !data || len == 0)
        return -EINVAL;

    /* TODO: Call GattCharacteristic1.WriteValue */
    (void)peer;
    (void)char_id;
    return 0;
}
