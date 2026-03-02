/**
 * nimblz — D-Bus helper functions.
 * SPDX-License-Identifier: MIT
 */

#include "dbus_util.h"
#include <stdio.h>

#define BLUEZ_SERVICE "org.bluez"

GDBusConnection *nimblz_dbus_get_system(void)
{
    GError *err = NULL;
    GDBusConnection *conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
    if (!conn) {
        fprintf(stderr, "nimblz: failed to connect to system D-Bus: %s\n",
                err ? err->message : "unknown");
        g_clear_error(&err);
    }
    return conn;
}

GVariant *nimblz_dbus_call(
    GDBusConnection *conn,
    const char *path,
    const char *iface,
    const char *method,
    GVariant *params,
    GError **error)
{
    return g_dbus_connection_call_sync(
        conn, BLUEZ_SERVICE, path, iface, method, params,
        NULL, G_DBUS_CALL_FLAGS_NONE, 5000, NULL, error);
}

GVariant *nimblz_dbus_get_property(
    GDBusConnection *conn,
    const char *path,
    const char *iface,
    const char *property,
    GError **error)
{
    GVariant *result = g_dbus_connection_call_sync(
        conn, BLUEZ_SERVICE, path,
        "org.freedesktop.DBus.Properties", "Get",
        g_variant_new("(ss)", iface, property),
        G_VARIANT_TYPE("(v)"),
        G_DBUS_CALL_FLAGS_NONE, 5000, NULL, error);

    if (!result)
        return NULL;

    GVariant *value = NULL;
    g_variant_get(result, "(v)", &value);
    g_variant_unref(result);
    return value;
}

bool nimblz_dbus_set_property(
    GDBusConnection *conn,
    const char *path,
    const char *iface,
    const char *property,
    GVariant *value,
    GError **error)
{
    GVariant *result = g_dbus_connection_call_sync(
        conn, BLUEZ_SERVICE, path,
        "org.freedesktop.DBus.Properties", "Set",
        g_variant_new("(ssv)", iface, property, value),
        NULL, G_DBUS_CALL_FLAGS_NONE, 5000, NULL, error);

    if (result) {
        g_variant_unref(result);
        return true;
    }
    return false;
}

char *nimblz_dbus_find_adapter(GDBusConnection *conn, const char *name)
{
    if (name) {
        /* If a specific adapter is requested, just build the path */
        return g_strdup_printf("/org/bluez/%s", name);
    }

    /* Auto-detect: query ObjectManager for the first adapter */
    GError *err = NULL;
    GVariant *result = g_dbus_connection_call_sync(
        conn, BLUEZ_SERVICE, "/",
        "org.freedesktop.DBus.ObjectManager", "GetManagedObjects",
        NULL, G_VARIANT_TYPE("(a{oa{sa{sv}}})"),
        G_DBUS_CALL_FLAGS_NONE, 5000, NULL, &err);

    if (!result) {
        fprintf(stderr, "nimblz: GetManagedObjects failed: %s\n",
                err ? err->message : "unknown");
        g_clear_error(&err);
        return NULL;
    }

    char *adapter_path = NULL;
    GVariantIter *iter = NULL;
    g_variant_get(result, "(a{oa{sa{sv}}})", &iter);

    const char *path;
    GVariant *ifaces;
    while (g_variant_iter_next(iter, "{&o@a{sa{sv}}}", &path, &ifaces)) {
        if (g_variant_lookup_value(ifaces, "org.bluez.Adapter1", NULL)) {
            adapter_path = g_strdup(path);
            g_variant_unref(ifaces);
            break;
        }
        g_variant_unref(ifaces);
    }

    g_variant_iter_free(iter);
    g_variant_unref(result);
    return adapter_path;
}

bool nimblz_dbus_remove_device(
    GDBusConnection *conn,
    const char *adapter_path,
    const char *device_path)
{
    GError *err = NULL;
    GVariant *result = nimblz_dbus_call(
        conn, adapter_path, "org.bluez.Adapter1", "RemoveDevice",
        g_variant_new("(o)", device_path), &err);

    if (result) {
        g_variant_unref(result);
        return true;
    }
    g_clear_error(&err);
    return false;
}
