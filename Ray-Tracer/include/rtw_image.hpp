#pragma once

#ifdef _MSC_VER
    #pragma warning(push, 0)
#endif

#include "util.hpp"

namespace rt {
    class RTWImage {
    private:
        std::vector<Color> data;
        int image_width = 0;
        int image_height = 0;

        Color fdata_to_color(const float* fdata);
        void convert_to_bytes(const float* fdata);
    public:
        RTWImage() = default;
        explicit RTWImage(const std::string& filename);

        bool load(const std::string& filename);
        inline int width() const { return (data.empty()) ? 0 : image_width; }
        inline int height() const { return (data.empty()) ? 0 : image_height; }
        const Color& pixel_data(int x, int y) const;
    };
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
