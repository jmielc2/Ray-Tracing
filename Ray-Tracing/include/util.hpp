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

// Helper Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = std::numbers::pi_v<double>;
static std::uniform_real_distribution<double> distribution(0.0, 1.0);
static std::mt19937 generator{};

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

// Helper Functions
double random_double() {
	return distribution(generator);
}

double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}

constexpr double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}


// Common Headers
#include "vec3.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "color.hpp"
#include "aabb.hpp"
#include "timer.hpp"
