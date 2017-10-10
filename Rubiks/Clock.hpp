#ifndef CLOCK_HPP
#define CLOCK_HPP


#include <chrono>


class Clock
{
private:
	std::chrono::steady_clock::time_point mStartPoint;

public:
	Clock();
	~Clock() = default;
	Clock(Clock const &) = delete;
	Clock& operator=(Clock const &) = delete;

public:
	void restart();
	long long getElapsedTimeAsMilliseconds() const;


private:
	std::chrono::steady_clock::time_point getActualTimePoint() const;

};


#endif //CLOCK_HPP