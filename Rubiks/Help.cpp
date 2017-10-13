#include "Help.hpp"

std::string Help::getOverview()
{
	std::stringstream stream;
	for (auto keyword : KeywordToken::arrayOfAllKeywords)
	{
		stream << Help::mapFromKeywordToName.at(keyword) << "\t\t" << Help::mapFromKeywordToShortDescription.at(keyword) << std::endl;
	}
	return stream.str();
}

std::string Help::getHelpFor(KeywordToken const & keywordToken)
{
	return Help::mapFromKeywordToDetailedDescription.at(keywordToken.getKeyword());
}

std::string Help::getHelpForRecursive(unsigned int numberOfTokens, KeywordToken const & lastKeywordToken)
{
	return "Lustiges Zeugs!\n";
}








const std::map<KeywordToken::Keyword, std::string> Help::mapFromKeywordToName = 
{
	std::make_pair(KeywordToken::Keyword::assign,			"assign    "),
	std::make_pair(KeywordToken::Keyword::clear,			"clear     "),
	std::make_pair(KeywordToken::Keyword::Cube,				"Cube      "),
	std::make_pair(KeywordToken::Keyword::echo,				"echo      "),
	std::make_pair(KeywordToken::Keyword::exit,				"exit      "),
	std::make_pair(KeywordToken::Keyword::help,				"help      "),
	std::make_pair(KeywordToken::Keyword::input,			"input     "),
	std::make_pair(KeywordToken::Keyword::quicksolve,		"quicksolve"),
	std::make_pair(KeywordToken::Keyword::solve,			"solve     "),
	std::make_pair(KeywordToken::Keyword::turn,				"turn      "),
	std::make_pair(KeywordToken::Keyword::Turns,			"Turns     ")
};




const std::map<KeywordToken::Keyword, std::string> Help::mapFromKeywordToShortDescription = 
{
	std::make_pair(KeywordToken::Keyword::assign,			"assign"),
	std::make_pair(KeywordToken::Keyword::clear,			"clear"),
	std::make_pair(KeywordToken::Keyword::Cube,				"Cube"),
	std::make_pair(KeywordToken::Keyword::echo,				"echo"),
	std::make_pair(KeywordToken::Keyword::exit,				"exit"),
	std::make_pair(KeywordToken::Keyword::help,				"help"),
	std::make_pair(KeywordToken::Keyword::input,			"input"),
	std::make_pair(KeywordToken::Keyword::quicksolve,		"quicksolve"),
	std::make_pair(KeywordToken::Keyword::solve,			"solve"),
	std::make_pair(KeywordToken::Keyword::turn,				"turn"),
	std::make_pair(KeywordToken::Keyword::Turns,			"Turns")
};




const std::map<KeywordToken::Keyword, std::string> Help::mapFromKeywordToDetailedDescription = 
{
	std::make_pair(KeywordToken::Keyword::assign,			"assign"),
	std::make_pair(KeywordToken::Keyword::clear,			"clear"),
	std::make_pair(KeywordToken::Keyword::Cube,				"Cube"),
	std::make_pair(KeywordToken::Keyword::echo,				"echo"),
	std::make_pair(KeywordToken::Keyword::exit,				"exit"),
	std::make_pair(KeywordToken::Keyword::help,				"help"),
	std::make_pair(KeywordToken::Keyword::input,			"input"),
	std::make_pair(KeywordToken::Keyword::quicksolve,		"quicksolve"),
	std::make_pair(KeywordToken::Keyword::solve,			"solve"),
	std::make_pair(KeywordToken::Keyword::turn,				"turn"),
	std::make_pair(KeywordToken::Keyword::Turns,			"Turns")
};

