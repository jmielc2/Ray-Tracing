#include "rtw_image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

namespace rt {
    static const int channels_per_pixel = 3;

    RTWImage::RTWImage(const std::string& filename) {
        auto image_dir = getenv("RTW_IMAGE");
        if (image_dir && load(std::string(image_dir) + "/" + filename)) return;
        if (load(filename)) return;
        if (load("images/" + filename)) return;
        if (load("../images/" + filename)) return;
        if (load("../../images/" + filename)) return;
        if (load("../../../images/" + filename)) return;

        throw std::invalid_argument("Could not load file " + filename);
    }

    bool RTWImage::load(const std::string& filename) {
        auto n = channels_per_pixel;
        float* fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, channels_per_pixel);
        if (fdata == nullptr) {
            return false;
        }
        convert_to_bytes(fdata);
        STBI_FREE(fdata);
        return true;
    }

    void RTWImage::convert_to_bytes(const float* fdata) {
        const int num_pixels = image_height * image_width;
        const int fdata_size = num_pixels * channels_per_pixel;
        data.clear();
        data.resize(num_pixels);
        for (auto i = 0, j = 0; i < fdata_size; i += channels_per_pixel, j++) {
            data[j] = Color(
                fdata[i],
                fdata[i + 1],
                fdata[i + 2]
            );
        }
    }

    const Color& RTWImage::pixel_data(int x, int y) const {
        if (data.empty()) return magenta;
        x = clamp<int>(x, 0, image_width);
        y = clamp<int>(y, 0, image_height);
        int index = y * image_width * y + x;
        return data[y * image_width + x];
    }
}