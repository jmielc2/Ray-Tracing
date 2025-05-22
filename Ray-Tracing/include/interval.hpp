#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "util.hpp"

class Interval {
public:
	double min, max;

	Interval() :
		min(+infinity),
		max(-infinity)
	{}
	Interval(double min, double max) :
		min(min),
		max(max)
	{}
	Interval(const Interval& a, const Interval& b) : 
		min(std::min(a.min, b.min)),
		max(std::max(a.max, b.max))
	{}

	double size() const { return max - min; }
	bool contains(double x) const { return min <= x && max >= x; }
	bool surrounds(double x) const { return min < x && max > x; }
	Interval expand(double delta) const {
		const auto padding = delta / 2.0;
		return Interval(min - padding, max + padding);
	}
	double clamp(double x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	static const Interval empty, universe;
};

const Interval Interval::empty(+infinity, -infinity);
const Interval Interval::universe(-infinity, +infinity);

#endif