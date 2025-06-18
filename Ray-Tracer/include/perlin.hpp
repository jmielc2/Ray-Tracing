#pragma once

#include "util.hpp"

namespace rt {
    class Perlin {
    private:
        static const int point_count = 256;
        std::vector<double> random_floats;
        std::vector<int> perm_x;
        std::vector<int> perm_y;
        std::vector<int> perm_z;

        static void permute(std::vector<int>& a);        
    public:
        Perlin();

        double noise(const Point3& point) const;
    };
}