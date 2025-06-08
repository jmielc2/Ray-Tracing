#include "interval.hpp"
#include "util.hpp"

namespace rt {
	const Interval Interval::empty(+infinity, -infinity);

	const Interval Interval::universe(-infinity, +infinity);

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
}

