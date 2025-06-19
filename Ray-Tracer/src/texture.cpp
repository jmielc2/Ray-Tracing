#include "texture.hpp"
#include "interval.hpp"

namespace rt {
	/**
	 * Solid Color Texture
	 */

	SolidColor::SolidColor(const Color& albedo) : albedo(albedo) {}

	SolidColor::SolidColor(double r, double g, double b) : albedo(Color(r, g, b)) {}

	/**
	 * Checker Texture
	 */

	CheckerTexture::CheckerTexture(double scale, const std::shared_ptr<Texture>& even_texture, const std::shared_ptr<Texture>& odd_texture) :
		inv_scale(1.0 / scale),
		even_texture(even_texture),
		odd_texture(odd_texture)
	{
	}

	CheckerTexture::CheckerTexture(double scale, const Color& even_color, const Color& odd_color) :
		inv_scale(1.0 / scale),
		even_texture(std::make_shared<SolidColor>(even_color)),
		odd_texture(std::make_shared<SolidColor>(odd_color))
	{
	}

	Color CheckerTexture::value(double u, double v, const Point3& p) const {
		const int x = static_cast<int>(std::floor(p.x() * inv_scale));
		const int y = static_cast<int>(std::floor(p.y() * inv_scale));
		if (int z = static_cast<int>(std::floor(p.z() * inv_scale)); (x + y + z) % 2 == 0) {
			return odd_texture->value(u, v, p);
		}
		return even_texture->value(u, v, p);
	}

	/**
	 * Image Texture
	 */

	ImageTexture::ImageTexture(const std::string& filename) : image(filename) {}

	Color ImageTexture::value(double u, double v, const Point3& p) const {
		if (image.height() <= 0) return cyan; // No image data, return cyan for debugging purposes.

		constexpr Interval value_range(0, 1);
		u = value_range.clamp(u);
		v = 1.0f - value_range.clamp(v);
		
		return image.pixel_data(
			map<double>(u, 0, 1, 0, image.width()),
			map<double>(v, 0, 1, 0, image.height())
		);
	}
}