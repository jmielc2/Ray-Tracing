#pragma once

#include "util.hpp"
#include "hittable.hpp"
#include "texture.hpp"

namespace rt {
	struct ScatterRecord {
		Color attenuation;
		Ray scatter_ray;
	};

	class Material {
	public:
		virtual ~Material() = default;

		virtual std::optional<ScatterRecord> scatter([[maybe_unused]] const Ray& ray, [[maybe_unused]] const HitRecord& record) const = 0;
	};

	class Lambertian : public Material {
	private:
		std::shared_ptr<Texture> tex;

	public:
		explicit Lambertian(const std::shared_ptr<Texture>& texture);
		explicit Lambertian(const Color& albedo);

		std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override;
	};

	class Metal : public Material {
	private:
		Color albedo;
		double fuzz;
	public:
		Metal(const Color& albedo, double fuzz);
	
		std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override;
	};

	class Dielectric : public Material {
	private:
		double refraction_index;

		// Schlick's reflectance approximation
		double reflectance(double cosine, double refraction_ratio) const;
	public:
		explicit Dielectric(double refraction_index) : refraction_index(refraction_index) {}

		std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override;
	};
}

