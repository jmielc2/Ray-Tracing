#include "hittable_list.hpp"

namespace rt {
	HittableList::HittableList() {
	}

	HittableList::HittableList(const std::shared_ptr<Hittable>& object) {
		add(object);
	}

	void HittableList::clear() {
		objects.clear();
	}

	void HittableList::add(const std::shared_ptr<Hittable>& object) {
		objects.emplace_back(object);
		bbox = AABB(bbox, object->bounding_box());
	}

	std::optional<HitRecord> HittableList::hit(const Ray& ray, const Interval& ray_t) const {
		bool has_collision = false;
		HitRecord record;
		double closest_collision_t = ray_t.max;

		for (const auto& hittable : objects) {
			auto result = hittable->hit(ray, Interval{ ray_t.min, closest_collision_t });
			if (result.has_value()) {
				has_collision = true;
				record = result.value();
				closest_collision_t = record.t;
			}
		}
		return has_collision ? std::optional<HitRecord>{ record } : std::optional<HitRecord>{};
	}

}