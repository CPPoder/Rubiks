#ifndef SCRAMBLER_HPP
#define SCRAMBLER_HPP

#include "Cube.hpp"
#include "TurnTypeOrder.hpp"


class Scrambler
{
public:
	Scrambler() = delete;
	~Scrambler() = delete;
	
public:
	static TurnTypeOrder getScramble(unsigned int numberOfTurns);


};


#endif //SCRAMBLER_HPP

