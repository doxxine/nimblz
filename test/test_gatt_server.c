/**
 * nimblz — GATT Server registration tests.
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <nimblz/nimblz.h>

static void test_service_def_struct(void)
{
    nimblz_char_def_t chars[] = {
        { .id = 0x0001, .props = NIMBLZ_PROP_READ | NIMBLZ_PROP_NOTIFY, .max_len = 4 },
        { .id = 0x0002, .props = NIMBLZ_PROP_WRITE,                     .max_len = 20 },
    };

    nimblz_service_def_t svc;
    memset(&svc, 0, sizeof(svc));
    svc.chars      = chars;
    svc.char_count = 2;

    assert(svc.char_count == 2);
    assert(svc.chars[0].id == 0x0001);
    assert(svc.chars[0].props == (NIMBLZ_PROP_READ | NIMBLZ_PROP_NOTIFY));
    assert(svc.chars[1].max_len == 20);

    printf("  PASS: test_service_def_struct\n");
}

static void test_prop_flags(void)
{
    assert(NIMBLZ_PROP_READ   == 1);
    assert(NIMBLZ_PROP_WRITE  == 2);
    assert(NIMBLZ_PROP_NOTIFY == 4);

    uint8_t all = NIMBLZ_PROP_READ | NIMBLZ_PROP_WRITE | NIMBLZ_PROP_NOTIFY;
    assert(all == 7);

    printf("  PASS: test_prop_flags\n");
}

static void test_register_null_ctx(void)
{
    nimblz_char_def_t c = { .id = 1, .props = NIMBLZ_PROP_READ, .max_len = 1 };
    nimblz_service_def_t s = { .chars = &c, .char_count = 1 };

    int rc = nimblz_gatt_server_register(NULL, &s, 1);
    assert(rc != 0);

    printf("  PASS: test_register_null_ctx\n");
}

static void test_register_zero_count(void)
{
    nimblz_char_def_t c = { .id = 1, .props = NIMBLZ_PROP_READ, .max_len = 1 };
    nimblz_service_def_t s = { .chars = &c, .char_count = 1 };

    int rc = nimblz_gatt_server_register(NULL, &s, 0);
    assert(rc != 0);

    printf("  PASS: test_register_zero_count\n");
}

static void test_notify_null_ctx(void)
{
    uint8_t data[] = { 0x42 };
    int rc = nimblz_gatt_notify(NULL, 0x0001, data, 1);
    assert(rc != 0);

    printf("  PASS: test_notify_null_ctx\n");
}

int main(void)
{
    printf("test_gatt_server:\n");

    test_service_def_struct();
    test_prop_flags();
    test_register_null_ctx();
    test_register_zero_count();
    test_notify_null_ctx();

    printf("All GATT server tests passed.\n");
    return 0;
}
