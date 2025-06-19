#include <fstream>
#include "util.hpp"
#include "color.hpp"
#include "interval.hpp"

namespace rt {
	static double linear_to_gamma(double linear_component) {
		if (linear_component > 0) {
			return std::sqrt(linear_component);
		}
		return 0;
	}

	static void write_color(std::ostream& stream, const Color& col) {
		const double r = linear_to_gamma(col.x());
		const double g = linear_to_gamma(col.y());
		const double b = linear_to_gamma(col.z());

		static constexpr Interval intensity(0.0, 0.999);
		stream << static_cast<int>(256 * intensity.clamp(r)) << " " << static_cast<int>(256 * intensity.clamp(g)) << " " << static_cast<int>(256 * intensity.clamp(b)) << "\n";
	}

	void write_to_file(const ImageData& data) {
		std::ofstream file(data.name);
		file << "P3\n" << data.width << ' ' << data.height << "\n255\n";
		for (const Color& color : data.pixel_buffer) {
			write_color(file, color);
		}
		file.close();
	}
}