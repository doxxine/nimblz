/**
 * nimblz — Mock GDBusConnection for unit testing.
 *
 * Provides stub implementations of D-Bus functions so tests can run
 * without a real system bus or BlueZ daemon.
 *
 * SPDX-License-Identifier: MIT
 */

#include <gio/gio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
 * Mock state: tracks calls made by the library under test.
 * Tests can inspect this to verify correct D-Bus interaction.
 */

static int mock_call_count;
static char mock_last_method[128];
static char mock_last_path[256];
static bool mock_adapter_powered;

void mock_dbus_reset(void)
{
    mock_call_count = 0;
    mock_last_method[0] = '\0';
    mock_last_path[0] = '\0';
    mock_adapter_powered = false;
}

int mock_dbus_get_call_count(void)
{
    return mock_call_count;
}

const char *mock_dbus_get_last_method(void)
{
    return mock_last_method;
}

const char *mock_dbus_get_last_path(void)
{
    return mock_last_path;
}

bool mock_dbus_is_adapter_powered(void)
{
    return mock_adapter_powered;
}

void mock_dbus_set_adapter_powered(bool powered)
{
    mock_adapter_powered = powered;
}

/*
 * Note: In a full mock implementation, we would override the GDBus
 * functions using linker tricks (--wrap) or a mock framework.
 * For now this provides the infrastructure that tests can build on.
 */
