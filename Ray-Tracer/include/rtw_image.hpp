#pragma once

#ifdef _MSC_VER
    #pragma warning(push, 0)
#endif

#include <vector>
#include "color.hpp"

namespace rt {
    class RTWImage {
        std::vector<Color> data;
        int image_width = 0;
        int image_height = 0;

        void convert_to_bytes(const float* fdata) const;
    public:
        RTWImage() = default;
        explicit RTWImage(const std::string& filename);

        bool load(const std::string& filename);
        [[nodiscard]] int width() const { return (data.empty()) ? 0 : image_width; }
        [[nodiscard]] int height() const { return (data.empty()) ? 0 : image_height; }
        [[nodiscard]] const Color& pixel_data(int x, int y) const;
    };
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
