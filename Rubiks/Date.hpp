#ifndef DATE_HPP
#define DATE_HPP


#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <vector>

#include "StringManipulation.hpp"

class Date
{
public:
	enum class Weekday
	{
		Mo = 1, Tu = 2, We = 3, Th = 4, Fr = 5, Sa = 6, Su = 7
	};

private:
	unsigned char day;
	unsigned char month;
	unsigned short year;

	unsigned char hour;
	unsigned char minute;
	unsigned char second;

	Weekday weekday;

public:
	Date();
	Date(unsigned char _day, unsigned char _month, unsigned short _year, unsigned char _hour, unsigned char _minute, unsigned char _second, Weekday _weekday);
	

public:
	unsigned char getDay() const;
	unsigned char getMonth() const;
	unsigned short getYear() const;
	unsigned char getHour() const;
	unsigned char getMinute() const;
	unsigned char getSecond() const;
	Weekday getWeekday() const;

	float getTimeInHours() const;



public:
	static Date getCurrentDate();
	static Weekday makeWeekdayFromOwnCodingString(std::string const & string);
	
private:
	static Weekday extractWeekdayFromString(std::string const & string);
	static unsigned char extractMonthFromString(std::string const & string);
	static void extractHourMinuteSecondFromString(std::string const & string, unsigned char & hour, unsigned char & minute, unsigned char & second);
	static int convertStringWithPossibleZerosAtBeginningIntoInt(std::string const & string);

public:
	friend std::ostream& operator<<(std::ostream & oStream, Date const & date);


};

std::ostream& operator<<(std::ostream & oStream, Date::Weekday const & weekday);




#endif //DATE_HPP

