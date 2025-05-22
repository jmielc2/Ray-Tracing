#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "util.hpp"

class Material;

struct HitRecord {
	Point3 point;
	Vec3 normal;
	std::shared_ptr<Material> material;
	double t = 0.0;
	double u = 0.0;
	double v = 0.0;
	bool front_face = false;

	// assumes outward_normal is unit vector
	void set_face_normal(const Ray& r, const Vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const = 0;
	virtual const AABB& bounding_box() const = 0;
};

#endif