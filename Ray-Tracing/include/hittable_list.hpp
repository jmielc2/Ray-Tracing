#pragma once

#include <vector>

#include "util.hpp"
#include "hittable.hpp"

namespace rt {
	class HittableList : public Hittable {
	private:
		AABB bbox;
		std::vector<std::shared_ptr<Hittable>> objects;

	public:
		HittableList();
		explicit HittableList(const std::shared_ptr<Hittable>& object);
		HittableList(const HittableList& other) = default;
		HittableList(HittableList&& other) = default;

		HittableList& operator=(const HittableList& other) = default;
		HittableList& operator=(HittableList&& other) = default;

		void clear();
		void add(const std::shared_ptr<Hittable>& object);
		std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override;
		const inline std::vector<std::shared_ptr<Hittable>>& objectList() const { return objects; }
		const inline AABB& bounding_box() const override { return bbox; }
	};
}

