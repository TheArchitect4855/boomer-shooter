#include "timer.hpp"

using namespace std::chrono;

Timer::Timer() {
	m_Start = m_Clock.now();
}

float Timer::reset() {
	time_point now = m_Clock.now();
	duration<float> elapsed = now - m_Start;
	m_Start = now;
	return elapsed.count();
}
