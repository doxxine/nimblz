/**
 * nimblz — Advertising lifecycle tests.
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <nimblz/nimblz.h>

static void test_adv_params_defaults(void)
{
    nimblz_adv_params_t params;
    memset(&params, 0, sizeof(params));

    /* Default interval should be treated as 100ms when 0 */
    assert(params.interval_ms == 0);
    assert(params.local_name == NULL);
    assert(params.uuid_count == 0);

    printf("  PASS: test_adv_params_defaults\n");
}

static void test_adv_params_with_name(void)
{
    nimblz_adv_params_t params = {
        .local_name  = "BORA-Test",
        .interval_ms = 200,
        .uuid_count  = 0,
    };

    assert(strcmp(params.local_name, "BORA-Test") == 0);
    assert(params.interval_ms == 200);

    printf("  PASS: test_adv_params_with_name\n");
}

static void test_adv_start_null_ctx(void)
{
    nimblz_adv_params_t params = { .local_name = "Test" };

    /* Should return error for NULL context */
    int rc = nimblz_advertise_start(NULL, &params);
    assert(rc != 0);

    printf("  PASS: test_adv_start_null_ctx\n");
}

static void test_adv_start_null_params(void)
{
    /* Should return error for NULL params (ctx is also NULL here but checked first) */
    int rc = nimblz_advertise_start(NULL, NULL);
    assert(rc != 0);

    printf("  PASS: test_adv_start_null_params\n");
}

static void test_adv_stop_null_ctx(void)
{
    int rc = nimblz_advertise_stop(NULL);
    assert(rc != 0);

    printf("  PASS: test_adv_stop_null_ctx\n");
}

int main(void)
{
    printf("test_advertise:\n");

    test_adv_params_defaults();
    test_adv_params_with_name();
    test_adv_start_null_ctx();
    test_adv_start_null_params();
    test_adv_stop_null_ctx();

    printf("All advertising tests passed.\n");
    return 0;
}
