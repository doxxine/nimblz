/**
 * nimblz — Minimal GATT Client (Central) Example
 *
 * Scans for a temperature probe peripheral, connects, subscribes to
 * notifications, and prints temperature readings.
 *
 * Usage: ./central
 *
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

static nimblz_t *g_ctx;

/* Target service UUID to filter for */
static const uint8_t TARGET_UUID[16] = {
    0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
    0xfe, 0xdc, 0xba, 0x09, 0x87, 0x65, 0x43, 0x21
};

#define CHAR_TEMPERATURE 0x0001

static void on_temperature(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    const uint8_t *data, size_t len, void *user)
{
    (void)ctx;
    (void)peer;
    (void)char_id;
    (void)user;

    if (len >= 2) {
        int16_t raw = (int16_t)((data[1] << 8) | data[0]);
        printf("Temperature: %.1f°C\n", raw / 10.0);
    }
}

static void on_scan_result(
    nimblz_t *ctx, const nimblz_addr_t *addr, int8_t rssi,
    const uint8_t *adv_data, size_t adv_len, void *user)
{
    (void)adv_data;
    (void)adv_len;
    (void)user;

    printf("Found device %02x:%02x:%02x:%02x:%02x:%02x (RSSI %d)\n",
           addr->addr[5], addr->addr[4], addr->addr[3],
           addr->addr[2], addr->addr[1], addr->addr[0], rssi);

    /* Stop scanning and connect */
    nimblz_scan_stop(ctx);

    nimblz_peer_t peer;
    if (nimblz_connect(ctx, addr, &peer) == 0) {
        printf("Connected as peer %u\n", peer);

        /* Subscribe to temperature notifications */
        nimblz_subscribe(ctx, peer, CHAR_TEMPERATURE, on_temperature, NULL);
        printf("Subscribed to temperature notifications\n");
    }
}

static void on_disconnect(nimblz_t *ctx, nimblz_peer_t peer, int reason, void *user)
{
    (void)user;
    printf("Peer %u disconnected (reason=%d), restarting scan...\n", peer, reason);

    /* Auto-reconnect by restarting scan */
    nimblz_scan_params_t params = {
        .filter_uuid = TARGET_UUID,
        .duration_ms = 0,
    };
    nimblz_scan_start(ctx, &params, on_scan_result, NULL);
}

static void on_signal(int sig)
{
    (void)sig;
    printf("\nShutting down...\n");
    nimblz_stop(g_ctx);
}

int main(void)
{
    nimblz_config_t config = {
        .adapter        = NULL,
        .auto_advertise = false,  /* Central doesn't advertise */
        .auto_cleanup   = true,
        .watchdog_sec   = 30,
    };

    g_ctx = nimblz_init(&config);
    if (!g_ctx) {
        fprintf(stderr, "Failed to initialize nimblz\n");
        return 1;
    }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    nimblz_on_disconnect(g_ctx, on_disconnect, NULL);

    /* Start scanning for the target service */
    nimblz_scan_params_t params = {
        .filter_uuid = TARGET_UUID,
        .duration_ms = 0,
    };

    if (nimblz_scan_start(g_ctx, &params, on_scan_result, NULL) != 0) {
        fprintf(stderr, "Failed to start scanning\n");
        nimblz_destroy(g_ctx);
        return 1;
    }

    printf("Central running, scanning for peripherals. Ctrl+C to stop.\n");
    nimblz_run(g_ctx);

    nimblz_destroy(g_ctx);
    printf("Clean exit.\n");
    return 0;
}
