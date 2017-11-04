#include "Record.hpp"

Record::Record(Date const & _date, TimePair const & _time, TurnTypeOrder _scramble)
	: date(_date), time(_time), scramble(_scramble)
{
}


Date Record::getDate() const
{
	return date;
}


TimePair Record::getTime() const
{
	return time;
}


TurnTypeOrder Record::getScramble() const
{
	return scramble;
}


Record Record::makeRecordFromFileStrings(std::string const & date, std::string const & time, std::string const & scramble)
{
	//Get Date
	std::vector<std::string> dateCuttedAtComma = StrManip::cutStringAtChar(date, ',');
	std::string datePart = dateCuttedAtComma.at(0);
	std::string timePart = dateCuttedAtComma.at(1);
	std::string weekdayPart = dateCuttedAtComma.at(2);
	std::vector<std::string> datePartCuttedAtDot = StrManip::cutStringAtChar(datePart, '.');
	std::string dayPart = datePartCuttedAtDot.at(0);
	std::string monthPart = datePartCuttedAtDot.at(1);
	std::string yearPart = datePartCuttedAtDot.at(2);
	std::vector<std::string> timePartCuttedAtColon = StrManip::cutStringAtChar(timePart, ':');
	std::string hourPart = timePartCuttedAtColon.at(0);
	std::string minutePart = timePartCuttedAtColon.at(1);
	std::string secondPart = timePartCuttedAtColon.at(2);
	Date dateObject(std::stoi(dayPart), std::stoi(monthPart), std::stoi(yearPart), std::stoi(hourPart), std::stoi(minutePart), std::stoi(secondPart), Date::makeWeekdayFromOwnCodingString(weekdayPart));

	//Get Time
	TimePair timePair;
	std::vector<std::string> cuttedTime = StrManip::cutStringAtChar(time, '.');
	timePair.seconds = std::stoi(cuttedTime.at(0));
	timePair.hundredths = std::stoi(cuttedTime.at(1));

	//Get Scramble
	TurnTypeOrder turns;
	if (!Turn::isStringConvertibleIntoTurns(scramble, turns))
	{
		std::cout << "In Record::makeRecordFromFileStrings(...): scramble is not convertible to Turns!" << std::endl;
	}

	return Record(dateObject, timePair, turns);
}


std::string Record::makeDateStringFromRecord(Record const & record) //Format: day.month.year,hour:minute:second,weekday (4.5.1991,14:5:13,We)
{
	std::stringstream str;
	Date date = record.date;
	str << +date.getDay() << "." << +date.getMonth() << "." << +date.getYear() << "," << +date.getHour() << ":" << +date.getMinute() << ":" << +date.getSecond() << "," << date.getWeekday();
	return str.str();
}


std::string Record::makeTimeStringFromRecord(Record const & record)
{
	std::stringstream str;
	TimePair timePair = record.getTime();
	str << timePair.seconds << ".";
	if (timePair.hundredths < 10)
	{
		str << "0";
	}
	str << timePair.hundredths;
	return str.str();
}


std::string Record::makeScrambleStringFromRecord(Record const & record)
{
	std::stringstream str;
	str << record.scramble;
	return str.str();
}

