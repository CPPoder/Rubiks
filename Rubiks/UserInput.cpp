#include "UserInput.hpp"


std::string UserInput::demand(std::string const & message, std::list<std::string> listOfValidStrings)
{
	while (true)
	{
		std::cout << message;
		std::string line;
		std::getline(std::cin, line);
		if (listOfValidStrings.empty())
		{
			return line;
		}
		for (auto const & validString : listOfValidStrings)
		{
			if (line == validString)
			{
				return line;
			}
		}
		std::cout << "Input was invalid! Try again!" << std::endl;
	}
}


std::string UserInput::demand(std::string const & message, std::function<bool(char)> isCharAllowedFunction)
{
	std::string line;
	bool wrongChar = false;
	do
	{
		wrongChar = false;
		std::cout << message;
		std::getline(std::cin, line);
		for (auto c : line)
		{
			if (!isCharAllowedFunction(c))
			{
				std::cout << "Input was invalid! Try again!" << std::endl;
				wrongChar = true;
				break;
			}
		}
	} while (wrongChar);
	return line;
}


const std::function<bool(char)> UserInput::IS_LETTER_OR_NUMBER = [](char c)
{
	bool upperCase = (c >= 65 && c <= 90);
	bool lowerCase = (c >= 97 && c <= 122);
	bool number = (c >= 48 && c <= 57);
	return (upperCase || lowerCase || number);
};


