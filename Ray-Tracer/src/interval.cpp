#include "interval.hpp"
#include "util.hpp"

namespace rt {
	const Interval Interval::empty(+infinity, -infinity);

	const Interval Interval::universe(-infinity, +infinity);

	double Interval::size() const {
		return max - min;
	}

	bool Interval::contains(const double x) const {
		return min <= x && max >= x;
	}

	bool Interval::surrounds(const double x) const {
		return min < x && max > x;
	}

	Interval Interval::expand(const double delta) const {
		const auto padding = delta / 2.0;
		return { min - padding, max + padding };
	}

	double Interval::clamp(const double x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
}

