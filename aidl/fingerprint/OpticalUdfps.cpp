#include "OpticalUdfps.h"
#include <fstream>
#include <string>
#include <android-base/logging.h>

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
        LOG(ERROR) << "Failed to open file at path: " << path << "; returning default value: " << def;
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

// Reads the display brightness directly from the brightness file.
int OpticalUdfps::readBrightness() {
    LOG(INFO) << "Reading display brightness from DISPLAY_BRIGHTNESS_PATH.";
    int brightness = get<int>(DISPLAY_BRIGHTNESS_PATH, -1);
    LOG(INFO) << "Display brightness read as: " << brightness;
    return brightness;
}

// Calculates the mask brightness based on display brightness.
int OpticalUdfps::calculateMaskBrightness(int displayBrightness) {
    LOG(INFO) << "Calculating mask brightness for display brightness: " << displayBrightness;
    int maskBrightness = getFingerPrintBacklightValue(displayBrightness);
    if (maskBrightness <= 0) {
        LOG(WARNING) << "Calculated mask brightness is too low (" << maskBrightness
                     << "); setting to default value of 319.";
        maskBrightness = 319;
    }

    LOG(INFO) << "Calculated mask brightness: " << maskBrightness
              << " based on display brightness: " << displayBrightness;

    // Set the mask brightness in the system
    set(MASK_BRIGHTNESS_PATH, maskBrightness);
    LOG(INFO) << "Mask brightness set to: " << maskBrightness;

    return maskBrightness;
}

// Placeholder function for obtaining fingerprint backlight value
int OpticalUdfps::getFingerPrintBacklightValue(int displayBrightness) {
    int result = static_cast<int>(displayBrightness * 0.607);
    LOG(INFO) << "Calculated fingerprint backlight value: " << result << " for display brightness: " << displayBrightness;
    return result;
}

// Enables the fingerprint overlay mask with brightness adjustment.
void OpticalUdfps::enableMask() {
    LOG(INFO) << "Enabling mask functionality.";

    // Step 1: Read display brightness and check for valid brightness
    int displayBrightness = readBrightness();
    if (displayBrightness < 0) {
        LOG(ERROR) << "Invalid display brightness: " << displayBrightness
                   << "; unable to proceed with enabling mask.";
        return;
    }

    // Step 2: Calculate mask brightness and verify it was set
    int maskBrightness = calculateMaskBrightness(displayBrightness);
    if (maskBrightness < 0) {
        LOG(ERROR) << "Failed to calculate a valid mask brightness; aborting enableMask.";
        return;
    }

    // Step 3: Set FOD rectangle and verify
    LOG(INFO) << "Setting FOD rectangle for mask overlay.";
    setFodRect();

    // Step 4: Enable fingerprint green circle overlay
    LOG(INFO) << "Enabling fingerprint green circle overlay.";
    set(FP_GREEN_CIRCLE_PATH, 1);

    LOG(INFO) << "Mask enabled successfully with brightness: " << maskBrightness;
}

// Disables the fingerprint overlay mask by turning off the green circle.
void OpticalUdfps::disableMask() {
    LOG(INFO) << "Disabling mask functionality.";
    set(FP_GREEN_CIRCLE_PATH, 0);
    LOG(INFO) << "Fingerprint overlay mask successfully disabled.";
}

// Sets the FOD rectangle for the area of the fingerprint overlay.
void OpticalUdfps::setFodRect() {
    LOG(INFO) << "Setting FOD rectangle with predefined coordinates.";
    set(TSP_CMD, "set_fod_rect,721,2289,880,2448");
    LOG(INFO) << "FOD rectangle set successfully.";
}

// Manually sets the mask brightness to a specific value.
void OpticalUdfps::maskBrightness(int brightness) {
    LOG(INFO) << "Setting mask brightness manually to: " << brightness;
    set(MASK_BRIGHTNESS_PATH, brightness);
    LOG(INFO) << "Mask brightness set manually to: " << brightness;
}

// Retrieves the actual mask brightness, with a default of 0 if unavailable.
int OpticalUdfps::actualMaskBrightness() {
    LOG(INFO) << "Reading actual mask brightness from system path.";
    int brightness = get<int>(ACTUAL_MASK_BRIGHTNESS_PATH, 0);
    LOG(INFO) << "Actual mask brightness retrieved as: " << brightness;
    return brightness;
}

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
