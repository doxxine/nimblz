/**
 * nimblz — Minimal GATT Server (Peripheral) Example
 *
 * Equivalent to ESP32 NimBLE's "bleprph" example.
 * Exposes a single GATT service with a read/notify and a write characteristic.
 *
 * Usage: ./peripheral
 *
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

static nimblz_t *g_ctx;

/* Example 128-bit service UUID: BORA Oven Service */
static const uint8_t SVC_UUID[16] = {
    0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
    0xfe, 0xdc, 0xba, 0x09, 0x87, 0x65, 0x43, 0x21
};

/* Characteristic IDs */
#define CHAR_TEMPERATURE 0x0001
#define CHAR_COMMAND     0x0002

static void on_connect(nimblz_t *ctx, nimblz_peer_t peer, void *user)
{
    (void)user;
    printf("Peer %u connected\n", peer);

    /* Send initial temperature reading */
    uint8_t temp[] = { 0x00, 0xC8 }; /* 200 = 20.0°C */
    nimblz_gatt_notify_peer(ctx, peer, CHAR_TEMPERATURE, temp, sizeof(temp));
}

static void on_disconnect(nimblz_t *ctx, nimblz_peer_t peer, int reason, void *user)
{
    (void)ctx;
    (void)user;
    printf("Peer %u disconnected (reason=%d)\n", peer, reason);
}

static void on_command_write(
    nimblz_t *ctx, uint16_t char_id,
    const uint8_t *data, size_t len, void *user)
{
    (void)ctx;
    (void)char_id;
    (void)user;
    printf("Command received (%zu bytes):", len);
    for (size_t i = 0; i < len; i++)
        printf(" %02x", data[i]);
    printf("\n");
}

static void on_signal(int sig)
{
    (void)sig;
    printf("\nShutting down...\n");
    nimblz_stop(g_ctx);
}

int main(void)
{
    /* Initialize with defaults (auto-advertise, auto-cleanup, watchdog) */
    g_ctx = nimblz_init(NULL);
    if (!g_ctx) {
        fprintf(stderr, "Failed to initialize nimblz\n");
        return 1;
    }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    /* Register connection callbacks */
    nimblz_on_connect(g_ctx, on_connect, NULL);
    nimblz_on_disconnect(g_ctx, on_disconnect, NULL);

    /* Define GATT service */
    nimblz_char_def_t chars[] = {
        { .id = CHAR_TEMPERATURE, .props = NIMBLZ_PROP_READ | NIMBLZ_PROP_NOTIFY, .max_len = 2 },
        { .id = CHAR_COMMAND,     .props = NIMBLZ_PROP_WRITE,                     .max_len = 20 },
    };

    nimblz_service_def_t services[] = {
        {
            .uuid       = { 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
                            0xfe, 0xdc, 0xba, 0x09, 0x87, 0x65, 0x43, 0x21 },
            .chars      = chars,
            .char_count = 2,
        },
    };

    if (nimblz_gatt_server_register(g_ctx, services, 1) != 0) {
        fprintf(stderr, "Failed to register GATT server\n");
        nimblz_destroy(g_ctx);
        return 1;
    }

    /* Register write handler for command characteristic */
    nimblz_on_write(g_ctx, CHAR_COMMAND, on_command_write, NULL);

    /* Start advertising */
    nimblz_adv_params_t adv = {
        .local_name    = "BORA-Oven",
        .service_uuids = SVC_UUID,
        .uuid_count    = 1,
        .interval_ms   = 100,
    };

    if (nimblz_advertise_start(g_ctx, &adv) != 0) {
        fprintf(stderr, "Failed to start advertising\n");
        nimblz_destroy(g_ctx);
        return 1;
    }

    printf("Peripheral running. Ctrl+C to stop.\n");

    /* Run main loop (blocks until nimblz_stop) */
    nimblz_run(g_ctx);

    nimblz_destroy(g_ctx);
    printf("Clean exit.\n");
    return 0;
}
