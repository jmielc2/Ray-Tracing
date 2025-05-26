#pragma once

#include "util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

#include <algorithm>

namespace rt {
	class BVHNode : public Hittable {
	private:
		std::shared_ptr<Hittable> left;
		std::shared_ptr<Hittable> right;
		AABB bbox;
	public:
		explicit BVHNode(const HittableList& list);
		BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);

		std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override;
		const inline AABB& bounding_box() const override { return bbox; }
	};
}

