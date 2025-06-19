#pragma once

#include <vector>
#include "aabb.hpp"
#include "hittable.hpp"

namespace rt {
	class HittableList final : public Hittable {
		AABB bbox;
		std::vector<std::shared_ptr<Hittable>> objects;

	public:
		HittableList();
		explicit HittableList(const std::shared_ptr<Hittable>& object);
		HittableList(const HittableList&) = delete;
		HittableList(HittableList&&) noexcept = default;
		~HittableList() override = default;

		HittableList& operator=(const HittableList&) = delete;
		HittableList& operator=(HittableList&& other) noexcept = delete;

		void clear();
		void add(const std::shared_ptr<Hittable>& object);
		[[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override;
		[[nodiscard]] const std::vector<std::shared_ptr<Hittable>>& objectList() const { return objects; }
		[[nodiscard]] const AABB& bounding_box() const override { return bbox; }
	};
}

