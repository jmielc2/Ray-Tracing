#pragma once

#include <memory>
#include "aabb.hpp"
#include "interval.hpp"
#include "vec3.hpp"
#include "ray.hpp"

namespace rt {
	class Material;

	struct HitRecord {
		Point3 point;
		Vec3 normal;
		std::shared_ptr<Material> material;
		double t = 0.0;
		double u = 0.0;
		double v = 0.0;
		bool front_face = false;

		// assumes outward_normal is unit vector
		void set_face_normal(const Ray& r, const Vec3& outward_normal);
	};

	class Hittable {
	public:
		Hittable() = default;
		Hittable(const Hittable&) = default;
		Hittable(Hittable&&) noexcept = default;
		virtual ~Hittable() = default;

		Hittable& operator=(const Hittable&) = default;
		Hittable& operator=(Hittable&&) noexcept = default;

		[[nodiscard]] virtual std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const = 0;
		[[nodiscard]] virtual const AABB& bounding_box() const = 0;
	};
}

