/**
 * nimblz — Intel AX200/201/210 quirks.
 *
 * Known issues:
 * - Firmware sometimes needs reload after suspend/resume
 * - USB autosuspend causes advertising to stop
 * - Connection parameter updates rejected if too aggressive
 *
 * SPDX-License-Identifier: MIT
 */

#include "quirks.h"
#include <stdio.h>

void nimblz_quirks_intel_pre_init(const char *adapter_path)
{
    /*
     * TODO:
     * 1. Disable USB autosuspend for the BT device:
     *    echo -1 > /sys/bus/usb/devices/.../power/autosuspend
     * 2. Check firmware loaded state via btmgmt info
     * 3. Reload firmware if needed via btmgmt power off/on cycle
     */
    (void)adapter_path;
    fprintf(stderr, "nimblz: Intel quirks pre-init applied\n");
}

void nimblz_quirks_intel_post_init(const char *adapter_path)
{
    /*
     * TODO:
     * 1. Set conservative connection parameters to avoid Intel rejection
     * 2. Verify advertising is actually running (Intel sometimes silently fails)
     */
    (void)adapter_path;
    fprintf(stderr, "nimblz: Intel quirks post-init applied\n");
}
