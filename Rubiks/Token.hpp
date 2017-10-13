#ifndef TOKEN_HPP
#define TOKEN_HPP


#include <iostream>
#include <array>


class KeywordToken;
class UnidentifiedBlockToken;




/////////////
//Token Class

class Token
{
public:
	enum class Type
	{
		Keyword,
		UnidentifiedBlock
	};

protected:
	Type mType;

protected:
	Token() = default;
	Token(Token const &) = default;
	Token& operator=(Token const &) = default;

public:
	virtual ~Token() = default;
	
public:
	Type getType() const;

public:
	static const KeywordToken ExitToken;
	static const KeywordToken SolveToken;
	static const KeywordToken CubeToken;
	static const KeywordToken TurnsToken;
	static const KeywordToken EchoToken;
	static const KeywordToken AssignToken;
	static const KeywordToken ClearToken;
	static const KeywordToken TurnToken;
	static const KeywordToken InputToken;
	static const KeywordToken QuicksolveToken;
	static const KeywordToken HelpToken;

public:
	static KeywordToken* tryDynamicCastToKeywordToken(Token* token);
	static UnidentifiedBlockToken* tryDynamicCastToUnidentifiedBlockToken(Token* token);

};





////////////////////
//KeywordToken Class

class KeywordToken final : public Token
{
public:
	enum class Keyword
	{
		exit,
		solve,
		Cube,
		Turns,
		echo,
		assign,
		clear,
		turn,
		input,
		quicksolve,
		help,
		NUM_OF_KEYWORDS
	};

	static const std::array<Keyword, static_cast<int>(Keyword::NUM_OF_KEYWORDS)> arrayOfAllKeywords;

private:
	Keyword mKeyword;

public:
	KeywordToken() = delete;
	virtual ~KeywordToken() override = default;
	KeywordToken(Keyword const & keyword);
	KeywordToken(KeywordToken const &) = default;
	KeywordToken& operator=(KeywordToken const &) = default; //Test this

public:
	Keyword getKeyword() const;

};






//////////////////////////////
//UnidentifiedBlockToken Class

class UnidentifiedBlockToken final : public Token
{
private:
	std::string mBlockText;

public:
	UnidentifiedBlockToken() = delete;
	virtual ~UnidentifiedBlockToken() override = default;
	UnidentifiedBlockToken(std::string const & text);
	UnidentifiedBlockToken(UnidentifiedBlockToken const &) = default;
	UnidentifiedBlockToken& operator=(UnidentifiedBlockToken const &) = default; //Test this

public:
	std::string getBlockText() const;

};

#endif //TOKEN_HPP

