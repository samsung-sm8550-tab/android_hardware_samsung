/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OpticalUdfps.h"

#include <android-base/logging.h>
#include <fingerprint.sysprop.h>

#include <fstream>

using namespace ::android::fingerprint::samsung;

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

OpticalUdfps::OpticalUdfps() {
    auto type = FingerprintHalProperties::type();
    mIsOptical = type.has_value() && type.value() == "udfps_optical";
}

void OpticalUdfps::setGreenCircle(bool enabled) {
    if (!mIsOptical) {
        return;
    }
    std::ofstream file(kFpGreenCirclePath);
    if (!file.is_open()) {
        return;
    }
    file << enabled;
}

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
