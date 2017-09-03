#include <iostream>


#include "Cube.hpp"



int main()
{
	Cube cube;
	cube.askForConsoleInput();
	//cube.turn(TurnOrder({ Turn::Down, Turn::Up , Turn::Left , Turn::Back , Turn::Up, Turn::LeftInverse, Turn::DownInverse }));
	//cube.turn(TurnOrder::T);
	std::cout << cube;
	cube.solve(Cube::SolveTurns::QUARTER_TURNS, Cube::Comparator::FullWhiteCross && Cube::Comparator::Corner::UpFrontRight);

	std::cout << "Done!" << std::endl;
	std::getchar();
    return 0;
}

