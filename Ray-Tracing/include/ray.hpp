#pragma once

#include "util.hpp"

namespace rt {
	class Ray {
	private:
		Point3 orig;
		Vec3 dir;
		double tm;
	public:
		Ray(const Point3& origin, const Vec3& direction, double time);
		Ray(const Point3& origin, const Vec3& direction);

		const inline Point3& origin() const { return orig; }
		const inline Vec3& direction() const { return dir; }
		double inline time() const { return tm; }
		Point3 at(double t) const;
	};
}

