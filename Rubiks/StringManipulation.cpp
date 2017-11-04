#include "StringManipulation.hpp"

std::vector<std::string> StringManipulation::cutStringAtChar(std::string const & string, char separator)
{
	std::vector<std::string> vec;
	std::string str = string;
	do
	{
		unsigned int pos = str.find(separator);
		std::string firstSubStr = str.substr(0u, pos);
		std::string secondSubStr;
		if (pos != std::string::npos)
		{
			secondSubStr = str.substr(pos + 1);
		}
		else
		{
			secondSubStr = "";
		}		
		vec.push_back(firstSubStr);
		str = secondSubStr;
	} while (!str.empty());
	return std::move(vec);
}

