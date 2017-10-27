#ifndef TURN_HPP
#define TURN_HPP


#include "TurnType.hpp"
#include "TurnTypeOrder.hpp"
#include "Random.hpp"

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
	static TurnType getTurnTypeFromNumberInArray(unsigned int number, bool useOnlyQuarterTurns);
	static unsigned int getNumberOfTurnTypes(bool useOnlyQuarterTurns);
	static TurnType getRandomTurnType();

public:
	static TurnType transformTurnTypeViaZ2Rotation(TurnType turnType); //This function is used to transform turns like T-perm onto the flipped cube
	static TurnTypeOrder transformTurnTypeOrderViaZ2Rotation(TurnTypeOrder const & turnTypeOrder); //Applies Z2Rotation to every turnType

	static TurnType transformTurnTypeViaYRotation(TurnType turnType); //This function is used to transform F2L-solving turns around the cube
	static TurnTypeOrder transformTurnTypeOrderViaYRotation(TurnTypeOrder const & turnTypeOrder); //Applies YRotation to every turnType

public:
	static bool isStringConvertibleIntoTurns(std::string const & string, TurnTypeOrder& turns);
	static bool isStringConvertibleIntoTurnType(std::string const & string, TurnType& turnType);
	static bool isCharEqualToOneCharInList(char c, std::list<char> listOfChars);

public:
	static void appendTurnTypeOrder(TurnTypeOrder& t1, TurnTypeOrder const & t2);

};

TurnTypeOrder operator+(TurnTypeOrder const & t1, TurnTypeOrder const & t2);
TurnTypeOrder& operator+=(TurnTypeOrder & t1, TurnTypeOrder const & t2);

TurnTypeOrder operator+(TurnTypeOrder const & t1, TurnType const & t2);
TurnTypeOrder operator+(TurnType const & t1, TurnTypeOrder const & t2);
TurnTypeOrder operator+(TurnType const & t1, TurnType const & t2);


#endif //TURN_HPP

