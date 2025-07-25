#include "aabb.hpp"
#include "ray.hpp"

namespace rt {
	const AABB AABB::empty{ Interval::empty, Interval::empty, Interval::empty };

	void AABB::pad_to_minimums() {
		constexpr double delta = 0.0001;
		if (x.size() < delta) {
			x = x.expand(delta);
		}
		if (y.size() < delta) {
			y = y.expand(delta);
		}
		if (z.size() < delta) {
			z = z.expand(delta);
		}
	}

	AABB::AABB(const Interval& x, const Interval& y, const Interval& z) :
		x(x),
		y(y),
		z(z)
	{
		pad_to_minimums();
	}

	AABB::AABB(const Point3& a, const Point3& b) :
		x((a.x() <= b.x()) ? Interval{ a.x(), b.x() } : Interval{ b.x(), a.x() }),
		y((a.y() <= b.y()) ? Interval{ a.y(), b.y() } : Interval{ b.y(), a.y() }),
		z((a.z() <= b.z()) ? Interval{ a.z(), b.z() } : Interval{ b.z(), a.z() })
	{
		pad_to_minimums();
	}

	AABB::AABB(const AABB& a, const AABB& b) :
		x(a.x, b.x),
		y(a.y, b.y),
		z(a.z, b.z)
	{
	}

	const Interval& AABB::axis_interval(const int axis) const {
		switch (axis) {
		case to_index(Axis::X): return x;
		case to_index(Axis::Y): return y;
		case to_index(Axis::Z): return z;
		default:
			throw std::runtime_error("Invalid axis provided when getting AABB axis interval.");
		}
	}

	int AABB::longest_axis() const {
		if (y.size() < x.size()) {
			return (z.size() < y.size()) ? to_index(Axis::Z) : to_index(Axis::Y);
		}
		return (z.size() < x.size()) ? to_index(Axis::Z) : to_index(Axis::X);
	}

	bool AABB::hit(const Ray& ray, const Interval& ray_t) const {
		Interval t = ray_t;
		const auto& origin = ray.origin();
		const auto& direction = ray.direction();

		for (int i = 0; i < 3; i++) {
			const auto& interval = axis_interval(i);
			const auto pos = origin[i];
			const auto dir_inv = 1.0 / direction[i];

			const double t0 = (interval.min - pos) * dir_inv;
			const double t1 = (interval.max - pos) * dir_inv;

			if (t0 < t1) {
				if (t0 > t.min) { t.min = t0; }
				if (t1 < t.max) { t.max = t1; }
			} else {
				if (t0 < t.max) { t.max = t0; }
				if (t1 > t.min) { t.min = t1; }
			}

			if (t.max < t.min) { return false; }
		}
		return true;
	}
}

