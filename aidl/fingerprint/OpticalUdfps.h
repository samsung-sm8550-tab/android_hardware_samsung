#ifndef AIDL_ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_OPTICALUDFPS_H
#define AIDL_ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_OPTICALUDFPS_H

#include <string>
#include <android-base/logging.h>

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

    // Manually sets the mask brightness to a specified value.
    void maskBrightness(int brightness);

    // Retrieves the actual mask brightness from the system.
    int actualMaskBrightness();

private:
    // Template function to read a value from a file. Returns default if reading fails.
    template <typename T>
    T get(const std::string& path, const T& def);

    // Template function to write a value to a file, with logging of success or failure.
    template <typename T>
    void set(const std::string& path, const T& value);

    // Reads the display brightness directly from the brightness file.
    int readBrightness();

    // Calculates the mask brightness based on the display brightness.
    int calculateMaskBrightness(int displayBrightness);

    // Placeholder function to get fingerprint backlight value based on display brightness.
    int getFingerPrintBacklightValue(int displayBrightness);

    // Sets the Finger On Display (FOD) rectangle.
    void setFodRect();

    // File paths for device brightness and mask configurations
    static constexpr const char* DISPLAY_BRIGHTNESS_PATH = "/sys/class/backlight/panel0-backlight/brightness";
    static constexpr const char* MASK_BRIGHTNESS_PATH = "/sys/class/lcd/panel/mask_brightness";
    static constexpr const char* FP_GREEN_CIRCLE_PATH = "/sys/class/lcd/panel/fp_green_circle";
    static constexpr const char* ACTUAL_MASK_BRIGHTNESS_PATH = "/sys/class/lcd/panel/actual_mask_brightness";
    static constexpr const char* TSP_CMD = "/sys/class/sec/tsp/cmd";

    // Default error return value
    static constexpr float RETURN_ERROR_F = -1.0f;
};

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl

#endif  // AIDL_ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_OPTICALUDFPS_H
