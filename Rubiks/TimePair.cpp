#include "TimePair.hpp"


TimePair::TimePair()
	: TimePair(0, 0)
{
}


TimePair::TimePair(int _seconds, int _hundredths)
	: seconds(_seconds), hundredths(_hundredths)
{
}


float TimePair::getTimeAsFloatInSeconds() const
{
	return (static_cast<float>(seconds) + static_cast<float>(hundredths) / 100.f);
}


TimePair TimePair::trafoMicrosecsIntoTimePair(long long const & timeInMicroseconds)
{
	int solvingTimeInHundredthOfASecond = timeInMicroseconds / 10000;
	int hundredths = solvingTimeInHundredthOfASecond % 100;
	int seconds = solvingTimeInHundredthOfASecond / 100;
	return TimePair(seconds, hundredths);
}


TimePair TimePair::trafoSecondsIntoTimePair(float timeInSeconds)
{
	return TimePair::trafoMicrosecsIntoTimePair(static_cast<long long>(timeInSeconds * 1000000.f));
}


std::ostream& operator<<(std::ostream& oStream, TimePair const & timePair)
{
	//Handle case when timePair is 0.0! This codes the unavailable case (e.g. in average calculation with empty listOfRecords)
	if (timePair.seconds == 0 && timePair.hundredths == 0)
	{
		oStream << "N/A";
		return oStream;
	}

	//Handle normal case
	std::stringstream hundredthsStringstream;
	if (timePair.hundredths < 10)
	{
		hundredthsStringstream << '0';
	}
	hundredthsStringstream << timePair.hundredths;

	oStream << timePair.seconds << "." << hundredthsStringstream.str() << " s";
	return oStream;
}


bool operator<(TimePair const & t1, TimePair const & t2)
{
	if (t1.seconds < t2.seconds)
	{
		return true;
	}
	else if (t1.seconds == t2.seconds)
	{
		return (t1.hundredths < t2.hundredths);
	}
	else
	{
		return false;
	}
}

