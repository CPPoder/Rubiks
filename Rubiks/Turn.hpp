#ifndef TURN_HPP
#define TURN_HPP


#include "TurnType.hpp"

#include <list>
#include <vector>
#include <array>
#include <functional>

class Cube;


class Turn
{
public:
	std::function<void(Cube& cube)> mTurnFunction;


public:
	Turn() = delete;
	Turn(std::function<void(Cube& cube)> turnFunction);
	~Turn() = default;
	Turn(Turn const & turn) = default;
	Turn & operator=(Turn const & turn) = default;


public:
	static const Turn & getTurn(TurnType turnType);


public:
	static const Turn Up;
	static const Turn UpInverse;
	static const Turn Up2;
	static const Turn Down;
	static const Turn DownInverse;
	static const Turn Down2;
	static const Turn Front;
	static const Turn FrontInverse;
	static const Turn Front2;
	static const Turn Back;
	static const Turn BackInverse;
	static const Turn Back2;
	static const Turn Right;
	static const Turn RightInverse;
	static const Turn Right2;
	static const Turn Left;
	static const Turn LeftInverse;
	static const Turn Left2;


public:
	static const std::array<TurnType, 18> arrayOfAllTurnTypes;
	static const std::array<TurnType, 12> arrayOfQuarterTurnTypes;


public:
	static std::string getTurnTypeString(TurnType turnType);

};



#endif //TURN_HPP

