#pragma once

#include "util.hpp"

class Interval {
public:
	double min, max;

	Interval();
	Interval(double min, double max);
	Interval(Interval a, Interval b);

	double size() const;
	bool contains(double x) const;
	bool surrounds(double x) const;
	Interval expand(double delta) const;
	double clamp(double x) const;

	static const Interval empty, universe;
};

