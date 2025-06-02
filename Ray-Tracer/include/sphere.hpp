#pragma once

#include "util.hpp"
#include "hittable.hpp"

namespace rt {
	class Sphere : public Hittable {
	private:
		Ray center;
		double radius;
		std::shared_ptr<Material> material;
		AABB bbox;

		constexpr std::pair<double, double> getSphereUV(const Point3& point) const;

	public:
		Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& material);
		Sphere(const Point3& start, const Point3& end, double radius, const std::shared_ptr<Material>& material);

		std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override;
		const inline AABB& bounding_box() const override { return bbox; }
	};
}
