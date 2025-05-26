#pragma once

#include "util.hpp"

namespace rt {
	class Ray; 

	class AABB {
	public:
		Interval x, y, z;

		AABB();
		AABB(Interval x, Interval y, Interval z);
		AABB(Point3 a, Point3 b);
		AABB(AABB a, AABB b);

		const Interval& axis_interval(int axis) const;
		int longest_axis() const;
		bool hit(Ray ray, Interval ray_t) const;

		static const AABB empty;
	};
}

