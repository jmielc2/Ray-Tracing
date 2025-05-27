#pragma once

#include "util.hpp"

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
		HitRecord() = default;
		void set_face_normal(const Ray& r, const Vec3& outward_normal);
	};

	class Hittable {
	public:
		Hittable() = default;
		virtual ~Hittable() = default;
		virtual std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const = 0;
		virtual const AABB& bounding_box() const = 0;
	};
}

