#include "util.hpp"

namespace rt {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator{};
	std::mutex ioMutex;

	double random_double() {
		return distribution(generator);
	}

	double random_double(double min, double max) {
		return min + (max - min) * random_double();
	}

	int random_int(int min, int max) {
		return static_cast<int>(random_double(min, max + 1));
	}
}