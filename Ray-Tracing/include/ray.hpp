#ifndef RAY_HPP
#define RAY_HPP

#include "util.hpp"

class Ray {
private:
	Point3 orig;
	Vec3 dir;
	double tm;
public:
	Ray(const Point3& origin, const Vec3& direction, double time) :
		orig(origin),
		dir(direction),
		tm(time)
	{}
	Ray(const Point3& origin, const Vec3& direction) : Ray(origin, direction, 0) {}

	const Point3& origin() const { return orig; }
	const Vec3& direction() const { return dir; }
	double time() const { return tm; }
	Point3 at(double t) const {
		return orig + dir * t;
	}
};

#endif
