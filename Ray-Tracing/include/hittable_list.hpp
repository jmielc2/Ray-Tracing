#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <vector>

#include "util.hpp"
#include "hittable.hpp"

class HittableList : public Hittable {
private:
	AABB bbox;
	std::vector<std::shared_ptr<Hittable>> objects;
public:

	HittableList() {}
	explicit HittableList(const std::shared_ptr<Hittable>& object) {
		add(object);
	}

	void clear() {
		objects.clear();
	}

	void add(const std::shared_ptr<Hittable>& object) {
		objects.emplace_back(object);
		bbox = AABB(bbox, object->bounding_box());
	}

	std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override {
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
		return has_collision? std::optional<HitRecord>{ record } : std::optional<HitRecord>{};
	}

	const std::vector<std::shared_ptr<Hittable>>& objectList() const {
		return objects;
	}

	const AABB& bounding_box() const override {
		return bbox;
	}
};

#endif