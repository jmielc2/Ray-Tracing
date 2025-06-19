#pragma once

#include "vec3.hpp"
#include "material.hpp"
#include "hittable.hpp"
#include "interval.hpp"
#include "aabb.hpp"

namespace rt {
	class Sphere final : public Hittable {
		Ray center;
		double radius;
		std::shared_ptr<Material> material;
		AABB bbox;

		[[nodiscard]] std::pair<double, double> getSphereUV(const Point3& point) const;

	public:
		Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& material);
		Sphere(const Point3& start, const Point3& end, double radius, const std::shared_ptr<Material>& material);

		[[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override;
		[[nodiscard]] const AABB& bounding_box() const override { return bbox; }
	};
}
