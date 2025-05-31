#include "color.hpp"

namespace rt {
	static double linear_to_gamma(double linear_component) {
		if (linear_component > 0) {
			return std::sqrt(linear_component);
		}
		return 0;
	}

	static void write_color(std::ostream& stream, const Color& col) {
		double r = linear_to_gamma(col.x());
		double g = linear_to_gamma(col.y());
		double b = linear_to_gamma(col.z());

		static const Interval intensity(0.0, 0.999);
		stream << int(256 * intensity.clamp(r)) << " " << int(256 * intensity.clamp(g)) << " " << int(256 * intensity.clamp(b)) << "\n";
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