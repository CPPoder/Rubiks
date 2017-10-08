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


std::chrono::steady_clock::time_point Clock::getActualTimePoint() const
{
	return std::chrono::steady_clock::now();
}



