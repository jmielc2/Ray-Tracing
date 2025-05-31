#pragma once

#include "util.hpp"

#include <sstream>

namespace rt {
	using Color = Vec3;

	struct ImageData {
		std::string name;
		size_t width, height;
		std::vector<Color> pixel_buffer;
	};

	void write_to_file(const ImageData& data);
}