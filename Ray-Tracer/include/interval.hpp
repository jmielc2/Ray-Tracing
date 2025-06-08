#pragma once

#include <algorithm>

namespace rt {

	extern const double infinity;

	class Interval {
	public:
		double min, max;

		constexpr Interval() : min(+infinity), max(-infinity) {}
		constexpr Interval(double min, double max) : min(min), max(max) {}
		constexpr Interval(Interval a, Interval b) : min(std::min(a.min, b.min)), max(std::max(a.max, b.max)) {}

		double size() const;
		bool contains(double x) const;
		bool surrounds(double x) const;
		Interval expand(double delta) const;
		double clamp(double x) const;

		static const Interval empty, universe;
	};
}

