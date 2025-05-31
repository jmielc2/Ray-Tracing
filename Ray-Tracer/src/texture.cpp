#include "texture.hpp"

namespace rt {
	SolidColor::SolidColor(const Color& albedo) :
		albedo(albedo)
	{
	}

	SolidColor::SolidColor(double r, double g, double b) :
		albedo(Color(r, g, b))
	{
	}

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
}