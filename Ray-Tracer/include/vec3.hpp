#pragma once

#include <iostream>

#include "util.hpp"

namespace rt {
	class Vec3 {
		double e[3];

	public:
		constexpr Vec3() : e{ 0.0, 0.0, 0.0 } {}
		constexpr Vec3(const double e0, const double e1, const double e2) : e{ e0, e1, e2 } {}

		static Vec3 random();
		static Vec3 random(double min, double max);

		[[nodiscard]] const double& x() const { return e[to_index(Axis::X)]; }
		[[nodiscard]] const double& y() const { return e[to_index(Axis::Y)]; }
		[[nodiscard]] const double& z() const { return e[to_index(Axis::Z)]; }

		Vec3 operator-() const;
		double operator[](const int i) const { return e[i]; }
		double& operator[](const int i) { return e[i]; }
		Vec3& operator+=(const Vec3& v);
		Vec3& operator-=(const Vec3& v);
		Vec3& operator*=(double t);
		Vec3& operator/=(double t);

		[[nodiscard]] double length() const { return std::sqrt(length_squared()); }
		[[nodiscard]] double length_squared() const;
		[[nodiscard]] bool near_zero() const;

		friend std::ostream& operator<<(std::ostream& out, const Vec3& v);
		friend Vec3 operator+(const Vec3& a, const Vec3& b);
		friend Vec3 operator-(const Vec3& a, const Vec3& b);
		friend Vec3 operator*(const Vec3& a, const Vec3& b);
		friend Vec3 operator*(double t, const Vec3& a);
		friend Vec3 operator*(const Vec3& a, const double t) { return t * a; }
		friend Vec3 operator/(const Vec3& a, const double t) { return (1.0 / t) * a; }
		friend double dot(const Vec3& a, const Vec3& b);
		friend Vec3 cross(const Vec3& a, const Vec3& b);
		friend Vec3 normalize(const Vec3& a) { return a / a.length(); }
	};

	using Point3 = Vec3;

	Vec3 random_unit_vector();
	Vec3 random_in_unit_disk();
	Vec3 random_on_hemisphere(const Vec3& normal);
	inline double distance(const Point3& a, const Point3& b) { return (b - a).length(); }
	inline Vec3 reflect(const Vec3& ray_dir, const Vec3& normal) { return ray_dir - 2 * normal * dot(ray_dir, normal); }
	Vec3 refract(const Vec3& ray_dir, const Vec3& normal, double refraction_ratio);
}

