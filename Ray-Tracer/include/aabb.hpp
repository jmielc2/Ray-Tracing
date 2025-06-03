#pragma once

#include "util.hpp"

namespace rt {
	class Ray; 

	class AABB {
	public:
		Interval x, y, z;

		AABB() = default;
		AABB(const Interval& x, const Interval& y, const Interval& z);
		AABB(const Point3& a, const Point3& b);
		AABB(const AABB& a, const AABB& b);

		const Interval& axis_interval(int axis) const;
		int longest_axis() const;
		bool hit(const Ray& ray, const Interval& ray_t) const;

		static const AABB empty;
	};
}

