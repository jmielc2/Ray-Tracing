#pragma once

#include <chrono>
#include <iostream>

namespace rt {
	template<class T = std::chrono::microseconds>
	class Timer {
		const std::chrono::high_resolution_clock::time_point start;
	public:
		Timer() :
			start(std::chrono::high_resolution_clock::now())
		{
		}

		~Timer() {
			stop();
		}

		void stop() const {
			const auto end = std::chrono::high_resolution_clock::now();
			std::cout << "Time: " << std::chrono::duration_cast<T>(end - start) << "\n";
		}
	};
}

