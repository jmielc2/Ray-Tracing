#pragma once

#include <chrono>
#include <iostream>

namespace rt {
	template<class T = std::chrono::microseconds>
	class Timer {
		const std::chrono::high_resolution_clock::time_point start;
		bool stopped = false;
	public:
		Timer() :
			start(std::chrono::high_resolution_clock::now())
		{
		}

		~Timer() {
			stop();
		}

		void stop() {
			if (!stopped) [[likely]] {
				const auto end = std::chrono::high_resolution_clock::now();
				std::cout << "Time: " << std::chrono::duration_cast<T>(end - start) << "\n";
				stopped = true;
			}
		}
	};
}

