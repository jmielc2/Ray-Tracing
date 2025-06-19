#pragma once

#include "vec3.hpp"

namespace rt {
	class Ray {
		Point3 orig;
		Vec3 dir;
		double tm;
	public:
		Ray(const Point3& origin, const Vec3& direction, double time);
		Ray(const Point3& origin, const Vec3& direction);

		[[nodiscard]] const Point3& origin() const { return orig; }
		[[nodiscard]] const Vec3& direction() const { return dir; }
		[[nodiscard]] double time() const { return tm; }
		[[nodiscard]] Point3 at(double t) const;
	};
}

