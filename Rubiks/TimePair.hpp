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
	static TimePair trafoMicrosecsIntoTimePair(long long const & timeInMicroseconds);

public:
	friend std::ostream& operator<<(std::ostream& oStream, TimePair const & timePair);
};




#endif //TIMEPAIR_HPP

