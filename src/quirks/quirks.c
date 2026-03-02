/**
 * nimblz — Auto-detect chip vendor and dispatch quirks.
 * SPDX-License-Identifier: MIT
 */

#include "quirks.h"
#include <stdio.h>
#include <string.h>

nimblz_chip_vendor_t nimblz_quirks_detect(const char *adapter_path)
{
    /*
     * TODO: Full implementation will:
     * 1. Read modalias from /sys/class/bluetooth/hciN/device/modalias
     * 2. Parse USB VID:PID
     * 3. Match against known vendor IDs:
     *    - Intel:    0x8087
     *    - Broadcom: 0x0a5c
     *    - Realtek:  0x0bda
     */
    (void)adapter_path;
    fprintf(stderr, "nimblz: chip detection stub — assuming unknown vendor\n");
    return NIMBLZ_CHIP_UNKNOWN;
}

void nimblz_quirks_apply_pre_init(nimblz_chip_vendor_t vendor, const char *adapter_path)
{
    switch (vendor) {
    case NIMBLZ_CHIP_INTEL:
        nimblz_quirks_intel_pre_init(adapter_path);
        break;
    case NIMBLZ_CHIP_BROADCOM:
        nimblz_quirks_broadcom_pre_init(adapter_path);
        break;
    case NIMBLZ_CHIP_REALTEK:
        nimblz_quirks_realtek_pre_init(adapter_path);
        break;
    case NIMBLZ_CHIP_UNKNOWN:
    default:
        break;
    }
}

void nimblz_quirks_apply_post_init(nimblz_chip_vendor_t vendor, const char *adapter_path)
{
    switch (vendor) {
    case NIMBLZ_CHIP_INTEL:
        nimblz_quirks_intel_post_init(adapter_path);
        break;
    case NIMBLZ_CHIP_BROADCOM:
        nimblz_quirks_broadcom_post_init(adapter_path);
        break;
    case NIMBLZ_CHIP_REALTEK:
        nimblz_quirks_realtek_post_init(adapter_path);
        break;
    case NIMBLZ_CHIP_UNKNOWN:
    default:
        break;
    }
}
