#ifndef TIMEPAIR_HPP
#define TIMEPAIR_HPP

#include <iostream>
#include <sstream>


struct TimePair
{
public:
	int seconds;
	int hundredths;

public:
	TimePair();
	TimePair(int _seconds, int _hundredths);

public:
	float getTimeAsFloatInSeconds() const;

public:
	static TimePair trafoMicrosecsIntoTimePair(long long const & timeInMicroseconds);
	static TimePair trafoSecondsIntoTimePair(float timeInSeconds);

public:
	friend std::ostream& operator<<(std::ostream& oStream, TimePair const & timePair);
	friend bool operator<(TimePair const & t1, TimePair const & t2);
};




#endif //TIMEPAIR_HPP

