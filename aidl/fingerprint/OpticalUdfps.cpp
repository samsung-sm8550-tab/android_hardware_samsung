#include "OpticalUdfps.h"
#include <fstream>
#include <string>
#include <android-base/logging.h>
#include <android-base/file.h>
#include <android-base/properties.h>

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

template <typename T>
T OpticalUdfps::get(const std::string& path, const T& def) {
    T value = def;
    std::ifstream file(path);
    if (file.is_open()) {
        file >> value;
        file.close();
        LOG(INFO) << "Read value: " << value << " from path: " << path;
    } else {
        LOG(WARNING) << "Failed to read from path: " << path << "; returning default value: " << def;
    }
    return value;
}

template <typename T>
void OpticalUdfps::set(const std::string& path, const T& value) {
    std::ofstream file(path);
    if (!file) {
        LOG(ERROR) << "Failed to write to path: " << path << " with value: " << value;
        return;
    }
    file << value << std::endl;
    LOG(INFO) << "Wrote value: " << value << " to path: " << path;
}

bool OpticalUdfps::fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

int OpticalUdfps::readBrightness() {
    return get<int>(DISPLAY_BRIGHTNESS_PATH, -1);
}

int OpticalUdfps::calculateMaskBrightness(int displayBrightness) {
    if (displayBrightness < 0) {
        LOG(WARNING) << "Invalid display brightness; defaulting mask brightness to 319.";
        return 319;
    }
    return static_cast<int>(displayBrightness * 0.607);
}

void OpticalUdfps::enableMask() {
    int displayBrightness = readBrightness();
    if (displayBrightness < 0) {
        LOG(ERROR) << "Invalid display brightness; aborting mask enable.";
        return;
    }

    int maskBrightness = calculateMaskBrightness(displayBrightness);
    set(MASK_BRIGHTNESS_PATH, maskBrightness);
    set(ACTUAL_MASK_BRIGHTNESS_PATH, maskBrightness);

    if (fileExists(FP_GREEN_CIRCLE_PATH)) {
        set(FP_GREEN_CIRCLE_PATH, 1);
        LOG(INFO) << "Fingerprint green circle enabled.";
    } else {
        LOG(WARNING) << "FP_GREEN_CIRCLE_PATH not available; mask functionality skipped.";
    }
}

void OpticalUdfps::disableMask() {
    if (fileExists(FP_GREEN_CIRCLE_PATH)) {
        set(FP_GREEN_CIRCLE_PATH, 0);
        LOG(INFO) << "Fingerprint green circle disabled.";
    } else {
        LOG(WARNING) << "FP_GREEN_CIRCLE_PATH not available; mask functionality skipped.";
    }
}

void OpticalUdfps::maskBrightness(int brightness) {
    set(MASK_BRIGHTNESS_PATH, brightness);
}

int OpticalUdfps::actualMaskBrightness() {
    return get<int>(ACTUAL_MASK_BRIGHTNESS_PATH, 0);
}

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
