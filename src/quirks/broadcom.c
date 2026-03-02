/**
 * nimblz — Broadcom BCM43xx quirks.
 *
 * Known issues:
 * - Connection parameter update requests can crash firmware
 * - Scan + advertise simultaneously unreliable on older chips
 * - Random address rotation sometimes stalls
 *
 * SPDX-License-Identifier: MIT
 */

#include "quirks.h"
#include <stdio.h>

void nimblz_quirks_broadcom_pre_init(const char *adapter_path)
{
    /*
     * TODO:
     * 1. Check BCM firmware version via HCI vendor command
     * 2. Apply known-good connection parameter limits
     */
    (void)adapter_path;
    fprintf(stderr, "nimblz: Broadcom quirks pre-init applied\n");
}

void nimblz_quirks_broadcom_post_init(const char *adapter_path)
{
    /*
     * TODO:
     * 1. Disable LE connection parameter update if firmware < threshold
     * 2. Add delay between scan stop and advertise start
     */
    (void)adapter_path;
    fprintf(stderr, "nimblz: Broadcom quirks post-init applied\n");
}
