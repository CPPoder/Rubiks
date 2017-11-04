#ifndef USERINPUT_HPP
#define USERINPUT_HPP


#include <list>
#include <string>
#include <iostream>
#include <functional>


class UserInput
{
public:
	UserInput() = delete;
	~UserInput() = delete;

public:
	static std::string demand(std::string const & message, std::list<std::string> listOfValidStrings);
	static std::string demand(std::string const & message, std::function<bool(char)> isCharAllowedFunction);

public:
	static const std::function<bool(char)> IS_LETTER_OR_NUMBER;


};

typedef UserInput UI;



#endif //USERINPUT_HPP

