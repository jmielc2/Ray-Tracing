#include "material.hpp"

namespace rt {
	/*
	* Lambertian
	*/

	Lambertian::Lambertian(const std::shared_ptr<Texture>& texture) : 
		tex(texture)
	{
	}

	Lambertian::Lambertian(const Color& albedo) :
		tex(std::make_shared<SolidColor>(albedo))
	{
	}

	std::optional<ScatterRecord> Lambertian::scatter(const Ray& ray, const HitRecord& record) const {
		Vec3 scatter_direction = record.normal + random_unit_vector();
		if (scatter_direction.near_zero()) {
			scatter_direction = record.normal;
		}
		return { ScatterRecord{ tex->value(record.u, record.v, record.point), Ray(record.point, scatter_direction, ray.time()) } };
	}

	/*
	* Metal
	*/

	Metal::Metal(const Color& albedo, double fuzz) :
		albedo(albedo),
		fuzz(fuzz < 1 ? std::fabs(fuzz) : 1)
	{
	}

	std::optional<ScatterRecord> Metal::scatter(const Ray& ray, const HitRecord& record) const {
		const Vec3 scatter_direction = normalize(reflect(ray.direction(), record.normal)) + random_unit_vector() * fuzz;
		if (dot(scatter_direction, record.normal) <= 0) {
			return {};
		}
		return { ScatterRecord{ albedo, Ray(record.point, scatter_direction, ray.time()) } };
	}

	/*
	* Dielectric
	*/

	double Dielectric::reflectance(double cosine, double refraction_ratio) {
		double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow(1 - cosine, 5);
	}

	std::optional<ScatterRecord> Dielectric::scatter(const Ray& ray, const HitRecord& record) const {
		const Vec3 unit_direction = normalize(ray.direction());
		const double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1);
		const double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
		const double refraction_ratio = record.front_face ? 1.0 / refraction_index : refraction_index;
		Vec3 scatter_direction;
		if (refraction_ratio * sin_theta > 1 || reflectance(cos_theta, refraction_ratio) > random_double()) {
			scatter_direction = reflect(unit_direction, record.normal);
		} else {
			scatter_direction = refract(unit_direction, record.normal, refraction_ratio);
		}
		return { ScatterRecord { Color(1, 1, 1), Ray(record.point, scatter_direction, ray.time()) } };
	}
}
