#include "aabb.hpp"

const AABB AABB::empty{ Interval::empty, Interval::empty, Interval::empty };

AABB::AABB() {}

AABB::AABB(Interval x, Interval y, Interval z) :
	x(std::move(x)), y(std::move(y)), z(std::move(z))
{ }

AABB::AABB(Point3 a, Point3 b) {
	x = (a.x() <= b.x()) ? Interval{ a.x(), b.x() } : Interval{ b.x(), a.x() };
	y = (a.y() <= b.y()) ? Interval{ a.y(), b.y() } : Interval{ b.y(), a.y() };
	z = (a.z() <= b.z()) ? Interval{ a.z(), b.z() } : Interval{ b.z(), a.z() };
}

AABB::AABB(AABB a, AABB b) :
	x(Interval(a.x, b.x)),
	y(Interval(a.y, b.y)),
	z(Interval(a.z, b.z))
{ }

const Interval& AABB::axis_interval(int axis) const {
	switch (axis) {
	case(X_AXIS): return x;
	case(Y_AXIS): return y;
	case(Z_AXIS): return z;
	default:
		throw std::runtime_error("Inalid axis provided when getting AABB axis interval.");
	}
}

int AABB::longest_axis() const {
	if (y.size() < x.size()) {
		return (z.size() < y.size()) ? Z_AXIS : Y_AXIS;
	}
	return (z.size() < x.size()) ? Z_AXIS : X_AXIS;
}

bool AABB::hit(Ray ray, Interval ray_t) const {
	const auto& origin = ray.origin();
	const auto& direction = ray.direction();

	for (int i = 0; i < 3; i++) {
		const auto& interval = axis_interval(i);
		const auto pos = origin[i];
		const auto dir_inv = 1.0 / direction[i];

		const double t0 = (interval.min - pos) * dir_inv;
		const double t1 = (interval.max - pos) * dir_inv;

		if (t0 < t1) {
			if (t0 > ray_t.min) { ray_t.min = t0; }
			if (t1 < ray_t.max) { ray_t.max = t1; }
		} else {
			if (t0 < ray_t.max) { ray_t.max = t0; }
			if (t1 > ray_t.min) { ray_t.min = t1; }
		}

		if (ray_t.max < ray_t.min) { return false; }
	}
	return true;
}


