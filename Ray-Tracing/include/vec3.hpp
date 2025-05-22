#ifndef VEC3_HPP
#define VEC3_HPP

#include "util.hpp"

class Vec3 {
private:
	double e[3];

public:
	constexpr Vec3() : e{ 0.0, 0.0, 0.0 } {}
	constexpr Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	static Vec3 random() {
		return Vec3(random_double(), random_double(), random_double());
	}
	static Vec3 random(double min, double max) {
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	const double& x() const { return e[X_AXIS]; }
	const double& y() const { return e[Y_AXIS]; }
	const double& z() const { return e[Z_AXIS]; }

	Vec3 operator-() const { return Vec3(-e[X_AXIS], -e[Y_AXIS], -e[Z_AXIS]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3& v) {
		e[X_AXIS] += v[X_AXIS];
		e[Y_AXIS] += v[Y_AXIS];
		e[Z_AXIS] += v[Z_AXIS];
		return *this;
	}

	Vec3& operator-=(const Vec3& v) {
		e[X_AXIS] -= v[X_AXIS];
		e[Y_AXIS] -= v[Y_AXIS];
		e[Z_AXIS] -= v[Z_AXIS];
		return *this;
	}

	Vec3& operator*=(double t) {
		e[X_AXIS] *= t;
		e[Y_AXIS] *= t;
		e[Z_AXIS] *= t;
		return *this;
	}

	Vec3& operator/=(double t) {
		e[X_AXIS] /= t;
		e[Y_AXIS] /= t;
		e[Z_AXIS] /= t;
		return *this;
	}

	double length() const {
		return std::sqrt(length_squared());
	}

	double length_squared() const {
		return e[X_AXIS] * e[X_AXIS] + e[Y_AXIS] * e[Y_AXIS] + e[Z_AXIS] * e[Z_AXIS];
	}

	bool near_zero() const {
		double s = 1e-8;
		return (std::fabs(e[X_AXIS]) < s) && (std::fabs(e[Y_AXIS]) < s) && (std::fabs(e[Z_AXIS]) < s);
	}

	friend std::ostream& operator<<(std::ostream& out, const Vec3& v);
	friend Vec3 operator+(const Vec3& a, const Vec3& b);
	friend Vec3 operator-(const Vec3& a, const Vec3& b);
	friend Vec3 operator*(const Vec3& a, const Vec3& b);
	friend Vec3 operator*(double t, const Vec3& a);
	friend Vec3 operator*(const Vec3& a, double t);
	friend Vec3 operator/(const Vec3& a, double t);
	friend double dot(const Vec3& a, const Vec3& b);
	friend Vec3 cross(const Vec3& a, const Vec3& b);
	friend Vec3 unit_vector(const Vec3& a);
};

using Point3 = Vec3;

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << '{' << v.e[X_AXIS] << ' ' << v.e[Y_AXIS] << ' ' << v.e[Z_AXIS] << '}';
}

Vec3 operator+(const Vec3& a, const Vec3& b) {
	return Vec3(
		a.e[X_AXIS] + b.e[X_AXIS],
		a.e[Y_AXIS] + b.e[Y_AXIS],
		a.e[Z_AXIS] + b.e[Z_AXIS]
	);
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
	return Vec3(
		a.e[X_AXIS] - b.e[X_AXIS],
		a.e[Y_AXIS] - b.e[Y_AXIS],
		a.e[Z_AXIS] - b.e[Z_AXIS]
	);
}

Vec3 operator*(const Vec3& a, const Vec3& b) {
	return Vec3(
		a.e[X_AXIS] * b.e[X_AXIS],
		a.e[Y_AXIS] * b.e[Y_AXIS],
		a.e[Z_AXIS] * b.e[Z_AXIS]
	);
}

Vec3 operator*(double t, const Vec3& a) {
	return Vec3(
		a.e[X_AXIS] * t,
		a.e[Y_AXIS] * t,
		a.e[Z_AXIS] * t
	);
}

Vec3 operator*(const Vec3& a, double t) {
	return t * a;
}

Vec3 operator/(const Vec3& a, double t) {
	return (1.0 / t) * a;
}

double dot(const Vec3& a, const Vec3& b) {
	return a.e[X_AXIS] * b.e[X_AXIS] + a.e[Y_AXIS] * b.e[Y_AXIS] + a.e[Z_AXIS] * b.e[Z_AXIS];
}

Vec3 cross(const Vec3& a, const Vec3& b) {
	return Vec3(
		a.e[Y_AXIS] * b.e[Z_AXIS] - a.e[Z_AXIS] * b.e[Y_AXIS],
		a.e[Z_AXIS] * b.e[X_AXIS] - a.e[X_AXIS] * b.e[Z_AXIS],
		a.e[X_AXIS] * b.e[Y_AXIS] - a.e[Y_AXIS] * b.e[X_AXIS]
	);
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

#endif