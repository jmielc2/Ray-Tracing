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
        const auto n = cross(u, v);
        normal = normalize(n);
        w = n / n.length_squared();
        D = dot(normal, corner);
        Quad::set_bounding_box();
    }

    void Quad::set_bounding_box() {
        const auto box_diagonal1 = AABB(corner, corner + u + v);
        const auto box_diagonal2 = AABB(corner + u, corner + v);
        bbox = AABB(box_diagonal1, box_diagonal2);
    }

    std::pair<double, double> Quad::getQuadUV(const Point3& point) const {
        const auto translated_point = point - corner;
        const auto a = dot(w, cross(translated_point, v));
        const auto b = dot(w, cross(u, translated_point));
        return { a, b };
    }

    bool Quad::is_interior(const double u, const double v) const {
        constexpr Interval zero_to_one(0, 1);
        if (!zero_to_one.contains(u) || !zero_to_one.contains(v)) {
            return false;
        }
        return true;
    }

    std::optional<HitRecord> Quad::hit(const Ray& ray, const Interval& ray_t) const {
        const auto normal_dot_direction = dot(normal, ray.direction());
        if (std::fabs(normal_dot_direction) < 1e-8) [[unlikely]] {
            return {};
        }
        const auto normal_dot_origin = dot(normal, ray.origin());
        const auto t = (D - normal_dot_origin) / normal_dot_direction;
        const auto point = ray.at(t);
        if (!ray_t.contains(t)) {
            return {};
        }
        const auto [u, v] = getQuadUV(point);
        if (!is_interior(u, v)) {
            return {};
        }

        HitRecord record {
            .point = point,
            .material = material,
            .t = t,
            .u = u,
            .v = v,
        };
        record.set_face_normal(ray, normal);
        return { record };
    }
}
