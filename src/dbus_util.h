/**
 * nimblz — internal D-Bus helper functions.
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_DBUS_UTIL_H
#define NIMBLZ_DBUS_UTIL_H

#include <gio/gio.h>
#include <stdbool.h>

/* Get the system D-Bus connection (cached, do not unref). */
GDBusConnection *nimblz_dbus_get_system(void);

/* Call a BlueZ method synchronously. Returns the reply variant or NULL. */
GVariant *nimblz_dbus_call(
    GDBusConnection *conn,
    const char *path,
    const char *iface,
    const char *method,
    GVariant *params,
    GError **error);

/* Get a property from a BlueZ D-Bus object. */
GVariant *nimblz_dbus_get_property(
    GDBusConnection *conn,
    const char *path,
    const char *iface,
    const char *property,
    GError **error);

/* Set a property on a BlueZ D-Bus object. */
bool nimblz_dbus_set_property(
    GDBusConnection *conn,
    const char *path,
    const char *iface,
    const char *property,
    GVariant *value,
    GError **error);

/* Find the first BlueZ adapter object path (e.g. "/org/bluez/hci0"). */
char *nimblz_dbus_find_adapter(GDBusConnection *conn, const char *name);

/* Remove a D-Bus device object (stale cache cleanup). */
bool nimblz_dbus_remove_device(
    GDBusConnection *conn,
    const char *adapter_path,
    const char *device_path);

#endif /* NIMBLZ_DBUS_UTIL_H */
