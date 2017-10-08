#ifndef TURNTYPE_HPP
#define TURNTYPE_HPP


#include <iostream>



enum class TurnType
{
	Up,
	UpInverse,
	Up2,
	Down,
	DownInverse,
	Down2,
	Front,
	FrontInverse,
	Front2,
	Back,
	BackInverse,
	Back2,
	Right,
	RightInverse,
	Right2,
	Left,
	LeftInverse,
	Left2
};


std::ostream& operator<<(std::ostream& oStream, TurnType const & turnType);

TurnType operator!(TurnType turnType);

TurnType doubleTurnTypeOnSamePlane(TurnType turnType);

bool doTurnTypesActOnSamePlane(TurnType t1, TurnType t2);


#endif //TURNTYPE_HPP

