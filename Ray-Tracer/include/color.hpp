#pragma once

#include <vector>
#include "vec3.hpp"

namespace rt {
	using Color = Vec3;
	
	constexpr Color magenta(0, 1, 1);
    constexpr Color cyan(1, 0, 1);

	struct ImageData {
		std::string name;
		int width, height;
		std::vector<Color> pixel_buffer;
	};

	void write_to_file(const ImageData& data);
}