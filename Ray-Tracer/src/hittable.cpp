#include "hittable.hpp"

namespace rt {
	void HitRecord::set_face_normal(const Ray& r, const Vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = (front_face) ? outward_normal : -outward_normal;
	}

	std::optional<HitRecord> Translate::hit(const Ray& ray, const Interval& ray_t) const {
		const auto offset_ray = Ray(ray.origin() - offset, ray.direction(), ray.time());
		auto hit_result = object->hit(offset_ray, ray_t);
		if (hit_result.has_value()) {
			hit_result->point += offset;
		}
		return hit_result;
	}

	std::optional<HitRecord> RotateY::hit(const Ray& ray, const Interval& ray_t) const {
		// Transform to object space
		const auto rotated_ray = Ray(
			ray.origin(),
			ray.direction()
		);

		// Run collision
		auto hit_record = object->hit(rotated_ray, ray_t);
		if (hit_record.has_value()) {
			// Update hit record
		}
		return hit_record;
	}
}

