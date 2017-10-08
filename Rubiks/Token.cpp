#include "Token.hpp"


/////////////
//Token Class

Token::Type Token::getType() const
{
	return mType;
}

const KeywordToken Token::ExitToken = KeywordToken(KeywordToken::Keyword::exit);
const KeywordToken Token::SolveToken = KeywordToken(KeywordToken::Keyword::solve);
const KeywordToken Token::CubeToken = KeywordToken(KeywordToken::Keyword::Cube);
const KeywordToken Token::TurnsToken = KeywordToken(KeywordToken::Keyword::Turns);
const KeywordToken Token::EchoToken = KeywordToken(KeywordToken::Keyword::echo);
const KeywordToken Token::AssignToken = KeywordToken(KeywordToken::Keyword::assign);
const KeywordToken Token::ClearToken = KeywordToken(KeywordToken::Keyword::clear);
const KeywordToken Token::TurnToken = KeywordToken(KeywordToken::Keyword::turn);
const KeywordToken Token::InputToken = KeywordToken(KeywordToken::Keyword::input);
const KeywordToken Token::QuicksolveToken = KeywordToken(KeywordToken::Keyword::quicksolve);


KeywordToken* Token::tryDynamicCastToKeywordToken(Token* token)
{
	KeywordToken* newToken = dynamic_cast<KeywordToken*>(token);
	if (newToken == nullptr)
	{
		throw "The programmer doesn't understand dynamic_cast!";
	}
	return newToken;
}

UnidentifiedBlockToken* Token::tryDynamicCastToUnidentifiedBlockToken(Token* token)
{
	UnidentifiedBlockToken* newToken = dynamic_cast<UnidentifiedBlockToken*>(token);
	if (newToken == nullptr)
	{
		throw "The programmer doesn't understand dynamic_cast!";
	}
	return newToken;
}





////////////////////
//KeywordToken Class

KeywordToken::KeywordToken(Keyword const & keyword)
	: mKeyword(keyword)
{
	mType = Token::Type::Keyword;
}

KeywordToken::Keyword KeywordToken::getKeyword() const
{
	return mKeyword;
}




//////////////////////////////
//UnidentifiedBlockToken Class

UnidentifiedBlockToken::UnidentifiedBlockToken(std::string const & text)
	: mBlockText(text)
{
	mType = Token::Type::UnidentifiedBlock;
}

std::string UnidentifiedBlockToken::getBlockText() const
{
	return mBlockText;
}



