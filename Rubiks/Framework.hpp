#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP


#include "Parser.hpp"
#include "Cube.hpp"
#include "TurnTypeOrder.hpp"
#include "Solver.hpp"


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>



class Framework
{
private:
	bool mRun = true;
	std::map<std::string, Cube*> mMapOfCubePointers;
	std::map<std::string, TurnTypeOrder> mMapOfTurns;

public:
	Framework();
	~Framework();

	void run();
	void terminate();

private:
	void outputASCIIArt();

private:
	bool addCube(std::string const & name);
	bool addTurns(std::string const & name);
	void clear();
	void clearCubeMap();
	void clearTurnsMap();
	bool echoCubeOrTurns(std::string const & name);
	bool assignCubeOrTurns(std::string const & from, std::string const & to);
	bool turn(std::string const & which, std::string const & turns);
	bool inputCube(std::string const & name);
	bool solveCube(std::string const & cube, std::string const & writeTo);
	bool quicksolveCube(std::string const & cube, std::string const & writeTo);

private:
	bool findCubeNamed(std::string const & name, Cube*& cube);
	bool findTurnTypeOrderNamed(std::string const & name, TurnTypeOrder& turnTypeOrder);

};














#endif //FRAMEWORK_HPP

