#ifndef PARSER_HPP
#define PARSER_HPP


#include <iostream>
#include <string>
#include <list>
#include <exception>

#include "Token.hpp"
#include "Command.hpp"
#include "Help.hpp"


class Parser
{
public:
	Parser() = delete;
	~Parser() = delete;
	Parser(Parser const &) = delete;
	Parser& operator=(Parser const &) = delete;

public:
	static Command waitForInput();


private:
	static std::string extractInputToString();
	static std::list<Token*> tokenizeLine(std::string const & line);
	static Command interprete(std::list<Token*> const & tokens);

private:
	static bool isCharWhiteSpace(char c);
	static bool isBlockKeyword(std::string const & block, KeywordToken& keywordTokenReturn);

private:
	static Command returnCommandIfSizeOfListIsOne(Command cmd, std::list<Token*> const & list);

	static Command handleSolveTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleCubeTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleTurnsTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleEchoTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleAssignTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleTurnTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleInputTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleQuicksolveTokenCase(std::list<Token*> const & listOfTokenPointers);
	static Command handleHelpTokenCase(std::list<Token*> const & listOfTokenPointers);


private:
	class InvalidCommandException
	{
	};

};

#endif //PARSER_HPP

