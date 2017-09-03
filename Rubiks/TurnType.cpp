#include "TurnType.hpp"


TurnType operator!(TurnType turnType)
{
	unsigned int num = static_cast<unsigned int>(turnType);
	switch (num % 3)
	{
	case 0:
		return static_cast<TurnType>(num + 1);
	case 1:
		return static_cast<TurnType>(num - 1);
	case 2:
		return turnType;
	}
}


TurnType doubleTurnTypeOnSamePlane(TurnType turnType)
{
	unsigned int num = static_cast<unsigned int>(turnType);
	switch (num % 3)
	{
	case 0:
		return static_cast<TurnType>(num + 2);
	case 1:
		return static_cast<TurnType>(num + 1);
	case 2:
		return turnType;
	}
}


bool doTurnTypesActOnSamePlane(TurnType t1, TurnType t2)
{
	return (doubleTurnTypeOnSamePlane(t1) == doubleTurnTypeOnSamePlane(t2));
}


