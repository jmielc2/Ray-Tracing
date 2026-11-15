#include "constant_medium.hpp"

namespace rt {
    std::optional<HitRecord> ConstantMedium::hit(const Ray &ray, const Interval &ray_t) const {
        auto rec1 = boundary->hit(ray, Interval::universe);
        if (!rec1.has_value()) {
            return {};
        }
        auto rec2 = boundary->hit(ray, Interval(rec1->t + 0.0001, infinity));
        if (!rec2.has_value()) {
            return {};
        }

        if (rec1->t < ray_t.min) rec1->t = std::fmax(0, ray_t.min);
        if (rec2->t > ray_t.max) rec2->t = ray_t.max;
        if (rec1->t >= rec2->t) {
            return {};
        }

        const auto ray_length = ray.direction().length();
        const auto distance_inside_boundary = (rec2->t - rec1->t) * ray_length;
        const auto hit_distance = neg_inv_density * std::log(random_double());

        if (hit_distance > distance_inside_boundary) {
            return {};
        }

        const auto t = rec1->t + hit_distance / ray_length;
        return HitRecord {
            .point = ray.at(t),
            .normal = Vec3(1, 0, 0),
            .material = phase_function,
            .t = t,
            .front_face = true,
        };
    }
}
