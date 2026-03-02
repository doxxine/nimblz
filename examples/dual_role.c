/**
 * nimblz — Dual-Role Example (Server + Client)
 *
 * XBO use case: acts as a GATT server for the mobile app while simultaneously
 * connecting as a GATT client to temperature probes.
 *
 * Usage: ./dual_role
 *
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

static nimblz_t *g_ctx;

/* XBO Service UUID (server role — exposed to mobile app) */
static const uint8_t XBO_SVC_UUID[16] = {
    0xB0, 0x8A, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00,
    0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB
};

/* Probe Service UUID (client role — connect to temp probes) */
static const uint8_t PROBE_SVC_UUID[16] = {
    0xB0, 0x8A, 0x00, 0x02, 0x00, 0x00, 0x10, 0x00,
    0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB
};

#define CHAR_OVEN_TEMP    0x0010
#define CHAR_OVEN_MODE    0x0011
#define CHAR_PROBE_TEMP   0x0020

/* ---- Server callbacks (mobile app interaction) ---- */

static void on_mode_write(
    nimblz_t *ctx, uint16_t char_id,
    const uint8_t *data, size_t len, void *user)
{
    (void)ctx;
    (void)char_id;
    (void)user;
    if (len >= 1)
        printf("[Server] Oven mode set to: %u\n", data[0]);
}

static void on_app_connect(nimblz_t *ctx, nimblz_peer_t peer, void *user)
{
    (void)ctx;
    (void)user;
    printf("[Server] Mobile app connected (peer %u)\n", peer);
}

/* ---- Client callbacks (probe interaction) ---- */

static void on_probe_temp(
    nimblz_t *ctx, nimblz_peer_t peer, uint16_t char_id,
    const uint8_t *data, size_t len, void *user)
{
    (void)ctx;
    (void)char_id;
    (void)user;

    if (len >= 2) {
        int16_t raw = (int16_t)((data[1] << 8) | data[0]);
        printf("[Client] Probe (peer %u) temperature: %.1f°C\n", peer, raw / 10.0);

        /* Forward probe temp to all connected mobile apps */
        nimblz_gatt_notify(ctx, CHAR_OVEN_TEMP, data, len);
    }
}

static void on_probe_found(
    nimblz_t *ctx, const nimblz_addr_t *addr, int8_t rssi,
    const uint8_t *adv_data, size_t adv_len, void *user)
{
    (void)adv_data;
    (void)adv_len;
    (void)user;

    printf("[Client] Probe found (RSSI %d), connecting...\n", rssi);
    nimblz_scan_stop(ctx);

    nimblz_peer_t peer;
    if (nimblz_connect(ctx, addr, &peer) == 0) {
        printf("[Client] Probe connected (peer %u)\n", peer);
        nimblz_subscribe(ctx, peer, CHAR_PROBE_TEMP, on_probe_temp, NULL);
    }
}

/* ---- Watchdog ---- */

static void on_watchdog(nimblz_t *ctx, const char *adapter, int event, void *user)
{
    (void)ctx;
    (void)user;
    const char *evt_str = event == NIMBLZ_WD_ADAPTER_DOWN  ? "DOWN"  :
                          event == NIMBLZ_WD_ADAPTER_RESET ? "RESET" :
                          event == NIMBLZ_WD_ADAPTER_UP    ? "UP"    : "UNKNOWN";
    printf("[Watchdog] Adapter %s: %s\n", adapter, evt_str);
}

static void on_signal(int sig)
{
    (void)sig;
    printf("\nShutting down dual-role...\n");
    nimblz_stop(g_ctx);
}

int main(void)
{
    nimblz_config_t config = {
        .adapter        = NULL,
        .auto_advertise = true,   /* Restart advertising when app disconnects */
        .auto_cleanup   = true,
        .watchdog_sec   = 15,     /* Check adapter health every 15s */
    };

    g_ctx = nimblz_init(&config);
    if (!g_ctx) {
        fprintf(stderr, "Failed to initialize nimblz\n");
        return 1;
    }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    /* ---- Server setup (Peripheral role) ---- */

    nimblz_on_connect(g_ctx, on_app_connect, NULL);

    nimblz_char_def_t xbo_chars[] = {
        { .id = CHAR_OVEN_TEMP, .props = NIMBLZ_PROP_READ | NIMBLZ_PROP_NOTIFY, .max_len = 2 },
        { .id = CHAR_OVEN_MODE, .props = NIMBLZ_PROP_WRITE,                     .max_len = 1 },
    };

    nimblz_service_def_t xbo_svc = {
        .uuid       = { 0xB0, 0x8A, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00,
                        0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB },
        .chars      = xbo_chars,
        .char_count = 2,
    };

    nimblz_gatt_server_register(g_ctx, &xbo_svc, 1);
    nimblz_on_write(g_ctx, CHAR_OVEN_MODE, on_mode_write, NULL);

    nimblz_adv_params_t adv = {
        .local_name    = "BORA-XBO",
        .service_uuids = XBO_SVC_UUID,
        .uuid_count    = 1,
        .interval_ms   = 100,
    };
    nimblz_advertise_start(g_ctx, &adv);

    /* ---- Client setup (Central role) ---- */

    nimblz_scan_params_t scan = {
        .filter_uuid = PROBE_SVC_UUID,
        .duration_ms = 0,
    };
    nimblz_scan_start(g_ctx, &scan, on_probe_found, NULL);

    /* ---- Watchdog ---- */

    nimblz_watchdog_set_callback(g_ctx, on_watchdog, NULL);

    printf("Dual-role running: server (BORA-XBO) + client (probe scanner)\n");
    printf("Ctrl+C to stop.\n");

    nimblz_run(g_ctx);

    nimblz_destroy(g_ctx);
    printf("Clean exit.\n");
    return 0;
}
