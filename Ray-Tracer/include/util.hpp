#pragma once

// Common Standard Library Headers
#include <fstream>
#include <string>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>
#include <optional>
#include <random>
#include <utility>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <concepts>
#include <mutex>

namespace rt {
	// Helper Constants
	constexpr double infinity = std::numeric_limits<double>::infinity();
	constexpr double pi = std::numbers::pi_v<double>;
	constexpr int X_AXIS = 0;
	constexpr int Y_AXIS = 1;
	constexpr int Z_AXIS = 2;
	extern std::mutex ioMutex;
	
	// Helper Functions
	double random_double();
	double random_double(double min, double max);
	int random_int(int min, int max);
	constexpr double degrees_to_radians(double degrees) {
		return degrees * pi / 180.0;
	}

	template<std::totally_ordered T>
    constexpr auto clamp(T x, T low, T high) {
        if (x < low) return low;
        if (x < high) return x;
        return high - 1;
    }

	template<std::totally_ordered T>
	constexpr auto map(T x, T ilow, T ihigh, T olow, T ohigh) {
		if (x < ilow) return olow;
		if (x >= ihigh) return ohigh;
		return ((x - ilow) / (ihigh - ilow)) * (ohigh - olow) + olow;
	}

	inline auto lockIO() { return std::lock_guard<std::mutex>(ioMutex); }
}


// Common Headers
#include "vec3.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "color.hpp"
#include "aabb.hpp"
#include "timer.hpp"
