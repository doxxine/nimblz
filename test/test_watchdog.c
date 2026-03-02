/**
 * nimblz — Watchdog tests.
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdio.h>
#include <nimblz/nimblz.h>

static void test_watchdog_events(void)
{
    assert(NIMBLZ_WD_ADAPTER_DOWN  == 1);
    assert(NIMBLZ_WD_ADAPTER_RESET == 2);
    assert(NIMBLZ_WD_ADAPTER_UP    == 3);

    printf("  PASS: test_watchdog_events\n");
}

static void test_is_healthy_null(void)
{
    bool healthy = nimblz_watchdog_is_healthy(NULL);
    assert(!healthy);

    printf("  PASS: test_is_healthy_null\n");
}

static void test_reset_null(void)
{
    int rc = nimblz_watchdog_reset_adapter(NULL);
    assert(rc != 0);

    printf("  PASS: test_reset_null\n");
}

static void test_set_callback_null(void)
{
    /* Should not crash with NULL context */
    nimblz_watchdog_set_callback(NULL, NULL, NULL);

    printf("  PASS: test_set_callback_null\n");
}

int main(void)
{
    printf("test_watchdog:\n");

    test_watchdog_events();
    test_is_healthy_null();
    test_reset_null();
    test_set_callback_null();

    printf("All watchdog tests passed.\n");
    return 0;
}
