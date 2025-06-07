#include "texture.hpp"

namespace rt {
	/**
	 * Solid Color Texture
	 */

	SolidColor::SolidColor(const Color& albedo) :
		albedo(albedo)
	{
	}

	SolidColor::SolidColor(double r, double g, double b) :
		albedo(Color(r, g, b))
	{
	}

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
		int x = static_cast<int>(std::floor(p.x() * inv_scale));
		int y = static_cast<int>(std::floor(p.y() * inv_scale));
		int z = static_cast<int>(std::floor(p.z() * inv_scale));
		if ((x + y + z) % 2 == 0) {
			return odd_texture->value(u, v, p);
		}
		return even_texture->value(u, v, p);
	}

	/**
	 * Image Texture
	 */

	ImageTexture::ImageTexture(const std::string& filename) :
		image(filename)
	{
	}

	Color ImageTexture::value(double u, double v, const Point3& p) const {
		if (image.height() <= 0) return Color{0, 1, 1}; // No image data, return cyan for debugging purposes.

		const Interval value_range(0, 1);
		u = value_range.clamp(u);
		v = 1.0f - value_range.clamp(v);
		auto pixel = image.pixel_data(u, v);
		const auto color_scale = 1.0f / 255.0f;
		return Color(pixel[0], pixel[1], pixel[2]) * color_scale;
	}
}