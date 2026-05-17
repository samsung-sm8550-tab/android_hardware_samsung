/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

class OpticalUdfps {
  public:
    OpticalUdfps();
    void setGreenCircle(bool enabled);

  private:
    static constexpr const char* kFpGreenCirclePath = "/sys/class/lcd/panel/fp_green_circle";

    bool mIsOptical;
};

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
