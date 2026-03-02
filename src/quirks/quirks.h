/**
 * nimblz — chip quirks registry (internal).
 * SPDX-License-Identifier: MIT
 */

#ifndef NIMBLZ_QUIRKS_H
#define NIMBLZ_QUIRKS_H

#include <stdbool.h>

typedef enum {
    NIMBLZ_CHIP_UNKNOWN,
    NIMBLZ_CHIP_INTEL,
    NIMBLZ_CHIP_BROADCOM,
    NIMBLZ_CHIP_REALTEK,
} nimblz_chip_vendor_t;

/* Auto-detect the BT chip vendor from modalias / USB IDs. */
nimblz_chip_vendor_t nimblz_quirks_detect(const char *adapter_path);

/* Apply pre-init quirks for the detected vendor. */
void nimblz_quirks_apply_pre_init(nimblz_chip_vendor_t vendor, const char *adapter_path);

/* Apply post-init quirks (e.g. advertising timing workarounds). */
void nimblz_quirks_apply_post_init(nimblz_chip_vendor_t vendor, const char *adapter_path);

/* Vendor-specific functions (called by quirks.c dispatch) */
void nimblz_quirks_intel_pre_init(const char *adapter_path);
void nimblz_quirks_intel_post_init(const char *adapter_path);

void nimblz_quirks_broadcom_pre_init(const char *adapter_path);
void nimblz_quirks_broadcom_post_init(const char *adapter_path);

void nimblz_quirks_realtek_pre_init(const char *adapter_path);
void nimblz_quirks_realtek_post_init(const char *adapter_path);

#endif /* NIMBLZ_QUIRKS_H */
