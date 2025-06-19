#include "bvh.hpp"

namespace rt {
	BVHNode::BVHNode(const HittableList& list) :
		BVHNode(list.objectList(), 0, list.objectList().size())
	{
	}

	BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, const size_t start, const size_t end) {
		switch (const size_t span = end - start) {
		case(1):
			left = right = objects[start];
			break;
		case(2):
			left = objects[start];
			right = objects[start + 1];
			break;
		default:
			bbox = AABB::empty;
			for (const auto& object : objects) {
				bbox = AABB(bbox, object->bounding_box());
			}
			const int axis = bbox.longest_axis();
			const auto comparator = [axis](const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
				const Interval& a_interval = a->bounding_box().axis_interval(axis);
				const Interval& b_interval = b->bounding_box().axis_interval(axis);
				return a_interval.min < b_interval.min;
				};
			std::vector subset(start + objects.begin(), objects.begin() + end);
			std::sort(subset.begin(), subset.end(), comparator);
			const auto half = static_cast<size_t>(span * 0.5);
			left = std::make_shared<BVHNode>(subset, 0, half);
			right = std::make_shared<BVHNode>(subset, half, subset.size());
			break;
		}
	}

	std::optional<HitRecord> BVHNode::hit(const Ray& ray, const Interval& ray_t) const {
		if (!bbox.hit(ray, ray_t)) {
			return {};
		}
		const auto left_hit = left->hit(ray, ray_t);
		const auto right_hit = right->hit(ray, ray_t);
		if (left_hit.has_value() || right_hit.has_value()) {
			if (left_hit.has_value() && right_hit.has_value()) {
				return (left_hit.value().t < right_hit.value().t) ? left_hit : right_hit;
			}
			if (left_hit.has_value()) {
				return left_hit;
			}
			return right_hit;
		}
		return {};
	}
}
