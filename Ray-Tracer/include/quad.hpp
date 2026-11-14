#pragma once

#include "hittable.hpp"
#include <hittable_list.hpp>
#include "interval.hpp"
#include "aabb.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "ray.hpp"

#include <optional>

namespace rt {
    class Quad : public Hittable {
        Point3 corner;
        Vec3 u, v; // front face is cross product of u and v
        Vec3 normal;
        Vec3 w; // Normal divided by its length squared
        double D;  // The dot product of any point on the plane and the normal will always equal this value
        std::shared_ptr<Material> material;
        AABB bbox;

        [[nodiscard]] virtual bool is_interior(double u, double v) const;
        [[nodiscard]] std::pair<double, double> getQuadUV(const Point3& point) const;
    public:
        Quad(const Point3& corner, const Vec3& u, const Vec3& v, const std::shared_ptr<Material>& material);

        virtual void set_bounding_box();

        [[nodiscard]] const AABB& bounding_box() const override { return bbox; }
        [[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, const Interval& ray_t) const override;
    };

    std::shared_ptr<HittableList> box(const Point3& a, const Point3& b, std::shared_ptr<Material> material);
}
