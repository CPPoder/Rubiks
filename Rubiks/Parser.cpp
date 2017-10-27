#include "Parser.hpp"


Command Parser::waitForInput()
{
	std::cout << ">";

	std::string line = Parser::extractInputToString();
	std::list<Token*> listOfTokens = Parser::tokenizeLine(line);
	Command command = Parser::interprete(listOfTokens);
	for (auto & tokenPointer : listOfTokens)
	{
		delete tokenPointer;
		tokenPointer = nullptr;
	}

	//std::cout << std::endl;
	return command;
}


std::string Parser::extractInputToString()
{
	std::string line;
	std::getline(std::cin, line);
	return line;
}


std::list<Token*> Parser::tokenizeLine(std::string const & line)
{
	//Cut line into blocks separated by whitespaces
	std::list<std::string> listOfBlocks;
	bool currentlyInBlock = false;
	std::string currentString = "";
	for (unsigned int i = 0; i < line.size(); ++i)
	{
		if (Parser::isCharWhiteSpace(line.at(i)))
		{
			if (currentlyInBlock)
			{
				currentlyInBlock = false;
				listOfBlocks.push_back(currentString);
				currentString = "";
			}
		}
		else //Is not whitespace
		{
			if (currentlyInBlock)
			{
				currentString.push_back(line.at(i));
			}
			else //Is not currently in block
			{
				currentlyInBlock = true;
				currentString.push_back(line.at(i));
			}
		}
	}
	if (currentlyInBlock)
	{
		currentlyInBlock = false;
		listOfBlocks.push_back(currentString);
		currentString = "";
	}

	//Create Tokens from elements in listOfBlocks
	std::list<Token*> listOfTokenPointer;
	for (auto const & block : listOfBlocks)
	{
		KeywordToken keywordToken(Token::ExitToken);
		bool isKeyword = Parser::isBlockKeyword(block, keywordToken);
		if (isKeyword)
		{
			listOfTokenPointer.push_back(new KeywordToken(keywordToken));
		}
		else
		{
			listOfTokenPointer.push_back(new UnidentifiedBlockToken(block));
		}
	}

	return listOfTokenPointer;
}


Command Parser::interprete(std::list<Token*> const & tokens)
{
	//Do nothing if command consist of pure whitespaces
	if (tokens.empty())
	{
		return Command::DO_NOTHING;
	}

	//Initialize iterator to begin
	std::list<Token*>::const_iterator it = tokens.cbegin();

	//Invalid Command if first block is unidentified block
	if ((*it)->getType() == Token::Type::UnidentifiedBlock)
	{
		return Command::INVALID;
	}

	if ((*it)->getType() == Token::Type::Keyword)
	{
		//Use dynamic_cast to transform Token* into KeywordToken*
		KeywordToken* keywordTokenPointer = Token::tryDynamicCastToKeywordToken((*it));
		KeywordToken::Keyword keyword = keywordTokenPointer->getKeyword();

		//Handle different cases different
		switch (keyword)
		{
		case KeywordToken::Keyword::exit:
			return Parser::returnCommandIfSizeOfListIsOne(Command::EXIT, tokens);
			break;
		case KeywordToken::Keyword::solve:
			return Parser::handleSolveTokenCase(tokens);
			break;
		case KeywordToken::Keyword::Cube:
			return Parser::handleCubeTokenCase(tokens);
			break;
		case KeywordToken::Keyword::Turns:
			return Parser::handleTurnsTokenCase(tokens);
			break;
		case KeywordToken::Keyword::echo:
			return Parser::handleEchoTokenCase(tokens);
			break;
		case KeywordToken::Keyword::assign:
			return Parser::handleAssignTokenCase(tokens);
			break;
		case KeywordToken::Keyword::clear:
			return Parser::returnCommandIfSizeOfListIsOne(Command::CLEAR, tokens);
			break;
		case KeywordToken::Keyword::turn:
			return Parser::handleTurnTokenCase(tokens);
			break;
		case KeywordToken::Keyword::input:
			return Parser::handleInputTokenCase(tokens);
			break;
		case KeywordToken::Keyword::quicksolve:
			return Parser::handleQuicksolveTokenCase(tokens);
			break;
		case KeywordToken::Keyword::help:
			return Parser::handleHelpTokenCase(tokens);
			break;
		case KeywordToken::Keyword::show:
			return Parser::handleShowTokenCase(tokens);
			break;
		case KeywordToken::Keyword::timer:
			return Parser::returnCommandIfSizeOfListIsOne(Command::TIMER, tokens);
			break;
		}
	}

}













bool Parser::isCharWhiteSpace(char c)
{
	return ((c == ' ') || (c == '\t') || (c == '\n'));
}


bool Parser::isBlockKeyword(std::string const & block, KeywordToken& keywordTokenReturn)
{
	//Search for all keywords
	if (block == "exit")
	{
		keywordTokenReturn = Token::ExitToken;
		return true;
	}
	if (block == "solve")
	{
		keywordTokenReturn = Token::SolveToken;
		return true;
	}
	if (block == "Cube")
	{
		keywordTokenReturn = Token::CubeToken;
		return true;
	}
	if (block == "Turns")
	{
		keywordTokenReturn = Token::TurnsToken;
		return true;
	}
	if (block == "echo")
	{
		keywordTokenReturn = Token::EchoToken;
		return true;
	}
	if (block == "assign")
	{
		keywordTokenReturn = Token::AssignToken;
		return true;
	}
	if (block == "clear")
	{
		keywordTokenReturn = Token::ClearToken;
		return true;
	}
	if (block == "turn")
	{
		keywordTokenReturn = Token::TurnToken;
		return true;
	}
	if (block == "input")
	{
		keywordTokenReturn = Token::InputToken;
		return true;
	}
	if (block == "quicksolve")
	{
		keywordTokenReturn = Token::QuicksolveToken;
		return true;
	}
	if (block == "help")
	{
		keywordTokenReturn = Token::HelpToken;
		return true;
	}
	if (block == "show")
	{
		keywordTokenReturn = Token::ShowToken;
		return true;
	}
	if (block == "timer")
	{
		keywordTokenReturn = Token::TimerToken;
		return true;
	}

	//If no keywords were found, return false
	return false;
}






Command Parser::returnCommandIfSizeOfListIsOne(Command cmd, std::list<Token*> const & list)
{
	if (list.size() == 1u)
	{
		return cmd;
	}
	else
	{
		return Command::INVALID;
	}
}





Command Parser::handleSolveTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Solve KeywordToken for sure!
	//Therefore, this function must check that there are exactly 3 tokens and that the second and third one are names!
	try
	{
		if ((listOfTokenPointers.size() != 3u))
		{
			throw InvalidCommandException();
		}
		std::string texts[2];
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		for (unsigned int i = 0; i < 2; ++i, ++it)
		{
			Token* token = *it;
			if (token->getType() == Token::Type::Keyword)
			{
				throw InvalidCommandException();
			}
			UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
			texts[i] = uToken->getBlockText();
		}
		Command cmd;
		cmd.setSolve(Command::SolveSpecification(texts[0], texts[1]));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}



Command Parser::handleQuicksolveTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Quicksolve KeywordToken for sure!
	//Therefore, this function must check that there are exactly 3 tokens and that the second and third one are names!
	try
	{
		if ((listOfTokenPointers.size() != 3u))
		{
			throw InvalidCommandException();
		}
		std::string texts[2];
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		for (unsigned int i = 0; i < 2; ++i, ++it)
		{
			Token* token = *it;
			if (token->getType() == Token::Type::Keyword)
			{
				throw InvalidCommandException();
			}
			UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
			texts[i] = uToken->getBlockText();
		}
		Command cmd;
		cmd.setQuicksolve(Command::QuicksolveSpecification(texts[0], texts[1]));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}



Command Parser::handleCubeTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Cube KeywordToken for sure!
	//Therefore, this function must check that there are exactly 2 tokens and that the second one is a name!
	try
	{
		if ((listOfTokenPointers.size() != 2u))
		{
			throw InvalidCommandException();
		}
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		Token* token = *it;
		if (token->getType() == Token::Type::Keyword)
		{
			throw InvalidCommandException();
		}
		UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
		Command cmd;
		cmd.setCreateCube(Command::CreateCubeSpecification(uToken->getBlockText()));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}



Command Parser::handleTurnsTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Turns KeywordToken for sure!
	//Therefore, this function must check that there are exactly 2 tokens and that the second one is a name!
	try
	{
		if ((listOfTokenPointers.size() != 2u))
		{
			throw InvalidCommandException();
		}
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		Token* token = *it;
		if (token->getType() == Token::Type::Keyword)
		{
			throw InvalidCommandException();
		}
		UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
		Command cmd;
		cmd.setCreateTurns(Command::CreateTurnsSpecification(uToken->getBlockText()));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}



Command Parser::handleEchoTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Echo KeywordToken for sure!
	//Therefore, this function must check that there are exactly 2 tokens and that the second one is a name!
	try
	{
		if ((listOfTokenPointers.size() != 2u))
		{
			throw InvalidCommandException();
		}
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		Token* token = *it;
		if (token->getType() == Token::Type::Keyword)
		{
			throw InvalidCommandException();
		}
		UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
		Command cmd;
		cmd.setEcho(Command::EchoSpecification(uToken->getBlockText()));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}



Command Parser::handleAssignTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Assign KeywordToken for sure!
	//Therefore, this function must check that there are exactly 3 tokens and that the second and third one are names!
	try
	{
		if ((listOfTokenPointers.size() != 3u))
		{
			throw InvalidCommandException();
		}
		std::string texts[2];
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		for (unsigned int i = 0; i < 2; ++i, ++it)
		{
			Token* token = *it;
			if (token->getType() == Token::Type::Keyword)
			{
				throw InvalidCommandException();
			}
			UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
			texts[i] = uToken->getBlockText();
		}
		Command cmd;
		cmd.setAssign(Command::AssignSpecification(texts[0], texts[1]));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}



Command Parser::handleTurnTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Turn KeywordToken for sure!
	//Therefore, this function must check that there are exactly 3 tokens and that the second and third one are names!
	try
	{
		if ((listOfTokenPointers.size() != 3u))
		{
			throw InvalidCommandException();
		}
		std::string texts[2];
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		for (unsigned int i = 0; i < 2; ++i, ++it)
		{
			Token* token = *it;
			if (token->getType() == Token::Type::Keyword)
			{
				throw InvalidCommandException();
			}
			UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
			texts[i] = uToken->getBlockText();
		}
		Command cmd;
		cmd.setTurn(Command::TurnSpecification(texts[0], texts[1]));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}



Command Parser::handleInputTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Input KeywordToken for sure!
	//Therefore, this function must check that there are exactly 2 tokens and that the second one is a name!
	try
	{
		if ((listOfTokenPointers.size() != 2u))
		{
			throw InvalidCommandException();
		}
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		Token* token = *it;
		if (token->getType() == Token::Type::Keyword)
		{
			throw InvalidCommandException();
		}
		UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
		Command cmd;
		cmd.setInput(Command::InputSpecification(uToken->getBlockText()));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}




Command Parser::handleHelpTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Help KeywordToken for sure!
	//This function must check that all tokens are KeywordTokens!
	//If there are more than 2 tokens, this function must check that all but the last are the help token!
	try
	{
		Command cmd;

		//Check that all tokens are KeywordTokens
		for (auto token : listOfTokenPointers)
		{
			if (token->getType() == Token::Type::UnidentifiedBlock)
			{
				throw InvalidCommandException();
			}
		}
		
		//Handle size==1 case
		if (listOfTokenPointers.size() == 1)
		{
			cmd.setHelp(Command::HelpSpecification(Help::getOverview()));
			return cmd;
		}

		//Handle size==2 case
		if (listOfTokenPointers.size() == 2)
		{
			KeywordToken* keywordToken = Token::tryDynamicCastToKeywordToken(listOfTokenPointers.back());
			cmd.setHelp(Command::HelpSpecification(Help::getHelpFor(*keywordToken)));
			return cmd;
		}

		//Handle size>=3 case
		std::list<KeywordToken*> listOfKeywordTokenPointers;
		for (auto token : listOfTokenPointers)
		{
			listOfKeywordTokenPointers.push_back(Token::tryDynamicCastToKeywordToken(token));
		}
		for (std::list<KeywordToken*>::reverse_iterator revIt = ++listOfKeywordTokenPointers.rbegin(); revIt != listOfKeywordTokenPointers.rend(); ++revIt) //Check that all but the last are help tokens
		{
			if ((*revIt)->getKeyword() != KeywordToken::Keyword::help)
			{
				throw InvalidCommandException();
			}
		}
		cmd.setHelp(Command::HelpSpecification(Help::getHelpForRecursive(listOfKeywordTokenPointers.size(), *listOfKeywordTokenPointers.back())));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}




Command Parser::handleShowTokenCase(std::list<Token*> const & listOfTokenPointers)
{
	//If this function is called, the first Token is the Show KeywordToken for sure!
	//Therefore, this function must check that there are exactly 2 tokens and that the second one is a name!
	try
	{
		if ((listOfTokenPointers.size() != 2u))
		{
			throw InvalidCommandException();
		}
		std::list<Token*>::const_iterator it = listOfTokenPointers.begin();
		++it;
		Token* token = *it;
		if (token->getType() == Token::Type::Keyword)
		{
			throw InvalidCommandException();
		}
		UnidentifiedBlockToken* uToken = Token::tryDynamicCastToUnidentifiedBlockToken(token);
		Command cmd;
		cmd.setShow(Command::ShowSpecification(uToken->getBlockText()));
		return cmd;
	}
	catch (InvalidCommandException exception) //Exceptions are used to catch all invalid command cases!
	{
		return Command::INVALID;
	}
}








