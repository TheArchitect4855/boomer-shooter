#pragma once

#include <chrono>

class Timer {
	std::chrono::steady_clock m_Clock;
	std::chrono::steady_clock::time_point m_Start;
public:
	Timer();
	float reset();
};
