#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <iostream>

template<class T = std::chrono::microseconds>
class Timer {
private:
	const std::chrono::high_resolution_clock::time_point start;
public:
	Timer() :
		start(std::chrono::high_resolution_clock::now())
	{}

	~Timer() {
		stop();
	}

	void stop() const {
		const auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Time: " << std::chrono::duration_cast<T>(end - start) << "\n";
	}
};

#endif
