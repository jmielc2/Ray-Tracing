#pragma once

#include <vector>
#include "vec3.hpp"

namespace rt {
    class Perlin {
        static constexpr int point_count = 256;
        std::vector<Vec3> random_vecs;
        std::vector<int> perm_x;
        std::vector<int> perm_y;
        std::vector<int> perm_z;
    public:
        Perlin();

        [[nodiscard]] double noise(const Point3& point) const;
        [[nodiscard]] double turbulence(const Point3& point, int depth) const;
    };
}