/**
 * nimblz — Library init/destroy and GLib main loop integration.
 * SPDX-License-Identifier: MIT
 */

#include <nimblz/nimblz.h>
#include "dbus_util.h"
#include "quirks/quirks.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Internal library state */
struct nimblz {
    GDBusConnection   *conn;
    GMainLoop         *loop;
    char              *adapter_path;
    nimblz_config_t    config;

    /* Connection tracking */
    nimblz_connect_cb    on_connect;
    void                *on_connect_user;
    nimblz_disconnect_cb on_disconnect;
    void                *on_disconnect_user;

    /* Advertising state */
    bool                 adv_active;
    bool                 adv_auto_restart;
    nimblz_adv_params_t  adv_params;

    /* Watchdog state */
    nimblz_watchdog_cb   wd_cb;
    void                *wd_user;
    guint                wd_timer_id;
    bool                 wd_healthy;

    /* Peer tracking */
    nimblz_peer_t        peers[32];
    nimblz_addr_t        peer_addrs[32];
    size_t               peer_count;
    uint32_t             next_peer_id;

    /* Chip quirks */
    nimblz_chip_vendor_t chip_vendor;
};

static const nimblz_config_t default_config = {
    .adapter        = NULL,
    .auto_advertise = true,
    .auto_cleanup   = true,
    .watchdog_sec   = 30,
};

nimblz_t *nimblz_init(const nimblz_config_t *config)
{
    if (!config)
        config = &default_config;

    nimblz_t *ctx = calloc(1, sizeof(*ctx));
    if (!ctx)
        return NULL;

    ctx->config = *config;
    ctx->adv_auto_restart = config->auto_advertise;
    ctx->wd_healthy = true;
    ctx->next_peer_id = 1;

    /* Connect to system D-Bus */
    ctx->conn = nimblz_dbus_get_system();
    if (!ctx->conn) {
        free(ctx);
        return NULL;
    }

    /* Find the BLE adapter */
    ctx->adapter_path = nimblz_dbus_find_adapter(ctx->conn, config->adapter);
    if (!ctx->adapter_path) {
        fprintf(stderr, "nimblz: no BLE adapter found\n");
        g_object_unref(ctx->conn);
        free(ctx);
        return NULL;
    }

    /* Detect chip vendor and apply pre-init quirks */
    ctx->chip_vendor = nimblz_quirks_detect(ctx->adapter_path);
    nimblz_quirks_apply_pre_init(ctx->chip_vendor, ctx->adapter_path);

    /* Power on the adapter */
    GError *err = NULL;
    nimblz_dbus_set_property(
        ctx->conn, ctx->adapter_path,
        "org.bluez.Adapter1", "Powered",
        g_variant_new_boolean(TRUE), &err);
    g_clear_error(&err);

    /* Apply post-init quirks */
    nimblz_quirks_apply_post_init(ctx->chip_vendor, ctx->adapter_path);

    /* Create GLib main loop */
    ctx->loop = g_main_loop_new(NULL, FALSE);

    fprintf(stderr, "nimblz: initialized on %s\n", ctx->adapter_path);
    return ctx;
}

void nimblz_destroy(nimblz_t *ctx)
{
    if (!ctx)
        return;

    if (ctx->wd_timer_id)
        g_source_remove(ctx->wd_timer_id);

    if (ctx->loop) {
        if (g_main_loop_is_running(ctx->loop))
            g_main_loop_quit(ctx->loop);
        g_main_loop_unref(ctx->loop);
    }

    g_free(ctx->adapter_path);

    if (ctx->conn)
        g_object_unref(ctx->conn);

    free(ctx);
}

int nimblz_run(nimblz_t *ctx)
{
    if (!ctx || !ctx->loop)
        return -EINVAL;

    g_main_loop_run(ctx->loop);
    return 0;
}

int nimblz_run_once(nimblz_t *ctx, int timeout_ms)
{
    if (!ctx)
        return -EINVAL;

    GMainContext *mc = g_main_loop_get_context(ctx->loop);
    (void)timeout_ms; /* TODO: use timeout in poll */
    g_main_context_iteration(mc, timeout_ms >= 0);
    return 0;
}

void nimblz_stop(nimblz_t *ctx)
{
    if (ctx && ctx->loop)
        g_main_loop_quit(ctx->loop);
}
