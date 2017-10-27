#include "Scrambler.hpp"

#include <map>

TurnTypeOrder Scrambler::getScramble(unsigned int numberOfTurns)
{
	TurnTypeOrder turns;
	while (turns.size() < numberOfTurns)
	{
		TurnType t = Turn::getRandomTurnType();
		if (turns.empty())
		{
			turns.push_back(t);
			continue;
		}
		if (!doTurnTypesActOnSamePlane(turns.back(), t))
		{
			turns.push_back(t);
		}
	}
	return turns;
}


