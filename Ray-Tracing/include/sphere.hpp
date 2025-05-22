#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "util.hpp"
#include "hittable.hpp"

class Sphere : public Hittable {
private:
	Ray center;
	double radius;
	std::shared_ptr<Material> material;
	AABB bbox;
public:
	Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& material) : 
	  center(center, Vec3(0, 0, 0)),
	  radius(std::fmax(0, radius)),
	  material(material)
	{
		const auto r_vec = Vec3(radius, radius, radius);
		bbox = AABB{
			center - r_vec,
			center + r_vec
		};
	}

	Sphere(const Point3& start, const Point3& end, double radius, const std::shared_ptr<Material>& material) :
		center(start, end - start),
		radius(std::fmax(0, radius)),
		material(material)
	{
		const auto r_vec = Vec3(radius, radius, radius);
		const AABB start_bounding_box{
			start - r_vec,
			start + r_vec
		};
		const AABB end_bounding_box{
			end - r_vec,
			end + r_vec
		};
		bbox = AABB{
			start_bounding_box,
			end_bounding_box
		};
	}

	std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override {
		Point3 current_center = center.at(ray.time());
		Vec3 qc = current_center - ray.origin();
		double a = ray.direction().length_squared();
		double h = dot(ray.direction(), qc);
		double c = qc.length_squared() - radius * radius;
		double determinant = h * h - a * c;
		if (determinant < 0) {
			return {};
		}
		double t = (h - std::sqrt(determinant)) / a;
		if (!ray_t.surrounds(t)) {
			t = (h + std::sqrt(determinant)) / a;
			if (!ray_t.surrounds(t)) {
				return {};
			}
		}

		HitRecord record;
		record.t = t;
		record.material = material;
		record.point = ray.at(t);
		Vec3 outward_normal = (record.point - current_center) / radius;
		record.set_face_normal(ray, outward_normal);
		return { record };
	}

	const AABB& bounding_box() const override {
		return bbox;
	}
};

#endif