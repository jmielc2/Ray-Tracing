#include "rtw_image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

namespace rt {
    RTWImage::RTWImage(const std::string& filename) {
        auto image_dir = getenv("RTW_IMAGE");
        if (image_dir && load(std::string(image_dir) + "/" + filename)) return;
        if (load(filename)) return;
        if (load("images/" + filename)) return;
        if (load("../images/" + filename)) return;
        if (load("../../images/" + filename)) return;

        throw std::invalid_argument("Could not load file " + filename);
    }

    RTWImage::~RTWImage() {
        bdata.clear();
        STBI_FREE(fdata);
    }

    bool RTWImage::load(const std::string& filename) {
        auto n = bytes_per_pixel;
        fdata = stbi_load(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        if (fdata == nullptr) {
            return false;
        }
        bytes_per_scanline = bytes_per_pixel * image_width;
        convert_to_bytes();
        return true;
    }

    int RTWImage::clamp(int x, int low, int height) {
        if (x < low) return low;
        if (x < height) return x;
        return height - 1;
    }

    unsigned char RTWImage::float_to_byte(float value) {
        if (value <= 0.0f) {
            return 0;
        } else if (value >= 1.0f) {
            return 255;
        }
        return static_cast<unsigned char>(value * 256.0f);
    }

    void RTWImage::convert_to_bytes() {
        const int total_bytes = bytes_per_scanline * image_height;
        bdata.clear();
        bdata.reserve(total_bytes);
        std::transform(fdata, fdata + total_bytes, std::back_inserter(bdata), [](float a) {
            return float_to_byte(a);
        });
    }

    const unsigned char* RTWImage::pixel_data(int x, int y) const {
        if (bdata.empty()) return magenta;
        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);
        return &bdata[y * bytes_per_scanline + x * bytes_per_pixel];
    }
}