#include "util.hpp"
#include <algorithm>
#include <numeric>
#include "perlin.hpp"

namespace rt {
    static void permute(std::vector<int>& a) {
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
        random_vecs(point_count),
        perm_x(point_count),
        perm_y(point_count),
        perm_z(point_count)
    {
        std::ranges::generate(random_vecs, []{
            return normalize(Vec3::random(-1, 1));
        });
        std::iota(perm_x.begin(), perm_x.end(), 0);
        std::iota(perm_y.begin(), perm_y.end(), 0);
        std::iota(perm_z.begin(), perm_z.end(), 0);
        permute(perm_x);
        permute(perm_y);
        permute(perm_z);   
    }

    static double perlin_interpolate(const Vec3 c[2][2][2], const double u, const double v, const double w) {
        const double uu = u * u * (3 - 2 * u);
        const double vv = v * v * (3 - 2 * v);
        const double ww = w * w * (3 - 2 * w);

        auto accumulator = 0.0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    Vec3 weight_vector(u - i, v - j, w - k);
                    accumulator += (uu * i + (1 - uu) * (1 - i)) *
                        (vv * j + (1 - vv) * (1 - j)) *
                        (ww * k + (1 - ww) * (1 - k)) *
                        dot(weight_vector, c[i][j][k]);
                }
            }
        }
        return accumulator;
    }

    double Perlin::noise(const Point3& point) const {
        const double u = point.x() - std::floor(point.x());
        const double v = point.y() - std::floor(point.y());
        const double w = point.z() - std::floor(point.z());

        const int x = std::floor(point.x());
        const int y = std::floor(point.y());
        const int z = std::floor(point.z());

        Vec3 c[2][2][2];
        for (int di = 0; di < 2; di++) {
            for (int dj = 0; dj < 2; dj++) {
                for (int dk = 0; dk < 2; dk++) {
                    c[di][dj][dk] = random_vecs[
                        perm_x[(x + di) & 255] ^
                        perm_y[(y + dj) & 255] ^
                        perm_z[(z + dk) & 255]
                    ];
                }
            }
        }
        return perlin_interpolate(c, u, v, w);
    }

    double Perlin::turbulence(const Point3& point, const int depth) const {
        auto accumulator = 0.0;
        auto weight = 1.0;
        auto temp_p = point;

        for (int i = 0; i < depth; i++) {
            accumulator += noise(temp_p) * weight;
            weight *= 0.5;
            temp_p *= 2.0;
        }
        return std::fabs(accumulator);
    }
}
