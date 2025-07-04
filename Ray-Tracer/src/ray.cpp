#include "ray.hpp"

namespace rt {
	Ray::Ray(const Point3& origin, const Vec3& direction, const double time) :
		orig(origin),
		dir(direction),
		tm(time)
	{
	}
	
	Ray::Ray(const Point3& origin, const Vec3& direction) :
		Ray(origin, direction, 0) 
	{
	}

	Point3 Ray::at(const double t) const {
		return orig + dir * t;
	}
}