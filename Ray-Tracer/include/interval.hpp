#pragma once

#include <algorithm>
#include "util.hpp"

namespace rt {

	class Interval {
	public:
		double min, max;

		constexpr Interval() : min(+infinity), max(-infinity) {}
		constexpr Interval(const double min, const double max) : min(min), max(max) {}
		constexpr Interval(const Interval a, const Interval b) : min(std::min(a.min, b.min)), max(std::max(a.max, b.max)) {}

		[[nodiscard]] double size() const;
		[[nodiscard]] bool contains(double x) const;
		[[nodiscard]] bool surrounds(double x) const;
		[[nodiscard]] Interval expand(double delta) const;
		[[nodiscard]] double clamp(double x) const;

		static const Interval empty, universe;
	};
}

