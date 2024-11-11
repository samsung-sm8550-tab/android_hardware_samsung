/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AIDL_ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_OPTICALUDFPS_H
#define AIDL_ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_OPTICALUDFPS_H

# pragma once

#include <android-base/logging.h>
#include <string>

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

class OpticalUdfps {
  public:
    // Enables the fingerprint overlay mask with brightness adjustment.
    void enableMask();

    // Disables the fingerprint overlay mask.
    void disableMask();

  private:
    // Template function to read a value from a file. Returns default if reading fails.
    template <typename T>
    T get(const std::string& path, const T& def);

    // Template function to write a value to a file, with logging of success or failure.
    template <typename T>
    void set(const std::string& path, const T& value);

    // File paths for device brightness and mask configurations
    static constexpr const char* FP_GREEN_CIRCLE_PATH = "/sys/class/lcd/panel/fp_green_circle";

    // Default error return value
    static constexpr float RETURN_ERROR_F = -1.0f;
};

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl

#endif  // AIDL_ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_OPTICALUDFPS_H
