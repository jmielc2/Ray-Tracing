#include "interval.hpp"

const Interval Interval::empty(+infinity, -infinity);

const Interval Interval::universe(-infinity, +infinity);

Interval::Interval() :
	min(+infinity),
	max(-infinity)
{ }

Interval::Interval(double min, double max) :
	min(min),
	max(max)
{ }

Interval::Interval(Interval a, Interval b) :
	min(std::min(a.min, b.min)),
	max(std::max(a.max, b.max))
{ }

double Interval::size() const {
	return max - min;
}

bool Interval::contains(double x) const { 
	return min <= x && max >= x;
}

bool Interval::surrounds(double x) const { 
	return min < x && max > x;
}

Interval Interval::expand(double delta) const {
	const auto padding = delta / 2.0;
	return Interval(min - padding, max + padding);
}

double Interval::clamp(double x) const {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
