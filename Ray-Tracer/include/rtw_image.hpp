#pragma once

#include "util.hpp"

namespace rt {
    class RTWImage {
    private:
        const int bytes_per_pixel = 3;
        std::vector<float> fdata;
        std::vector<std::byte> bdata;
        size_t image_width = 0;
        size_t image_height = 0;
        size_t bytes_per_scanline = 0;

        static int clamp(int x, int low, int high);
        static std::byte float_to_byte(float value);

    public:
        RTWImage() = default;
        explicit RTWImage(const std::string& filename);
    };
}
