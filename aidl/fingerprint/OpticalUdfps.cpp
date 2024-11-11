/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OpticalUdfps.h"
#include <android-base/logging.h>
#include <fstream>
#include <string>

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

// Reads an integer value from a file path, returning a default value if reading fails.
template <typename T>
T OpticalUdfps::get(const std::string& path, const T& def) {
    T value = def;
    LOG(INFO) << "Attempting to read value from path: " << path;
    std::ifstream file(path);
    if (file.is_open()) {
        file >> value;
        file.close();
        LOG(INFO) << "Successfully read value: " << value << " from path: " << path;
    } else {
        LOG(ERROR) << "Failed to open file at path: " << path
                   << "; returning default value: " << def;
    }
    return value;
}

// Writes a value to a specified file path, logging success or failure.
template <typename T>
void OpticalUdfps::set(const std::string& path, const T& value) {
    LOG(INFO) << "Attempting to set value at path: " << path << " with value: " << value;
    std::ofstream file(path);
    if (!file) {
        LOG(ERROR) << "Failed to open file at path: " << path << "; unable to set value.";
        return;
    }
    file << value << std::endl;
    if (!file) {
        LOG(ERROR) << "Failed to write value: " << value << " to path: " << path;
    } else {
        LOG(INFO) << "Successfully wrote value: " << value << " to path: " << path;
    }
}

// Enables the fingerprint overlay mask
void OpticalUdfps::enableMask() {
    LOG(INFO) << "Enabling fingerprint green circle overlay.";
    set(FP_GREEN_CIRCLE_PATH, 1);
}

// Disables the fingerprint overlay mask
void OpticalUdfps::disableMask() {
    LOG(INFO) << "Disabling fingerprint green circle overlay.";
    set(FP_GREEN_CIRCLE_PATH, 0);
}

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
