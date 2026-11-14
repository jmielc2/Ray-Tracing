#pragma once

#include "hittable.hpp"
#include "texture.hpp"
#include "color.hpp"

namespace rt {
	struct ScatterRecord {
		Color attenuation;
		Ray scatter_ray;
	};

	class Material {
	public:
		virtual ~Material() = default;

		[[nodiscard]] virtual Color emitted(double u, double v, const Point3& p) const { return Color{0, 0, 0}; }

		[[nodiscard]] virtual std::optional<ScatterRecord> scatter([[maybe_unused]] const Ray& ray, [[maybe_unused]] const HitRecord& record) const {
			return {};
		}
	};

	class Lambertian final : public Material {
		std::shared_ptr<Texture> tex;

	public:
		explicit Lambertian(const std::shared_ptr<Texture>& texture);
		explicit Lambertian(const Color& albedo);

		[[nodiscard]] std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override;
	};

	class Metal final : public Material {
		Color albedo;
		double fuzz;
	public:
		Metal(const Color& albedo, double fuzz);
	
		[[nodiscard]] std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override;
	};

	class Dielectric final : public Material {
		double refraction_index;

		// Schlick's reflectance approximation
		[[nodiscard]] static double reflectance(double cosine, double refraction_ratio);
	public:
		explicit Dielectric(const double refraction_index) : refraction_index(refraction_index) {}

		[[nodiscard]] std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override;
	};

	class DiffuseLight final : public Material {
	public:
		explicit DiffuseLight(std::shared_ptr<Texture> texture) : tex(std::move(texture)) {}
		explicit DiffuseLight(const Color& emit) : tex(std::make_shared<SolidColor>(emit)) {}

		[[nodiscard]] Color emitted(double u, double v, const Point3& p) const override;
	private:
		std::shared_ptr<Texture> tex;
	};
}

