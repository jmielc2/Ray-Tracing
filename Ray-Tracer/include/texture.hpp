#pragma once

#include "util.hpp"
#include "rtw_image.hpp"

namespace rt {
	class Texture {
	public:
		virtual ~Texture() = default;

		virtual Color value(double u, double v, const Point3& p) const = 0;
	};

	class SolidColor : public Texture {
	private:
		const Color albedo;

	public:
		explicit SolidColor(const Color& albedo);
		SolidColor(double r, double g, double b);

		inline Color value([[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const Point3& p) const override { return albedo; }
	};

	class CheckerTexture : public Texture {
	private:
		const double inv_scale;
		const std::shared_ptr<Texture> even_texture;
		const std::shared_ptr<Texture> odd_texture;

	public:
		CheckerTexture(double scale, const std::shared_ptr<Texture>& even_texture, const std::shared_ptr<Texture>& odd_texture);
		CheckerTexture(double scale, const Color& even_color, const Color& odd_color);

		Color value(double u, double v, const Point3& p) const override;
	};

	class ImageTexture : public Texture {
	private:
		const RTWImage image;
	public:
		explicit ImageTexture(const std::string& filename);
		
		Color value(double u, double v, const Point3& p) const override;
	};
}

