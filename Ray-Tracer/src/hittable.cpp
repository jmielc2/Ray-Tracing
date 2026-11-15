#include "hittable.hpp"

namespace rt {
	void HitRecord::set_face_normal(const Ray& r, const Vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = (front_face) ? outward_normal : -outward_normal;
	}

	std::optional<HitRecord> Translate::hit(const Ray& ray, const Interval& ray_t) const {
		const auto offset_ray = Ray(ray.origin() - offset, ray.direction(), ray.time());
		auto hit_result = object->hit(offset_ray, ray_t);
		if (hit_result.has_value()) {
			hit_result->point += offset;
		}
		return hit_result;
	}

	RotateY::RotateY(std::shared_ptr<Hittable> object, double angle) : object(std::move(object)) {
		const auto radian = degrees_to_radians(angle);
		sin_theta = std::sin(radian);
		cos_theta = std::cos(radian);

		// Get rotated points and find min and max along each dimension
		if (object == nullptr) {
			bbox = AABB::empty;
			return;
		}
		const auto& box = object->bounding_box();

		auto min = Point3(infinity, box.y.min, infinity);
		auto max = Point3(-infinity, box.y.max, -infinity);

		for (auto i = 0; i < 2; i++) {
			const auto z = box.z.min + i * box.z.size();
			for (auto j = 0; j < 2; j++) {
				const auto x = box.x.min + j * box.x.size();
				for (auto k = 0; k < 2; k++) {
					const auto rotated = rotate_y(
						Vec3(x, box.y.min + k * box.y.size(), z),
						cos_theta, sin_theta
					);

					for (auto r = 0; r < 3; r++) {
						const auto axis = to_axis(r);
						min[axis] = std::fmin(min[axis], rotated[axis]);
						max[axis] = std::fmax(max[axis], rotated[axis]);
					}
				}
			}
		}
		bbox = AABB(min, max);
	}

	std::optional<HitRecord> RotateY::hit(const Ray& ray, const Interval& ray_t) const {
		// Transform to object space
		const auto rotated_ray = Ray(
			rotate_y(ray.origin(), cos_theta, sin_theta),
			rotate_y(ray.direction(), cos_theta, sin_theta),
			ray.time()
		);

		// Run collision
		auto hit_record = object->hit(rotated_ray, ray_t);
		if (hit_record.has_value()) {
			hit_record->point = rotate_y(hit_record->point, cos_theta, -sin_theta);
			hit_record->normal = rotate_y(hit_record->normal, cos_theta, -sin_theta);
		}
		return hit_record;
	}

	RotateZ::RotateZ(std::shared_ptr<Hittable> object, double angle) : object(std::move(object)) {
		const auto radian = degrees_to_radians(angle);
		sin_theta = std::sin(radian);
		cos_theta = std::cos(radian);

		// Get rotated points and find min and max along each dimension
		if (object == nullptr) {
			bbox = AABB::empty;
			return;
		}
		const auto& box = object->bounding_box();

		auto min = Point3(infinity, infinity, box.z.min);
		auto max = Point3(-infinity, -infinity, box.z.max);

		for (auto i = 0; i < 2; i++) {
			for (auto j = 0; j < 2; j++) {
				for (auto k = 0; k < 2; k++) {
					const auto x = box.x.min + i * box.x.size();
					const auto y = box.y.min + j * box.y.size();

					const auto rotated = rotate_z(
						Vec3(x, y, box.z.min + k * box.z.size()),
						cos_theta, sin_theta
					);

					for (auto r = 0; r < 3; r++) {
						const auto axis = to_axis(r);
						min[axis] = std::fmin(min[axis], rotated[axis]);
						max[axis] = std::fmax(max[axis], rotated[axis]);
					}
				}
			}
		}
		bbox = AABB(min, max);
	}

	std::optional<HitRecord> RotateZ::hit(const Ray& ray, const Interval& ray_t) const {
		// Transform to object space
		const auto rotated_ray = Ray(
			rotate_z(ray.origin(), cos_theta, sin_theta),
			rotate_z(ray.direction(), cos_theta, sin_theta),
			ray.time()
		);

		// Run collision
		auto hit_record = object->hit(rotated_ray, ray_t);
		if (hit_record.has_value()) {
			hit_record->point = rotate_z(hit_record->point, cos_theta, -sin_theta);
			hit_record->normal = rotate_z(hit_record->normal, cos_theta, -sin_theta);
		}
		return hit_record;
	}

	RotateX::RotateX(std::shared_ptr<Hittable> object, double angle) : object(std::move(object)) {
		const auto radian = degrees_to_radians(angle);
		sin_theta = std::sin(radian);
		cos_theta = std::cos(radian);

		// Get rotated points and find min and max along each dimension
		if (object == nullptr) {
			bbox = AABB::empty;
			return;
		}
		const auto& box = object->bounding_box();

		auto min = Point3(infinity, infinity, box.z.min);
		auto max = Point3(-infinity, -infinity, box.z.max);

		for (auto i = 0; i < 2; i++) {
			for (auto j = 0; j < 2; j++) {
				for (auto k = 0; k < 2; k++) {
					const auto x = box.x.min + i * box.x.size();
					const auto z = box.z.min + j * box.z.size();

					const auto rotated = rotate_x(
						Vec3(x, box.y.min + k * box.y.size(), z),
						cos_theta, sin_theta
					);

					for (auto r = 0; r < 3; r++) {
						const auto axis = to_axis(r);
						min[axis] = std::fmin(min[axis], rotated[axis]);
						max[axis] = std::fmax(max[axis], rotated[axis]);
					}
				}
			}
		}
		bbox = AABB(min, max);
	}

	std::optional<HitRecord> RotateX::hit(const Ray& ray, const Interval& ray_t) const {
		// Transform to object space
		const auto rotated_ray = Ray(
			rotate_x(ray.origin(), cos_theta, sin_theta),
			rotate_x(ray.direction(), cos_theta, sin_theta),
			ray.time()
		);

		// Run collision
		auto hit_record = object->hit(rotated_ray, ray_t);
		if (hit_record.has_value()) {
			hit_record->point = rotate_x(hit_record->point, cos_theta, -sin_theta);
			hit_record->normal = rotate_x(hit_record->normal, cos_theta, -sin_theta);
		}
		return hit_record;
	}
}

