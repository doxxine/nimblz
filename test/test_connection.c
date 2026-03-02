/**
 * nimblz — Connection tracking tests.
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <nimblz/nimblz.h>

static void test_addr_struct(void)
{
    nimblz_addr_t addr;
    memset(&addr, 0, sizeof(addr));

    addr.addr[0] = 0xAA;
    addr.addr[5] = 0xBB;
    addr.type = 1; /* random */

    assert(addr.addr[0] == 0xAA);
    assert(addr.addr[5] == 0xBB);
    assert(addr.type == 1);

    printf("  PASS: test_addr_struct\n");
}

static void test_get_peers_null(void)
{
    int rc = nimblz_get_peers(NULL, NULL, NULL);
    assert(rc != 0);

    printf("  PASS: test_get_peers_null\n");
}

static void test_get_peer_addr_null(void)
{
    int rc = nimblz_get_peer_addr(NULL, 0, NULL);
    assert(rc != 0);

    printf("  PASS: test_get_peer_addr_null\n");
}

static void test_get_peer_rssi_null(void)
{
    int rc = nimblz_get_peer_rssi(NULL, 0, NULL);
    assert(rc != 0);

    printf("  PASS: test_get_peer_rssi_null\n");
}

static void test_on_connect_null_ctx(void)
{
    /* Should not crash with NULL context */
    nimblz_on_connect(NULL, NULL, NULL);

    printf("  PASS: test_on_connect_null_ctx\n");
}

static void test_on_disconnect_null_ctx(void)
{
    nimblz_on_disconnect(NULL, NULL, NULL);

    printf("  PASS: test_on_disconnect_null_ctx\n");
}

int main(void)
{
    printf("test_connection:\n");

    test_addr_struct();
    test_get_peers_null();
    test_get_peer_addr_null();
    test_get_peer_rssi_null();
    test_on_connect_null_ctx();
    test_on_disconnect_null_ctx();

    printf("All connection tests passed.\n");
    return 0;
}
