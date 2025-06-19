#pragma once

#include "vec3.hpp"
#include "interval.hpp"

namespace rt {
	class Ray; 

	class AABB {
	public:
		const Interval x, y, z;

		AABB() = default;
		AABB(const Interval& x, const Interval& y, const Interval& z);
		AABB(const Point3& a, const Point3& b);
		AABB(const AABB& a, const AABB& b);

		[[nodiscard]] const Interval& axis_interval(int axis) const;
		[[nodiscard]] int longest_axis() const;
		[[nodiscard]] bool hit(const Ray& ray, const Interval& ray_t) const;

		static const AABB empty;
	};
}

