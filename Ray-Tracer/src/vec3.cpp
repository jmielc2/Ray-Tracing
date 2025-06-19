#include "vec3.hpp"
#include "util.hpp"

namespace rt {
	Vec3 Vec3::random() {
		return {random_double(), random_double(), random_double()};
	}

	Vec3 Vec3::random(const double min, const double max) {
		return {random_double(min, max), random_double(min, max), random_double(min, max)};
	}

	const double& Vec3::x() const {
		return e[to_index(Axis::X)];
	}

	const double& Vec3::y() const {
		return e[to_index(Axis::Y)];
	}

	const double& Vec3::z() const {
		return e[to_index(Axis::Z)];
	}

	Vec3 Vec3::operator-() const {
		return {-e[to_index(Axis::X)], -e[to_index(Axis::Y)], -e[to_index(Axis::Z)]};
	}

	double Vec3::operator[](int i) const {
		return e[i];
	}

	double& Vec3::operator[](int i) {
		return e[i];
	}

	Vec3& Vec3::operator+=(const Vec3& v) {
		e[to_index(Axis::X)] += v[to_index(Axis::X)];
		e[to_index(Axis::Y)] += v[to_index(Axis::Y)];
		e[to_index(Axis::Z)] += v[to_index(Axis::Z)];
		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& v) {
		e[to_index(Axis::X)] -= v[to_index(Axis::X)];
		e[to_index(Axis::Y)] -= v[to_index(Axis::Y)];
		e[to_index(Axis::Z)] -= v[to_index(Axis::Z)];
		return *this;
	}

	Vec3& Vec3::operator*=(double t) {
		e[to_index(Axis::X)] *= t;
		e[to_index(Axis::Y)] *= t;
		e[to_index(Axis::Z)] *= t;
		return *this;
	}

	Vec3& Vec3::operator/=(double t) {
		e[to_index(Axis::X)] /= t;
		e[to_index(Axis::Y)] /= t;
		e[to_index(Axis::Z)] /= t;
		return *this;
	}

	double Vec3::length() const {
		return std::sqrt(length_squared());
	}

	double Vec3::length_squared() const {
		return e[to_index(Axis::X)] * e[to_index(Axis::X)] + e[to_index(Axis::Y)] * e[to_index(Axis::Y)] + e[to_index(Axis::Z)] * e[to_index(Axis::Z)];
	}

	bool Vec3::near_zero() const {
		double s = 1e-8;
		return (std::fabs(e[to_index(Axis::X)]) < s) && (std::fabs(e[to_index(Axis::Y)]) < s) && (std::fabs(e[to_index(Axis::Z)]) < s);
	}

#ifdef DNDEBUG
	std::ostream& operator<<(std::ostream& out, const Vec3& v) {
		return out << '{' << v.e[to_index(Axis::X)] << ' ' << v.e[to_index(Axis::Y)] << ' ' << v.e[to_index(Axis::Z)] << '}';
	}
#endif

	Vec3 operator+(const Vec3& a, const Vec3& b) {
		return {
			a.e[to_index(Axis::X)] + b.e[to_index(Axis::X)],
			a.e[to_index(Axis::Y)] + b.e[to_index(Axis::Y)],
			a.e[to_index(Axis::Z)] + b.e[to_index(Axis::Z)]
		};
	}

	Vec3 operator-(const Vec3& a, const Vec3& b) {
		return {
			a.e[to_index(Axis::X)] - b.e[to_index(Axis::X)],
			a.e[to_index(Axis::Y)] - b.e[to_index(Axis::Y)],
			a.e[to_index(Axis::Z)] - b.e[to_index(Axis::Z)]
		};
	}

	Vec3 operator*(const Vec3& a, const Vec3& b) {
		return {
			a.e[to_index(Axis::X)] * b.e[to_index(Axis::X)],
			a.e[to_index(Axis::Y)] * b.e[to_index(Axis::Y)],
			a.e[to_index(Axis::Z)] * b.e[to_index(Axis::Z)]
		};
	}

	Vec3 operator*(double t, const Vec3& a) {
		return {
			a.e[to_index(Axis::X)] * t,
			a.e[to_index(Axis::Y)] * t,
			a.e[to_index(Axis::Z)] * t
		};
	}

	Vec3 operator*(const Vec3& a, double t) {
		return t * a;
	}

	Vec3 operator/(const Vec3& a, double t) {
		return (1.0 / t) * a;
	}

	double dot(const Vec3& a, const Vec3& b) {
		return a.e[to_index(Axis::X)] * b.e[to_index(Axis::X)] + a.e[to_index(Axis::Y)] * b.e[to_index(Axis::Y)] + a.e[to_index(Axis::Z)] * b.e[to_index(Axis::Z)];
	}

	Vec3 cross(const Vec3& a, const Vec3& b) {
		return {
			a.e[to_index(Axis::Y)] * b.e[to_index(Axis::Z)] - a.e[to_index(Axis::Z)] * b.e[to_index(Axis::Y)],
			a.e[to_index(Axis::Z)] * b.e[to_index(Axis::X)] - a.e[to_index(Axis::X)] * b.e[to_index(Axis::Z)],
			a.e[to_index(Axis::X)] * b.e[to_index(Axis::Y)] - a.e[to_index(Axis::Y)] * b.e[to_index(Axis::X)]
		};
	}

	Vec3 unit_vector(const Vec3& a) {
		return a / a.length();
	}

	Vec3 random_unit_vector() {
		while (true) {
			Vec3 random = Vec3::random(-1, 1);
			double length_sqrd = random.length_squared();
			if (1e-160 < length_sqrd && length_sqrd <= 1) {
				return random / std::sqrt(length_sqrd);
			}
		}
	}

	Vec3 random_in_unit_disk() {
		while (true) {
			Vec3 random(random_double(-1, 1), random_double(-1, 1), 0);
			if (random.length_squared() < 1) {
				return random;
			}
		}
	}

	Vec3 random_on_hemisphere(const Vec3& normal) {
		Vec3 random = random_unit_vector();
		if (dot(normal, random) <= 0) {
			random *= -1;
		}
		return random;
	}

	double distance(const Point3& a, const Point3& b) {
		return (b - a).length();
	}

	Vec3 reflect(const Vec3& ray_dir, const Vec3& normal) {
		return ray_dir - 2 * normal * dot(ray_dir, normal);
	}

	Vec3 refract(const Vec3& ray_dir, const Vec3& normal, double refraction_ratio) {
		double cos_theta = std::fmin(dot(-ray_dir, normal), 1);
		Vec3 perp_component = refraction_ratio * (ray_dir + cos_theta * normal);
		Vec3 parallel_component = -1 * std::sqrt(std::fabs(1 - perp_component.length_squared())) * normal;
		return perp_component + parallel_component;
	}
}
