#include "quad.hpp"

#include <memory>

#include "material.hpp"
#include "vec3.hpp"

namespace rt {
    Quad::Quad(const Point3& corner, const Vec3& u, const Vec3& v, const std::shared_ptr<Material>& material) :
        corner(corner),
        u(u),
        v(v),
        material(material)
    {
        Quad::set_bounding_box();
    }

    void Quad::set_bounding_box()
    {
        const auto box_diagonal1 = AABB(corner, corner + u + v);
        const auto box_diagonal2 = AABB(corner + u, corner + v);
        bbox = AABB(box_diagonal1, box_diagonal2);
    }

    const AABB& Quad::bounding_box() const
    {
        return bbox;
    }

    std::optional<HitRecord> Quad::hit(const Ray& ray, const Interval& ray_t) const
    {
        return {};
    }
}
