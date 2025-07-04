#include "sphere.hpp"

namespace rt {
	Sphere::Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& material) :
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

	Sphere::Sphere(const Point3& start, const Point3& end, double radius, const std::shared_ptr<Material>& material) :
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

	std::pair<double, double> Sphere::getSphereUV(const Point3& point) const {
		const Point3 translated = (point - center.origin()) / radius;
		const double theta = std::acos(-translated.y());
		const double phi = std::atan2(-translated.z(), translated.x()) + pi;
		return { phi * 0.5 / pi, theta / pi};
	}

	std::optional<HitRecord> Sphere::hit(const Ray& ray, const Interval& ray_t) const {
		const Point3 current_center = center.at(ray.time());
		const Vec3 qc = current_center - ray.origin();
		const double a = ray.direction().length_squared();
		const double h = dot(ray.direction(), qc);
		const double c = qc.length_squared() - radius * radius;
		const double determinant = h * h - a * c;
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
		const auto [u, v] = getSphereUV(record.point);
		record.u = u;
		record.v = v;
		const Vec3 outward_normal = (record.point - current_center) / radius;
		record.set_face_normal(ray, outward_normal);
		return { record };
	}
}