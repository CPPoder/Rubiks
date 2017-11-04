#ifndef STRINGMANIPULATION_HPP
#define STRINGMANIPULATION_HPP


#include <vector>
#include <string>


class StringManipulation
{
public:
	StringManipulation() = delete;
	~StringManipulation() = delete;

public:
	static std::vector<std::string> cutStringAtChar(std::string const & string, char separator);

};

typedef StringManipulation StrManip;

#endif //STRINGMANIPULATION_HPP

