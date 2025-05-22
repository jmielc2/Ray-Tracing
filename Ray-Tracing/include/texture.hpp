#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "util.hpp"
#include <iostream>

class Texture {
public:
	virtual ~Texture() = default;

	virtual Color value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
private:
	const Color albedo;
public:

	template<typename T>
	requires std::same_as<std::decay_t<T>, Color>
	explicit SolidColor(T&& albedo) : 
		albedo(std::forward<T>(albedo))
	{}
	
	SolidColor(double r, double g, double b) :
		albedo(Color(r, g, b))
	{}

	Color value([[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const Point3& p) const override {
		return albedo;
	}
};

class CheckerTexture : public Texture {
private:
	const double inv_scale;
	const std::shared_ptr<Texture> even_texture;
	const std::shared_ptr<Texture> odd_texture;
public:
	CheckerTexture(double scale, const std::shared_ptr<Texture>& even_texture, const std::shared_ptr<Texture>& odd_texture) :
		inv_scale(1.0 / scale),
		even_texture(even_texture),
		odd_texture(odd_texture)
	{}

	CheckerTexture(double scale, const Color& even_color, const Color& odd_color) :
		inv_scale(1.0 / scale),
		even_texture(std::make_shared<SolidColor>(even_color)),
		odd_texture(std::make_shared<SolidColor>(odd_color))
	{}

	Color value(double u, double v, const Point3& p) const override {
		int x = static_cast<int>(std::floor(p.x() * inv_scale));
		int y = static_cast<int>(std::floor(p.y() * inv_scale));
		int z = static_cast<int>(std::floor(p.z() * inv_scale));
		if ((x + y + z) % 2 == 0) {
			return odd_texture->value(u, v, p);
		}
		return even_texture->value(u, v, p);
	}
};

#endif

