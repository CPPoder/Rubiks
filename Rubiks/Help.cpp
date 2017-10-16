#include "Help.hpp"

std::string Help::getOverview()
{
	std::stringstream stream;
	stream << "Type help <command> for detailed help!" << std::endl;
	for (auto keyword : KeywordToken::arrayOfAllKeywords)
	{
		stream << Help::mapFromKeywordToName.at(keyword) << "\t" << Help::mapFromKeywordToShortDescription.at(keyword) << std::endl;
	}
	stream << std::endl;
	return stream.str();
}

std::string Help::getHelpFor(KeywordToken const & keywordToken)
{
	KeywordToken::Keyword keyword = keywordToken.getKeyword();
	return (Help::mapFromKeywordToShortDescription.at(keyword) + "\n\n" + Help::mapFromKeywordToDetailedDescription.at(keyword) + "\n");
}

std::string Help::getHelpForRecursive(unsigned int numberOfTokens, KeywordToken const & lastKeywordToken)
{
	std::stringstream stream;
	stream << "Shows the help for the command";
	for (unsigned int i = 0; i < numberOfTokens - 2; ++i)
	{
		stream << " help";
	}
	stream << " " << mapFromKeywordToName.at(lastKeywordToken.getKeyword()) << std::endl << std::endl;

	return stream.str();
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
	std::make_pair(KeywordToken::Keyword::assign,			"Assigns Cube/Turns to another Cube/Turns object"),
	std::make_pair(KeywordToken::Keyword::clear,			"Deletes all Cubes/Turns and clears the screen"),
	std::make_pair(KeywordToken::Keyword::Cube,				"Creates new Cube object"),
	std::make_pair(KeywordToken::Keyword::echo,				"Outputs Cube/Turns object"),
	std::make_pair(KeywordToken::Keyword::exit,				"Exits the application"),
	std::make_pair(KeywordToken::Keyword::help,				"Shows help"),
	std::make_pair(KeywordToken::Keyword::input,			"Allows to input a Cube manually"),
	std::make_pair(KeywordToken::Keyword::quicksolve,		"Solves a Cube quickly, but with many turns"),
	std::make_pair(KeywordToken::Keyword::solve,			"Solves a Cube with least turns"),
	std::make_pair(KeywordToken::Keyword::turn,				"Turns a Cube object"),
	std::make_pair(KeywordToken::Keyword::Turns,			"Creates a new Turns object")
};



std::string createDetailedDescriptionForAssign()
{
	std::stringstream stream;

	stream << "assign <name1> <name2>" << std::endl;
	stream << "Assigns the contents of <name1> to <name2>." << std::endl;
	stream << "<name1> and <name2> must be existing Cube or Turns objects." << std::endl;
	stream << "Instead of <name1> one can use either solved or -." << std::endl;
	stream << "solved represents a solved cube, - represents the identity turn." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForClear()
{
	std::stringstream stream;

	stream << "clear\n\n";
	stream << "Deletes all Cube and Turn objects and clears the screen.\n";

	return stream.str();
}

std::string createDetailedDescriptionForCube()
{
	std::stringstream stream;

	stream << "Cube <name>" << std::endl;
	stream << "Creates a solved cube. Using <name>, one can later refer to it." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForEcho()
{
	std::stringstream stream;

	stream << "echo <name>" << std::endl;
	stream << "Outputs the Cube or Turns object named <name> to the terminal." << std::endl;
	stream << "<name> must be an existing Cube or Turns object." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForExit()
{
	std::stringstream stream;

	stream << "exit" << std::endl;
	stream << "Exits the application." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForHelp()
{
	std::stringstream stream;

	stream << "help [<command>]" << std::endl;
	stream << "Without <command>, this shows the overview of all commands." << std::endl;
	stream << "With <command>, this shows the detailed help for <command>." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForInput()
{
	std::stringstream stream;

	stream << "input <cube>" << std::endl;
	stream << "Lets the user input a cube manually." << std::endl;
	stream << "<cube> must be a name of an existing Cube object." << std::endl;
	stream << "The input process can be aborted by typing quit." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForQuicksolve()
{
	std::stringstream stream;

	stream << "quicksolve <cube> <turns>" << std::endl;
	stream << "Solves <cube> quickly, but with many turns. Writes the result into <turns>." << std::endl;
	stream << "<cube> must be the name of an existing Cube object." << std::endl;
	stream << "<turns> must be the name of an existing Turns object." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForSolve()
{
	std::stringstream stream;

	stream << "solve <cube> <turns>" << std::endl;
	stream << "Solves <cube> using least turns. Writes the result into <turns>." << std::endl;
	stream << "<cube> must be the name of an existing Cube object." << std::endl;
	stream << "<turns> must be the name of an existing Turns object." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForTurn()
{
	std::stringstream stream;

	stream << "turn <cube> <turns>" << std::endl;
	stream << "Turns the <cube> object via <turns>." << std::endl;
	stream << "<cube> must be a name of an existing Cube object." << std::endl;
	stream << "<turns> must be either a name of an existing Turns object or a turn itself." << std::endl;
	stream << "The following turns can be used: R, L, F, B, U, D, R', L', F', B', U', D', R2, L2, F2, B2, U2, D2" << std::endl;
	stream << "Example: turn <cube> RF'F2B" << std::endl;
	stream << "It is also possible to use PLL or OLL algorithms like this: turn <cube> PLL:T." << std::endl;

	return stream.str();
}

std::string createDetailedDescriptionForTurns()
{
	std::stringstream stream;

	stream << "Turns <name>" << std::endl;
	stream << "Creates a indenty turns object. Using <name>, one can later refer to it." << std::endl;

	return stream.str();
}



const std::map<KeywordToken::Keyword, std::string> Help::mapFromKeywordToDetailedDescription = 
{
	std::make_pair(KeywordToken::Keyword::assign,			createDetailedDescriptionForAssign()),
	std::make_pair(KeywordToken::Keyword::clear,			createDetailedDescriptionForClear()),
	std::make_pair(KeywordToken::Keyword::Cube,				createDetailedDescriptionForCube()),
	std::make_pair(KeywordToken::Keyword::echo,				createDetailedDescriptionForEcho()),
	std::make_pair(KeywordToken::Keyword::exit,				createDetailedDescriptionForExit()),
	std::make_pair(KeywordToken::Keyword::help,				createDetailedDescriptionForHelp()),
	std::make_pair(KeywordToken::Keyword::input,			createDetailedDescriptionForInput()),
	std::make_pair(KeywordToken::Keyword::quicksolve,		createDetailedDescriptionForQuicksolve()),
	std::make_pair(KeywordToken::Keyword::solve,			createDetailedDescriptionForSolve()),
	std::make_pair(KeywordToken::Keyword::turn,				createDetailedDescriptionForTurn()),
	std::make_pair(KeywordToken::Keyword::Turns,			createDetailedDescriptionForTurns())
};

