#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "util.hpp"
#include "hittable.hpp"
#include "texture.hpp"

struct ScatterRecord {
	Color attenuation;
	Ray scatter_ray;
};

class Material {
public:
	virtual ~Material() = default;

	virtual std::optional<ScatterRecord> scatter([[maybe_unused]] const Ray& ray, [[maybe_unused]] const HitRecord& record) const {
		return {};
	}
};

class Lambertian : public Material {
private:
	std::shared_ptr<Texture> tex;
public:
	explicit Lambertian(const std::shared_ptr<Texture>& texture) :
		tex(texture)
	{}

	explicit Lambertian(const Color& albedo) :
		tex(std::make_shared<SolidColor>(albedo))
	{}

	explicit Lambertian(Color&& albedo) : 
		tex(std::make_shared<SolidColor>(std::forward<decltype(albedo)>(albedo)))
	{}

	std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override {
		Vec3 scatter_direction = record.normal + random_unit_vector();
		if (scatter_direction.near_zero()) {
			scatter_direction = record.normal;
		}
		return { ScatterRecord{ tex->value(record.u, record.v, record.point), Ray(record.point, scatter_direction, ray.time()) } };
	}
};

class Metal : public Material {
private:
	Color albedo;
	double fuzz;
public:
	Metal(const Color& albedo, double fuzz) :
		albedo(albedo),
		fuzz(fuzz < 1 ? std::fabs(fuzz) : 1)
	{}

	Metal(const Color&& albedo, double fuzz) :
		albedo(std::forward<decltype(albedo)>(albedo)),
		fuzz(fuzz < 1 ? std::fabs(fuzz) : 1)
	{}

	std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override {
		Vec3 scatter_direction = unit_vector(reflect(ray.direction(), record.normal)) + random_unit_vector() * fuzz;
		if (dot(scatter_direction, record.normal) <= 0) {
			return {};
		}
		return { ScatterRecord{ albedo, Ray(record.point, scatter_direction, ray.time()) } };
	}
};

class Dielectric : public Material {
private:
	double refraction_index;

	// Schlick's reflectance approximation
	double reflectance(double cosine, double refraction_ratio) const {
		double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow(1 - cosine, 5);
	}
public:
	explicit Dielectric(double refraction_index) : refraction_index(refraction_index) {}

	std::optional<ScatterRecord> scatter(const Ray& ray, const HitRecord& record) const override {
		Vec3 unit_direction = unit_vector(ray.direction());
		double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1);
		double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
		double refraction_ratio = record.front_face ? 1.0 / refraction_index : refraction_index;
		Vec3 scatter_direction;
		if (refraction_ratio * sin_theta > 1 || reflectance(cos_theta, refraction_ratio) > random_double()) {
			scatter_direction = reflect(unit_direction, record.normal);
		} else {
			scatter_direction = refract(unit_direction, record.normal, refraction_ratio);
		}
		return { ScatterRecord { Color(1, 1, 1), Ray(record.point, scatter_direction, ray.time()) } };
	}
};

#endif
