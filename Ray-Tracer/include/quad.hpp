#pragma once

#include "hittable.hpp"
#include "interval.hpp"
#include "aabb.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include <optional>

namespace rt {
    class Quad : public Hittable {
        Point3 corner;
        Vec3 u, v;
        std::shared_ptr<Material> material;
        AABB bbox;
    public:
        Quad(const Point3& corner, const Vec3& u, const Vec3& v, const std::shared_ptr<Material>& material);

        virtual void set_bounding_box();

        const AABB& bounding_box() const override;
        std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override;
    };
}
