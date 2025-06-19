#include "util.hpp"
#include <algorithm>
#include <numeric>
#include "perlin.hpp"

namespace rt {
    void Perlin::permute(std::vector<int>& a) {
        std::for_each(a.rbegin(), a.rend(), [&a](int& element){
            const int index = std::distance(a.data(), &element) - 1;
            if (index <= 0) {
                return;
            }
            const int target = random_int(0, index);
            const int temp = a[index];
            a[index] = a[target];
            a[target] = temp;
        });
    }

    Perlin::Perlin() :
        random_floats(point_count),
        perm_x(point_count),
        perm_y(point_count),
        perm_z(point_count)
    {
        std::ranges::generate(random_floats, []{
            return random_double();
        });
        std::iota(perm_x.begin(), perm_x.end(), 0);
        std::iota(perm_y.begin(), perm_y.end(), 0);
        std::iota(perm_z.begin(), perm_z.end(), 0);
        permute(perm_x);
        permute(perm_y);
        permute(perm_z);   
    }

    double Perlin::noise(const Point3& point) const {
        int i = static_cast<int>(4 * point.x()) & 255;
        int j = static_cast<int>(4 * point.y()) & 255;
        int k = static_cast<int>(4 * point.z()) & 255;
        return random_floats[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }
}
