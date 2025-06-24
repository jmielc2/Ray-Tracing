#pragma once

#include "perlin.hpp"
#include "util.hpp"
#include "rtw_image.hpp"

namespace rt {
	class Texture {
	public:
		virtual ~Texture() = default;

		[[nodiscard]] virtual Color value(double u, double v, const Point3& p) const = 0;
	};

	class SolidColor final : public Texture {
		const Color albedo;

	public:
		explicit SolidColor(const Color& albedo);
		SolidColor(double r, double g, double b);

		[[nodiscard]] Color value([[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const Point3& p) const override { return albedo; }
	};

	class CheckerTexture final : public Texture {
		const double inv_scale;
		const std::shared_ptr<Texture> even_texture;
		const std::shared_ptr<Texture> odd_texture;

	public:
		CheckerTexture(double scale, const std::shared_ptr<Texture>& even_texture, const std::shared_ptr<Texture>& odd_texture);
		CheckerTexture(double scale, const Color& even_color, const Color& odd_color);

		[[nodiscard]] Color value(double u, double v, const Point3& p) const override;
	};

	class ImageTexture final : public Texture {
		const RTWImage image;
	public:
		explicit ImageTexture(const std::string& filename);
		
		[[nodiscard]] Color value(double u, double v, const Point3& p) const override;
	};

	class NoiseTexture final : public Texture {
		const Perlin noise;
		const double scale;
	public:
		explicit NoiseTexture(double scale);

		[[nodiscard]] Color value(double u, double v, const Point3& p) const override;
	};
}

