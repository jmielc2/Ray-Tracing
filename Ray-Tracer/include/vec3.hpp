#pragma once

#include <iostream>

namespace rt {
	class Vec3 {
	private:
		double e[3];

	public:
		constexpr Vec3() :
			e{ 0.0, 0.0, 0.0 }
		{
		}
		constexpr Vec3(double e0, double e1, double e2) :
			e{ e0, e1, e2 }
		{
		}

		static Vec3 random();
		static Vec3 random(double min, double max);

		const double& x() const;
		const double& y() const;
		const double& z() const;

		Vec3 operator-() const;
		double operator[](int i) const;
		double& operator[](int i);
		Vec3& operator+=(const Vec3& v);
		Vec3& operator-=(const Vec3& v);
		Vec3& operator*=(double t);
		Vec3& operator/=(double t);

		double length() const;
		double length_squared() const;
		bool near_zero() const;

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

	Vec3 random_unit_vector();
	Vec3 random_in_unit_disk();
	Vec3 random_on_hemisphere(const Vec3& normal);
	double distance(const Point3& a, const Point3& b);
	Vec3 reflect(const Vec3& ray_dir, const Vec3& normal);
	Vec3 refract(const Vec3& ray_dir, const Vec3& normal, double refraction_ratio);
}

