/**
 * nimblz — Realtek RTL8761B quirks.
 *
 * Known issues:
 * - Advertising restart after disconnect needs a delay (200-500ms)
 * - Discovery sometimes returns stale results after adapter reset
 * - Random address changes can fail silently
 *
 * SPDX-License-Identifier: MIT
 */

#include "quirks.h"
#include <stdio.h>

void nimblz_quirks_realtek_pre_init(const char *adapter_path)
{
    /*
     * TODO:
     * 1. Flush stale device cache on adapter path
     * 2. Verify Realtek firmware is loaded
     */
    (void)adapter_path;
    fprintf(stderr, "nimblz: Realtek quirks pre-init applied\n");
}

void nimblz_quirks_realtek_post_init(const char *adapter_path)
{
    /*
     * TODO:
     * 1. Configure advertising restart delay (300ms recommended)
     * 2. Set discovery timeout to avoid stale results
     */
    (void)adapter_path;
    fprintf(stderr, "nimblz: Realtek quirks post-init applied\n");
}
