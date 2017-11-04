#ifndef RECORD_HPP
#define RECORD_HPP


#include "Date.hpp"
#include "TimePair.hpp"
#include "TurnTypeOrder.hpp"
#include "Turn.hpp"
#include "StringManipulation.hpp"


class Record
{
private:
	Date date;
	TimePair time;
	TurnTypeOrder scramble;

public:
	Record() = delete;
	Record(Date const & _date, TimePair const & _time, TurnTypeOrder _scramble);

public:
	Date getDate() const;
	TimePair getTime() const;
	TurnTypeOrder getScramble() const;

public:
	static Record makeRecordFromFileStrings(std::string const & date, std::string const & time, std::string const & scramble);
	static std::string makeDateStringFromRecord(Record const & record);
	static std::string makeTimeStringFromRecord(Record const & record);
	static std::string makeScrambleStringFromRecord(Record const & record);


};


#endif //RECORD_HPP

