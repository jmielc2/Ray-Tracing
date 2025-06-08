#pragma once

#include "util.hpp"

#include <sstream>

namespace rt {
	using Color = Vec3;
	
	constexpr Color magenta(0, 1, 1);
    constexpr Color cyan(1, 0, 1);

	struct ImageData {
		std::string name;
		size_t width, height;
		std::vector<Color> pixel_buffer;
	};

	void write_to_file(const ImageData& data);
}