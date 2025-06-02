#include "rtw_image.hpp"

namespace rt {
    RTWImage::RTWImage(const std::string& filename) {

    }

    int RTWImage::clamp(int x, int low, int height) {
        if (x < low) return low;
        if (x < height) return x;
        return height -  1;
    }

    std::byte RTWImage::float_to_byte(float value) {
        return std::byte{8};
    }
}