#include "TimePair.hpp"


TimePair::TimePair()
	: TimePair(0, 0)
{
}


TimePair::TimePair(int _seconds, int _hundredths)
	: seconds(_seconds), hundredths(_hundredths)
{
}


TimePair TimePair::trafoMicrosecsIntoTimePair(long long const & timeInMicroseconds)
{
	int solvingTimeInHundredthOfASecond = timeInMicroseconds / 10000;
	int hundredths = solvingTimeInHundredthOfASecond % 100;
	int seconds = solvingTimeInHundredthOfASecond / 100;
	return TimePair(seconds, hundredths);
}


std::ostream& operator<<(std::ostream& oStream, TimePair const & timePair)
{
	std::stringstream hundredthsStringstream;
	if (timePair.hundredths < 10)
	{
		hundredthsStringstream << '0';
	}
	hundredthsStringstream << timePair.hundredths;

	oStream << timePair.seconds << "." << hundredthsStringstream.str() << " s";
	return oStream;
}

