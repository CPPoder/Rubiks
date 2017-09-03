#ifndef TURNTYPE_HPP
#define TURNTYPE_HPP




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


TurnType operator!(TurnType turnType);

TurnType doubleTurnTypeOnSamePlane(TurnType turnType);

bool doTurnTypesActOnSamePlane(TurnType t1, TurnType t2);


#endif //TURNTYPE_HPP

