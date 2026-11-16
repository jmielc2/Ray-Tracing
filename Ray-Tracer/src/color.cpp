#include "util.hpp"
#include "color.hpp"
#include "interval.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image_write.h>
#include <ranges>

namespace rt {
	static double linear_to_gamma(double linear_component) {
		if (linear_component > 0) {
			return std::sqrt(linear_component);
		}
		return 0;
	}

	// static void write_color(std::ostream& stream, const Color& col) {
	// 	const double r = linear_to_gamma(col.x());
	// 	const double g = linear_to_gamma(col.y());
	// 	const double b = linear_to_gamma(col.z());
	//
	// 	static constexpr Interval intensity(0.0, 0.999);
	// 	stream << static_cast<int>(256 * intensity.clamp(r)) << " " << static_cast<int>(256 * intensity.clamp(g)) << " " << static_cast<int>(256 * intensity.clamp(b)) << "\n";
	// }

	void write_to_file(const ImageData& data) {
		const auto& width = data.width;
		const auto& height = data.height;

		const auto transformColorChannel = [](double channel) -> std::uint8_t {
			constexpr Interval intensity(0.0, 0.999);
			return static_cast<std::uint8_t>(255 * intensity.clamp(linear_to_gamma(channel)));
		};
		const auto transformColorToArray = [&transformColorChannel](const Color& color) -> std::array<std::uint8_t, 3> {
			return std::array {
				transformColorChannel(color.x()),
				transformColorChannel(color.y()),
				transformColorChannel(color.z()),
			};
		};
		std::vector<std::uint8_t> buffer;
		buffer.reserve(3 * data.pixels.size());
		std::ranges::copy(data.pixels | std::views::transform(transformColorToArray) | std::views::join, std::back_inserter(buffer));

		// Last argument is the stride: bytes per row
		stbi_write_png(std::string(data.name).c_str(), width, height, 3, buffer.data(), width * 3);
	}
}