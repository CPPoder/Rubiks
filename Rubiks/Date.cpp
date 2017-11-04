#include "Date.hpp"


Date::Date()
{
	*this = Date::getCurrentDate();
}


Date::Date(unsigned char _day, unsigned char _month, unsigned short _year, unsigned char _hour, unsigned char _minute, unsigned char _second, Weekday _weekday)
	: day(_day), month(_month), year(_year), hour(_hour), minute(_minute), second(_second), weekday(_weekday)
{
}



unsigned char Date::getDay() const
{
	return day;
}

unsigned char Date::getMonth() const
{
	return month;
}

unsigned short Date::getYear() const
{
	return year;
}

unsigned char Date::getHour() const
{
	return hour;
}

unsigned char Date::getMinute() const
{
	return minute;
}

unsigned char Date::getSecond() const
{
	return second;
}

Date::Weekday Date::getWeekday() const
{
	return weekday;
}

float Date::getTimeInHours() const
{
	float hours = static_cast<float>(hour);
	float minutesAsHours = static_cast<float>(minute) / 60.f;
	float secondsAsHours = static_cast<float>(second) / 3600.f;
	return (hours + minutesAsHours + secondsAsHours);
}




Date Date::getCurrentDate()
{
	//Create stream
	std::stringstream stream;

	//Get stream text
	auto now = std::chrono::system_clock::now();
	auto time_t_now = std::chrono::system_clock::to_time_t(now);
	stream << std::ctime(&time_t_now);
	std::string text = stream.str();
	text.pop_back();

	//Cut at ' '. Be aware of double spaces!
	std::vector<std::string> vecOfStringsCuttedAtSpace = StrManip::cutStringAtChar(text, ' ');
	std::vector<std::string> vecOfCorrectStrings;
	for (auto const & string : vecOfStringsCuttedAtSpace)
	{
		if (!string.empty())
		{
			vecOfCorrectStrings.push_back(string);
		}
	}
	if (vecOfCorrectStrings.size() != 5u)
	{
		std::cout << "vecOfCorrectStrings does not have the correct size! Size is " << vecOfCorrectStrings.size() << "!" << std::endl;
	}

	//Extract data
	Weekday extractedWeekday = Date::extractWeekdayFromString(vecOfCorrectStrings.at(0));
	unsigned char extractedMonth = Date::extractMonthFromString(vecOfCorrectStrings.at(1));
	unsigned char extractedDay = static_cast<unsigned char>(Date::convertStringWithPossibleZerosAtBeginningIntoInt(vecOfCorrectStrings.at(2)));
	unsigned char extractedHour;
	unsigned char extractedMinute;
	unsigned char extractedSecond;
	Date::extractHourMinuteSecondFromString(vecOfCorrectStrings.at(3), extractedHour, extractedMinute, extractedSecond);
	unsigned short extractedYear = static_cast<unsigned short>(Date::convertStringWithPossibleZerosAtBeginningIntoInt(vecOfCorrectStrings.at(4)));

	//Return result
	return Date(extractedDay, extractedMonth, extractedYear, extractedHour, extractedMinute, extractedSecond, extractedWeekday);
}




Date::Weekday Date::makeWeekdayFromOwnCodingString(std::string const & string)
{
	if (string == "Mo")
	{
		return Weekday::Mo;
	}
	if (string == "Tu")
	{
		return Weekday::Tu;
	}
	if (string == "We")
	{
		return Weekday::We;
	}
	if (string == "Th")
	{
		return Weekday::Th;
	}
	if (string == "Fr")
	{
		return Weekday::Fr;
	}
	if (string == "Sa")
	{
		return Weekday::Sa;
	}
	if (string == "Su")
	{
		return Weekday::Su;
	}
}




Date::Weekday Date::extractWeekdayFromString(std::string const & string)
{
	if (string == "Mon")
	{
		return Weekday::Mo;
	}
	if (string == "Tue")
	{
		return Weekday::Tu;
	}
	if (string == "Wed")
	{
		return Weekday::We;
	}
	if (string == "Thu")
	{
		return Weekday::Th;
	}
	if (string == "Fri")
	{
		return Weekday::Fr;
	}
	if (string == "Sat")
	{
		return Weekday::Sa;
	}
	if (string == "Sun")
	{
		return Weekday::Su;
	}
}


unsigned char Date::extractMonthFromString(std::string const & string)
{
	if (string == "Jan")
	{
		return 1;
	}
	if (string == "Feb")
	{
		return 2;
	}
	if (string == "Mar")
	{
		return 3;
	}
	if (string == "Apr")
	{
		return 4;
	}
	if (string == "May")
	{
		return 5;
	}
	if (string == "Jun")
	{
		return 6;
	}
	if (string == "Jul")
	{
		return 7;
	}
	if (string == "Aug")
	{
		return 8;
	}
	if (string == "Sep")
	{
		return 9;
	}
	if (string == "Oct")
	{
		return 10;
	}
	if (string == "Nov")
	{
		return 11;
	}
	if (string == "Dec")
	{
		return 12;
	}
}


void Date::extractHourMinuteSecondFromString(std::string const & string, unsigned char & hour, unsigned char & minute, unsigned char & second)
{
	//Cut string into 3 parts
	std::string hourString = string.substr(0u, 2u);
	std::string minuteString = string.substr(3u, 2u);
	std::string secondString = string.substr(6u, 2u);

	//Extract values
	hour = static_cast<unsigned char>(Date::convertStringWithPossibleZerosAtBeginningIntoInt(hourString));
	minute = static_cast<unsigned char>(Date::convertStringWithPossibleZerosAtBeginningIntoInt(minuteString));
	second = static_cast<unsigned char>(Date::convertStringWithPossibleZerosAtBeginningIntoInt(secondString));
}


int Date::convertStringWithPossibleZerosAtBeginningIntoInt(std::string const & string)
{
	//Create newString variable
	std::string newString;

	//Filter out all beginning zeros
	bool beginningZeros = true;
	for (unsigned int i = 0; i < string.size(); ++i)
	{
		char c = string.at(i);
		if (beginningZeros)
		{
			if (c != '0')
			{
				beginningZeros = false;
			}
			else
			{
				continue;
			}
		}
		newString.push_back(c);
	}

	//If string consists of pure zeros, recreate one zero
	if (newString.empty())
	{
		newString = "0";
	}

	//Make int from newString
	return std::stoi(newString);
}



std::ostream& operator<<(std::ostream & oStream, Date const & date)
{
	oStream << "Weekday: " << date.weekday << std::endl;
	oStream << "Day: " << +date.day << std::endl;
	oStream << "Month: " << +date.month << std::endl;
	oStream << "Year: " << +date.year << std::endl;
	oStream << "Hour: " << +date.hour << std::endl;
	oStream << "Minute: " << +date.minute << std::endl;
	oStream << "Second: " << +date.second << std::endl;

	return oStream;
}


std::ostream& operator<<(std::ostream & oStream, Date::Weekday const & weekday)
{
	std::string name;
	switch (weekday)
	{
	case Date::Weekday::Mo:
		name = "Mo";
		break;
	case Date::Weekday::Tu:
		name = "Tu";
		break;
	case Date::Weekday::We:
		name = "We";
		break;
	case Date::Weekday::Th:
		name = "Th";
		break;
	case Date::Weekday::Fr:
		name = "Fr";
		break;
	case Date::Weekday::Sa:
		name = "Sa";
		break;
	case Date::Weekday::Su:
		name = "Su";
		break;
	}
	oStream << name;
	return oStream;
}

