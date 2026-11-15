#pragma once

#include "hittable.hpp"
#include "texture.hpp"
#include "material.hpp"

namespace rt {
    class ConstantMedium : public Hittable {
    public:
        ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> texture) :
            boundary(std::move(boundary)), neg_inv_density(-1.0 / density), phase_function(std::make_shared<Isotropic>(texture))
        {}

        ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Color& albedo) :
            boundary(std::move(boundary)), neg_inv_density(-1.0 / density), phase_function(std::make_shared<Isotropic>(albedo))
        {}

        [[nodiscard]] std::optional<HitRecord> hit(const Ray &ray, const Interval &ray_t) const override;
        [[nodiscard]] const AABB & bounding_box() const override { return boundary->bounding_box(); };
    private:
        std::shared_ptr<Hittable> boundary;
        double neg_inv_density;
        std::shared_ptr<Material> phase_function;
    };
}
