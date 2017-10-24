#include "Clock.hpp"


Clock::Clock()
	: mStartPoint(this->getActualTimePoint())
{

}


void Clock::restart()
{
	mStartPoint = this->getActualTimePoint();
}


long long Clock::getElapsedTimeAsMilliseconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(this->getActualTimePoint() - mStartPoint).count();
}

long long Clock::getElapsedTimeAsMicroseconds() const
{
	return std::chrono::duration_cast<std::chrono::microseconds>(this->getActualTimePoint() - mStartPoint).count();
}

std::chrono::duration<double, std::micro> Clock::getElapsedTimeAsDuration() const
{
	return (this->getActualTimePoint() - mStartPoint);
}


std::chrono::steady_clock::time_point Clock::getActualTimePoint() const
{
	return std::chrono::steady_clock::now();
}



