#pragma once

#include "util.hpp"
#include "hittable.hpp"

namespace rt {
	class Triangle : Hittable {
		Point3 points[3];
	public:
		Triangle(const Point3& a, const Point3& b, const Point3& c) : points{ a, b, c } {}

		std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const {
			return {};
		}
	};
}
