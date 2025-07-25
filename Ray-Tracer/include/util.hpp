#pragma once

// Common Standard Library Headers
#include <limits>
#include <numbers>
#include <random>
#include <utility>
#include <cstdlib>
#include <concepts>
#include <mutex>

namespace rt {
	// Helper Constants
	constexpr double infinity = std::numeric_limits<double>::infinity();
	constexpr double pi = std::numbers::pi_v<double>;
	enum class Axis {
		X = 0,
		Y = 1,
		Z = 2
	};
	extern std::mutex ioMutex;

	// Helper Functions

	auto random_double() -> double;
	auto random_double(double min, double max) -> double;
	auto random_int(int min, int max) -> int;
	constexpr double degrees_to_radians(const double degrees) {
		return degrees * pi / 180.0;
	}
	constexpr int to_index(const Axis a) {
		return std::to_underlying(a);
	}

	template<class O, std::totally_ordered T>
    constexpr O clamp(T x, T low, T high) {
        if (x < low) return low;
        if (x < high) return x;
        return static_cast<O>(high - 1);
    }

	template<class O, std::totally_ordered T>
	constexpr O map(T x, T input_low, T input_high, T output_low, T output_high) {
		if (x < input_low) return output_low;
		if (x >= input_high) return output_high;
		return static_cast<O>((x - input_low) / (input_high - input_low) * (output_high - output_low) + output_low);
	}

	inline auto lockIO() { return std::lock_guard(ioMutex); }
}
