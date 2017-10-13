#ifndef HELP_HPP
#define HELP_HPP


#include "Token.hpp"
#include "Command.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <map>



class Help
{
public:
	Help() = delete;
	~Help() = delete;
	Help& operator=(Help const &) = delete;
	Help(Help const &) = delete;

public:
	static std::string getOverview();
	static std::string getHelpFor(KeywordToken const & keywordToken);
	static std::string getHelpForRecursive(unsigned int numberOfTokens, KeywordToken const & lastKeywordToken);

private:
	static const std::map<KeywordToken::Keyword, std::string> mapFromKeywordToName;
	static const std::map<KeywordToken::Keyword, std::string> mapFromKeywordToShortDescription;
	static const std::map<KeywordToken::Keyword, std::string> mapFromKeywordToDetailedDescription;


};







#endif //HELP_HPP

