#pragma once

#ifdef _MSC_VER
    #pragma warning(push, 0)
#endif

#include "util.hpp"
#include "stb_image.h"

namespace rt {
    const unsigned char magenta[] = { 255, 0, 255 };
    const unsigned char cyan[] = { 0, 255, 255 };
    const int bytes_per_pixel = 3;

    class RTWImage {
    private:
        stbi_uc* fdata;
        std::vector<unsigned char> bdata;
        int image_width = 0;
        int image_height = 0;
        int bytes_per_scanline = 0;

        static int clamp(int x, int low, int high);
        static unsigned char float_to_byte(float value);
        void convert_to_bytes();
    public:
        RTWImage() = default;
        explicit RTWImage(const std::string& filename);
        ~RTWImage();

        bool load(const std::string& filename);
        inline int width() const { return (fdata == nullptr) ? 0 : image_width; }
        inline int height() const { return (fdata == nullptr) ? 0 : image_height; }
        const unsigned char* pixel_data(int x, int y) const;
    };
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
