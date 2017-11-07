#include "Solver.hpp"



Solver::SolveAttributes::SolveAttributes()
	: SolveAttributes(SolveTurns::QUARTER_TURNS, Comparator::FullCube, InterruptProperties(InterruptProperties::Type::NEVER), SolverMode::Classic)
{
}

Solver::SolveAttributes::SolveAttributes(SolveTurns const & _solveTurns, Comparator const & _comparator, InterruptProperties const & _interruptProperties, SolverMode const & _solverMode)
	: solveTurns(_solveTurns), comparator(_comparator), interruptProperties(_interruptProperties), solverMode(_solverMode)
{
}

Solver::InterruptProperties::InterruptProperties(Type _type, int _info)
	: type(_type), info(_info)
{
}

Solver::InterruptContinuationInfo::InterruptContinuationInfo()
	: isContinuation(false)
{
}

Solver::InterruptContinuationInfo::InterruptContinuationInfo(std::vector<unsigned int> _oldVecOfTurnNumbers)
	: isContinuation(true), oldVecOfTurnNumbers(_oldVecOfTurnNumbers)
{
}

const Solver::InterruptContinuationInfo Solver::InterruptContinuationInfo::NO_CONTINUATION = Solver::InterruptContinuationInfo();

Solver::SolverState::SolverState(std::vector<unsigned int> const & _oldVectorOfTurnNumbers, Cube const & _initialCube, SolveAttributes const & _solveAttributes)
	: oldVectorOfTurnNumbers(_oldVectorOfTurnNumbers), initialCube(_initialCube), solveAttributes(_solveAttributes)
{
}

Solver::InterruptException::InterruptException(SolverState const & _solverState)
	: solverState(_solverState)
{
}





void Solver::solveManager(Cube const & cube, TurnTypeOrder& turns)
{
	try
	{
		SolveAttributes solveAttributes = Solver::askForUserInput();
		Solver::solveSubManager(cube, turns, solveAttributes, InterruptContinuationInfo::NO_CONTINUATION);
	}
	catch (Solver::QuitException)
	{
		std::cout << "Aborted solving subprogram!" << std::endl << std::endl;
		return;
	}
}


void Solver::solveSubManager(Cube const & cube, TurnTypeOrder& turns, SolveAttributes const & solveAttributes, InterruptContinuationInfo const & interruptContinuationInfo)
{
	try
	{
		TurnTypeOrder turnTypeOrder;
		std::vector<unsigned int> continuationVector = (interruptContinuationInfo.isContinuation ? interruptContinuationInfo.oldVecOfTurnNumbers : std::vector<unsigned int>{});
		switch (solveAttributes.solverMode)
		{
		case SolverMode::Classic:
			turnTypeOrder = Solver::solve(cube, solveAttributes, continuationVector);
			break;
		case SolverMode::RAM:
			turnTypeOrder = Solver::solveUsingRAM(cube, solveAttributes);
			break;
		}
		turns = turnTypeOrder;
	}
	catch (Solver::InterruptException exception)
	{
		std::cout << "Solve algorithm has been interrupted!" << std::endl;
		InterruptProperties newInterruptProperties;
		if (Solver::askIfUserWantsToContinueSolving(newInterruptProperties))
		{
			SolveAttributes newSolveAttributes(solveAttributes);
			newSolveAttributes.interruptProperties = newInterruptProperties;
			Solver::solveSubManager(cube, turns, newSolveAttributes, InterruptContinuationInfo(exception.solverState.oldVectorOfTurnNumbers));
		}
		else
		{
			throw Solver::QuitException();
		}
	}
}

TurnTypeOrder Solver::solve(Cube const & cube, SolveAttributes solveAttributes, std::vector<unsigned int> const & initialVectorOfTurnNumbers, std::ostream & oStream)
{
	//Translate SolveTurns into bool
	bool useOnlyQuarterTurns = (solveAttributes.solveTurns == SolveTurns::QUARTER_TURNS);

	//Declare Clocks
	Clock timeSinceSolveClock;
	Clock newTurnNumberClock;

	//Declare interrupt variable
	bool needInterruption = false;

	//Set up cubes
	Cube const solvedCube;
	Cube newCube(cube);

	//Handle solved cube case
	if (solveAttributes.comparator(newCube, solvedCube))
	{
		oStream << "Cube is already solved!" << std::endl;
		return TurnTypeOrder();
	}

	//Initialize vector of turn Numbers
	std::vector<unsigned int> vectorOfTurnNumbers(initialVectorOfTurnNumbers);

	//Start Solving
	while (true)
	{
		//Create next vector of turn numbers
		bool sizeOfVectorOfTurnNumbersHasChanged;
		std::vector<unsigned int> const oldVectorOfTurnNumbers(vectorOfTurnNumbers);
		createNextVectorOfTurnNumbersForSolvingSmarter(vectorOfTurnNumbers, useOnlyQuarterTurns, sizeOfVectorOfTurnNumbersHasChanged);

		//If size has changed, output time
		if (sizeOfVectorOfTurnNumbersHasChanged)
		{
			unsigned int const size = vectorOfTurnNumbers.size();
			oStream << "Time for checking " << size - 1 << ((size - 1 == 1u) ? " turn: " : " turns: ") << newTurnNumberClock.getElapsedTimeAsMilliseconds() << " ms" << std::endl;
			oStream << std::endl;
			oStream << "Check " << size << ((size == 1u) ? " turn..." : " turns...") << std::endl;
			newTurnNumberClock.restart();
			if (solveAttributes.interruptProperties.type == InterruptProperties::Type::AFTER_TURNS)
			{
				if (size > solveAttributes.interruptProperties.info)
				{
					needInterruption = true;
				}
			}
		}

		//Check for interrupt due to time
		if (solveAttributes.interruptProperties.type == InterruptProperties::Type::AFTER_SECONDS)
		{
			if (timeSinceSolveClock.getElapsedTimeAsMilliseconds() / 1000 >= solveAttributes.interruptProperties.info)
			{
				needInterruption = true;
			}
		}

		//Check if solve needs to be interrupted
		if (needInterruption)
		{
			throw InterruptException(SolverState(oldVectorOfTurnNumbers, cube, solveAttributes));
		}

		//Execute turns of vectorOfTurnNumbers
		for (auto const & num : vectorOfTurnNumbers)
		{
			TurnType turnType = Turn::getTurnTypeFromNumberInArray(num, useOnlyQuarterTurns);
			newCube.turn(turnType);
		}


		//Check if cube is solved
		if (solveAttributes.comparator(newCube, solvedCube))
		{
			TurnTypeOrder turnTypeOrder = Solver::trafoVecOfTurnNumInTurnTypeOrder(vectorOfTurnNumbers, useOnlyQuarterTurns);
			oStream << "Solved! Turns: " << turnTypeOrder << " (Time: " << timeSinceSolveClock.getElapsedTimeAsMilliseconds() << " ms)" << std::endl;
			return turnTypeOrder;
		}

		//Reset newCube
		newCube = cube;
	}
}


TurnTypeOrder Solver::solveUsingRAM(Cube const & cube, SolveAttributes solveAttributes, std::ostream & oStream)
{
	//Translate SolveTurns into bool
	bool useOnlyQuarterTurns = (solveAttributes.solveTurns == SolveTurns::QUARTER_TURNS);

	//Declare Clocks
	Clock timeSinceSolveClock;
	Clock newTurnNumberClock;

	//Declare interrupt variable
	bool needInterruption = false;

	//Set up solved cube
	Cube const solvedCube;

	//Handle solved cube case
	if (solveAttributes.comparator(cube, solvedCube))
	{
		oStream << "Cube is already solved!" << std::endl;
		return TurnTypeOrder();
	}

	//Create Vector of TurnTypeOrder and Cube Pairs
	using TurnedCubesType = std::list<std::pair<TurnTypeOrder, Cube>>;
	TurnedCubesType vecOfTurnedCubes;
	vecOfTurnedCubes.push_back(std::make_pair(TurnTypeOrder{}, cube));

	//Create mode variable
	bool fillMode = true;

	//Fill vecOfTurnedCubes
	while (fillMode)
	{
		std::cout << "Check all turns with " << vecOfTurnedCubes.front().first.size() + 1 << " turns..." << std::endl;
		TurnedCubesType newVecOfTurnedCubes;
		for (auto const & pair : vecOfTurnedCubes)
		{
			std::vector<TurnType> extendingTurnTypes = Solver::getExtendingTurnTypesFor(pair.first, useOnlyQuarterTurns);
			for (auto const & extendingTurnType : extendingTurnTypes)
			{
				Cube newCube = pair.second;
				newCube.turn(extendingTurnType);
				if (solveAttributes.comparator(newCube, solvedCube))
				{
					//Do stuff in case that solution has been found
					oStream << "Solved! Turns: " << pair.first + extendingTurnType << " (Time: " << timeSinceSolveClock.getElapsedTimeAsMilliseconds() << " ms)" << std::endl;
					return pair.first + extendingTurnType;
				}
				else
				{
					//Put newCube into newVecOfTurnedCubes
					newVecOfTurnedCubes.push_back(std::make_pair(pair.first + extendingTurnType, std::move(newCube)));
				}
			}
		}
		vecOfTurnedCubes = std::move(newVecOfTurnedCubes);
		oStream << "Time for checking: " << newTurnNumberClock.getElapsedTimeAsMilliseconds() << " ms" << std::endl;
		newTurnNumberClock.restart();
		fillMode = (vecOfTurnedCubes.front().first.size() < 6u);
	}

	//Use vecOfTurnedCubes to solve faster
	std::cout << "Left fill mode!" << std::endl;
	std::vector<unsigned int> vecOfExtraTurnNumbers;
	
	while (true)
	{
		bool sizeOfVecOfExtraTurnNumbersHasChanged;
		createNextVectorOfTurnNumbersForSolvingSmarter(vecOfExtraTurnNumbers, useOnlyQuarterTurns, sizeOfVecOfExtraTurnNumbersHasChanged);
		if (sizeOfVecOfExtraTurnNumbersHasChanged)
		{
			if (vecOfExtraTurnNumbers.size() != 1u) //This excludes the output of time directly after mode change!
			{
				oStream << "Time for checking: " << newTurnNumberClock.getElapsedTimeAsMilliseconds() << " ms" << std::endl;
			}
			newTurnNumberClock.restart();
			std::cout << "Check all turns with " << vecOfTurnedCubes.front().first.size() + vecOfExtraTurnNumbers.size() << " turns..." << std::endl;
		}
		TurnTypeOrder extraTurnTypeOrder;
		for (auto const & num : vecOfExtraTurnNumbers)
		{
			TurnType turnType = Turn::getTurnTypeFromNumberInArray(num, useOnlyQuarterTurns);
			extraTurnTypeOrder = extraTurnTypeOrder + turnType;
		}

		for (auto const & pair : vecOfTurnedCubes)
		{
			Cube turnedCube = pair.second;
			turnedCube.turn(extraTurnTypeOrder);
			if (solveAttributes.comparator(turnedCube, solvedCube))
			{
				oStream << "Solved! Turns: " << pair.first + extraTurnTypeOrder << " (Time: " << timeSinceSolveClock.getElapsedTimeAsMilliseconds() << " ms)" << std::endl;
				return pair.first + extraTurnTypeOrder;
			}
		}
	}
}


TurnTypeOrder Solver::solveForMultithreading(Cube const & cube, SolveAttributes solveAttributes, std::vector<unsigned int> const & initialVectorOfTurnNumbers, MutexProtectedClass<bool>& solutionFound)
{
	//Translate SolveTurns into bool
	bool useOnlyQuarterTurns = (solveAttributes.solveTurns == SolveTurns::QUARTER_TURNS);

	//Declare Clocks
	Clock timeSinceSolveClock;
	Clock newTurnNumberClock;

	//Declare interrupt variable
	bool needInterruption = false;

	//Set up cubes
	Cube const solvedCube;
	Cube newCube(cube);

	//Handle solved cube case
	if (solveAttributes.comparator(newCube, solvedCube))
	{
		return TurnTypeOrder();
	}

	//Initialize vector of turn Numbers
	std::vector<unsigned int> vectorOfTurnNumbers(initialVectorOfTurnNumbers);

	//Start Solving
	while (!solutionFound.get())
	{
		//Create next vector of turn numbers
		bool sizeOfVectorOfTurnNumbersHasChanged;
		std::vector<unsigned int> const oldVectorOfTurnNumbers(vectorOfTurnNumbers);
		createNextVectorOfTurnNumbersForSolvingSmarter(vectorOfTurnNumbers, useOnlyQuarterTurns, sizeOfVectorOfTurnNumbersHasChanged);

		//If size has changed, output time
		if (sizeOfVectorOfTurnNumbersHasChanged)
		{
			unsigned int const size = vectorOfTurnNumbers.size();
			newTurnNumberClock.restart();
			if (solveAttributes.interruptProperties.type == InterruptProperties::Type::AFTER_TURNS)
			{
				if (size > solveAttributes.interruptProperties.info)
				{
					needInterruption = true;
				}
			}
		}

		//Check for interrupt due to time
		if (solveAttributes.interruptProperties.type == InterruptProperties::Type::AFTER_SECONDS)
		{
			if (timeSinceSolveClock.getElapsedTimeAsMilliseconds() / 1000 >= solveAttributes.interruptProperties.info)
			{
				needInterruption = true;
			}
		}

		//Check if solve needs to be interrupted
		if (needInterruption)
		{
			throw InterruptException(SolverState(oldVectorOfTurnNumbers, cube, solveAttributes));
		}

		//Execute turns of vectorOfTurnNumbers
		for (auto const & num : vectorOfTurnNumbers)
		{
			TurnType turnType = Turn::getTurnTypeFromNumberInArray(num, useOnlyQuarterTurns);
			newCube.turn(turnType);
		}


		//Check if cube is solved
		if (solveAttributes.comparator(newCube, solvedCube))
		{
			TurnTypeOrder turnTypeOrder = Solver::trafoVecOfTurnNumInTurnTypeOrder(vectorOfTurnNumbers, useOnlyQuarterTurns);
			return turnTypeOrder;
		}

		//Reset newCube
		newCube = cube;
	}
	return TurnTypeOrder();
}


TurnTypeOrder Solver::solveOneOfSeveralComparators(Cube const & cube, SolveAttributes solveAttributesWithComparatorBase, std::list<std::pair<Comparator, F2LPairTargetPos>> const & listOfExtraComparatorPairs, std::vector<unsigned int> const & initialVectorOfTurnNumbers, F2LPairTargetPos& solvedPairPosReturn, std::ostream & oStream)
{
	//Setup comarator function
	std::function<bool(Cube const &, Cube const &, F2LPairTargetPos&)> compareFunction =
		[&solveAttributesWithComparatorBase, &listOfExtraComparatorPairs](Cube const & c1, Cube const & c2, F2LPairTargetPos& returnPairPos) -> bool
	{
		if (solveAttributesWithComparatorBase.comparator(c1, c2))
		{
			//Check also other comparators
			for (auto it = listOfExtraComparatorPairs.cbegin(); it != listOfExtraComparatorPairs.cend(); ++it)
			{
				if ((it->first)(c1, c2))
				{
					returnPairPos = it->second;
					return true;
				}
			}
		}
		return false;
	};

	//Translate SolveTurns into bool
	bool useOnlyQuarterTurns = (solveAttributesWithComparatorBase.solveTurns == SolveTurns::QUARTER_TURNS);

	//Declare Clocks
	Clock timeSinceSolveClock;
	Clock newTurnNumberClock;

	//Declare interrupt variable
	bool needInterruption = false;

	//Set up cubes
	Cube const solvedCube;
	Cube newCube(cube);

	//Handle solved cube case
	if (compareFunction(newCube, solvedCube, solvedPairPosReturn))
	{
		oStream << "Cube is already solved!" << std::endl;
		return TurnTypeOrder();
	}

	//Initialize vector of turn Numbers
	std::vector<unsigned int> vectorOfTurnNumbers(initialVectorOfTurnNumbers);

	//Start Solving
	while (true)
	{
		//Create next vector of turn numbers
		bool sizeOfVectorOfTurnNumbersHasChanged;
		std::vector<unsigned int> const oldVectorOfTurnNumbers(vectorOfTurnNumbers);
		createNextVectorOfTurnNumbersForSolvingSmarter(vectorOfTurnNumbers, useOnlyQuarterTurns, sizeOfVectorOfTurnNumbersHasChanged);

		//If size has changed, output time
		if (sizeOfVectorOfTurnNumbersHasChanged)
		{
			unsigned int const size = vectorOfTurnNumbers.size();
			oStream << "Time for checking " << size - 1 << ((size - 1 == 1u) ? " turn: " : " turns: ") << newTurnNumberClock.getElapsedTimeAsMilliseconds() << " ms" << std::endl;
			oStream << std::endl;
			oStream << "Check " << size << ((size == 1u) ? " turn..." : " turns...") << std::endl;
			newTurnNumberClock.restart();
			if (solveAttributesWithComparatorBase.interruptProperties.type == InterruptProperties::Type::AFTER_TURNS)
			{
				if (size > solveAttributesWithComparatorBase.interruptProperties.info)
				{
					needInterruption = true;
				}
			}
		}

		//Check for interrupt due to time
		if (solveAttributesWithComparatorBase.interruptProperties.type == InterruptProperties::Type::AFTER_SECONDS)
		{
			if (timeSinceSolveClock.getElapsedTimeAsMilliseconds() / 1000 >= solveAttributesWithComparatorBase.interruptProperties.info)
			{
				needInterruption = true;
			}
		}

		//Check if solve needs to be interrupted
		if (needInterruption)
		{
			throw InterruptException(SolverState(oldVectorOfTurnNumbers, cube, solveAttributesWithComparatorBase));
		}

		//Execute turns of vectorOfTurnNumbers
		for (auto const & num : vectorOfTurnNumbers)
		{
			TurnType turnType = Turn::getTurnTypeFromNumberInArray(num, useOnlyQuarterTurns);
			newCube.turn(turnType);
		}


		//Check if cube is solved
		if (compareFunction(newCube, solvedCube, solvedPairPosReturn))
		{
			TurnTypeOrder turnTypeOrder = Solver::trafoVecOfTurnNumInTurnTypeOrder(vectorOfTurnNumbers, useOnlyQuarterTurns);
			oStream << "Solved! Turns: " << turnTypeOrder << " (Time: " << timeSinceSolveClock.getElapsedTimeAsMilliseconds() << " ms)" << std::endl;
			return turnTypeOrder;
		}

		//Reset newCube
		newCube = cube;
	}
}


TurnTypeOrder Solver::quicksolve(Cube const & cube, bool giveOutputOnTerminal)
{
	//Copy cube
	Cube c(cube);

	//Create useless ostream object
	std::stringbuf buffer;
	std::ostream log(&buffer);

	//Collect outputs in some stringbuf instead of cout, if giveOutputOnTerminal is false
	std::stringbuf coutBuffer;
	std::ostream oStream(&coutBuffer);
	if (giveOutputOnTerminal)
	{
		oStream.rdbuf(std::cout.rdbuf());
	}
	
	//Check if the number of all colors is 8
	oStream << "Check numbers of colors...";
	std::map<Color, unsigned int> colorNumbers;
	if (!c.numberOfAllColorsIs8(colorNumbers))
	{
		throw Cube::InvalidCubeException(Cube::InvalidCubeException::Reason::NUMBER_PER_COLOR_IS_NOT_8, Cube::InvalidCubeException::Info(colorNumbers));
	}
	oStream << "Done!" << std::endl;

	//Check if all pieces of the cube are valid
	oStream << "Check for invalid pieces...";
	Pieces invalidPieces;
	if (!c.allPiecesValid(invalidPieces))
	{
		throw Cube::InvalidCubeException(Cube::InvalidCubeException::Reason::PIECES_ARE_INVALID, Cube::InvalidCubeException::Info(invalidPieces));
	}
	oStream << "Done!" << std::endl;

	//Solve
	TurnTypeOrder fullTurns;

	oStream << "Solve white cross...";
	TurnTypeOrder whiteCrossTurns;
	Solver::solveWhiteCross(c, log, whiteCrossTurns);
	c.turn(whiteCrossTurns);
	fullTurns += whiteCrossTurns;
	oStream << "Done!" << std::endl;

	oStream << "Solve F2L...";
	TurnTypeOrder F2LTurns;
	//Solver::solveF2L(c, log, F2LTurns);
	//Solver::solveF2LMultithreaded(c, log, F2LTurns);
	//Solver::solveF2LByCheckingMultiplePairsAtOnce(c, log, F2LTurns);
	Solver::solveF2LByCheckingMultiplePairsAtOnceAndFirstInsertingCorners(c, log, F2LTurns);
	c.turn(F2LTurns);
	fullTurns += F2LTurns;
	oStream << "Done!" << std::endl;
	
	oStream << "Solve OLL...";
	TurnTypeOrder OLLTurns;
	Solver::solveOLL(c, log, OLLTurns);
	c.turn(OLLTurns);
	fullTurns += OLLTurns;
	oStream << "Done!" << std::endl;

	oStream << "Solve PLL...";
	TurnTypeOrder PLLTurns;
	Solver::solvePLL(c, log, PLLTurns);
	c.turn(PLLTurns);
	fullTurns += PLLTurns;
	oStream << "Done!" << std::endl;


	return fullTurns;
}


bool Solver::solveWhiteCross(Cube const & c, std::ostream & log, TurnTypeOrder& t)
{
	Cube cubeCopy(c);
	TurnTypeOrder pureCrossTurns = Solver::solve(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::PureWhiteCross, InterruptProperties(InterruptProperties::Type::NEVER)), {}, log);
	cubeCopy.turn(pureCrossTurns);
	TurnTypeOrder fullCrossTurns = Solver::solve(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::FullWhiteCross, InterruptProperties(InterruptProperties::Type::NEVER)), {}, log);

	t = pureCrossTurns + fullCrossTurns;
	return true;
}


bool Solver::solveF2L(Cube const & c, std::ostream & log, TurnTypeOrder& t)
{
	Solver::solveF2LusingPairOrder(c, log, t, { F2LPairTargetPos::LEFT_BACK, F2LPairTargetPos::LEFT_FRONT, F2LPairTargetPos::RIGHT_BACK, F2LPairTargetPos::RIGHT_FRONT });
	return true;
}


bool Solver::solveF2LusingPairOrder(Cube const & c, std::ostream & log, TurnTypeOrder& t, std::array<F2LPairTargetPos, 4> const & pairOrder)
{
	std::function<Comparator const & (F2LPairTargetPos const &)> mapPairPosToComparator = [](F2LPairTargetPos const & pairPos) -> Comparator const &
	{
		switch (pairPos)
		{
		case F2LPairTargetPos::LEFT_BACK:
			return Comparator::Pair::BackLeft;
			break;
		case F2LPairTargetPos::LEFT_FRONT:
			return Comparator::Pair::FrontLeft;
			break;
		case F2LPairTargetPos::RIGHT_BACK:
			return Comparator::Pair::BackRight;
			break;
		case F2LPairTargetPos::RIGHT_FRONT:
			return Comparator::Pair::FrontRight;
			break;
		}
	};

	std::array<Comparator, 4> const cmp{
		mapPairPosToComparator(pairOrder.at(0)),
		mapPairPosToComparator(pairOrder.at(1)),
		mapPairPosToComparator(pairOrder.at(2)),
		mapPairPosToComparator(pairOrder.at(3))
	};

	Cube cubeCopy(c);

	TurnTypeOrder t1 = Solver::solve(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::FullWhiteCross && cmp.at(0), InterruptProperties(InterruptProperties::Type::NEVER)), {}, log);
	cubeCopy.turn(t1);
	TurnTypeOrder t2 = Solver::solve(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::FullWhiteCross && cmp.at(0) && cmp.at(1), InterruptProperties(InterruptProperties::Type::NEVER)), {}, log);
	cubeCopy.turn(t2);
	TurnTypeOrder t3 = Solver::solve(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::FullWhiteCross && cmp.at(0) && cmp.at(1) && cmp.at(2), InterruptProperties(InterruptProperties::Type::NEVER)), {}, log);
	cubeCopy.turn(t3);
	TurnTypeOrder t4 = Solver::solve(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::SecondLayer, InterruptProperties(InterruptProperties::Type::NEVER)), {}, log);
	cubeCopy.turn(t4);

	t = t1 + t2 + t3 + t4;
	return true;
}


void Solver::solveF2LusingPairOrderForMultithreading(Cube const & c, TurnTypeOrder& t, std::array<F2LPairTargetPos, 4> const & pairOrder, MutexProtectedClass<bool>& solutionFound, bool& hasThisFunctionFoundTheSolution)
{
	std::function<Comparator const & (F2LPairTargetPos const &)> mapPairPosToComparator = [](F2LPairTargetPos const & pairPos) -> Comparator const &
	{
		switch (pairPos)
		{
		case F2LPairTargetPos::LEFT_BACK:
			return Comparator::Pair::BackLeft;
			break;
		case F2LPairTargetPos::LEFT_FRONT:
			return Comparator::Pair::FrontLeft;
			break;
		case F2LPairTargetPos::RIGHT_BACK:
			return Comparator::Pair::BackRight;
			break;
		case F2LPairTargetPos::RIGHT_FRONT:
			return Comparator::Pair::FrontRight;
			break;
		}
	};

	std::array<Comparator, 4> const cmp{
		mapPairPosToComparator(pairOrder.at(0)),
		mapPairPosToComparator(pairOrder.at(1)),
		mapPairPosToComparator(pairOrder.at(2)),
		mapPairPosToComparator(pairOrder.at(3))
	};

	Cube cubeCopy(c);

	TurnTypeOrder t1 = Solver::solveForMultithreading(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::FullWhiteCross && cmp.at(0), InterruptProperties(InterruptProperties::Type::NEVER)), {}, solutionFound);
	if (solutionFound.get())
	{
		return;
	}
	std::cout << "1/4\n";
	cubeCopy.turn(t1);
	TurnTypeOrder t2 = Solver::solveForMultithreading(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::FullWhiteCross && cmp.at(0) && cmp.at(1), InterruptProperties(InterruptProperties::Type::NEVER)), {}, solutionFound);
	if (solutionFound.get())
	{
		return;
	}
	std::cout << "2/4\n";
	cubeCopy.turn(t2);
	TurnTypeOrder t3 = Solver::solveForMultithreading(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::FullWhiteCross && cmp.at(0) && cmp.at(1) && cmp.at(2), InterruptProperties(InterruptProperties::Type::NEVER)), {}, solutionFound);
	if (solutionFound.get())
	{
		return;
	}
	std::cout << "3/4\n";
	cubeCopy.turn(t3);
	TurnTypeOrder t4 = Solver::solveForMultithreading(cubeCopy, SolveAttributes(SolveTurns::ALL, Comparator::SecondLayer, InterruptProperties(InterruptProperties::Type::NEVER)), {}, solutionFound);
	if (solutionFound.get())
	{
		return;
	}
	std::cout << "4/4\n";
	cubeCopy.turn(t4);

	t = t1 + t2 + t3 + t4;
	hasThisFunctionFoundTheSolution = true;
	solutionFound.set(true);
}


bool Solver::solveF2LMultithreaded(Cube const & c, std::ostream & log, TurnTypeOrder& t)
{
	//////////////////////////////
	//Generate array of pairOrders
	unsigned int constexpr permNum = 24u;
	std::array<std::array<unsigned int, 4>, permNum> const permutationArray
	{
		std::array<unsigned int, 4>{ 0, 1, 3, 2 },
		std::array<unsigned int, 4>{ 0, 1, 2, 3 },
		std::array<unsigned int, 4>{ 0, 2, 1, 3 },
		std::array<unsigned int, 4>{ 0, 2, 3, 1 },
		std::array<unsigned int, 4>{ 0, 3, 1, 2 },
		std::array<unsigned int, 4>{ 0, 3, 2, 1 },
		std::array<unsigned int, 4>{ 1, 0, 2, 3 },
		std::array<unsigned int, 4>{ 1, 0, 3, 2 },
		std::array<unsigned int, 4>{ 1, 2, 0, 3 },
		std::array<unsigned int, 4>{ 1, 2, 3, 0 },
		std::array<unsigned int, 4>{ 1, 3, 0, 2 },
		std::array<unsigned int, 4>{ 1, 3, 2, 0 },
		std::array<unsigned int, 4>{ 2, 0, 1, 3 },
		std::array<unsigned int, 4>{ 2, 0, 3, 1 },
		std::array<unsigned int, 4>{ 2, 1, 0, 3 },
		std::array<unsigned int, 4>{ 2, 1, 3, 0 },
		std::array<unsigned int, 4>{ 2, 3, 0, 1 },
		std::array<unsigned int, 4>{ 2, 3, 1, 0 },
		std::array<unsigned int, 4>{ 3, 0, 1, 2 },
		std::array<unsigned int, 4>{ 3, 0, 2, 1 },
		std::array<unsigned int, 4>{ 3, 1, 0, 2 },
		std::array<unsigned int, 4>{ 3, 1, 2, 0 },
		std::array<unsigned int, 4>{ 3, 2, 0, 1 },
		std::array<unsigned int, 4>{ 3, 2, 1, 0 }
	};

	std::array<std::array<F2LPairTargetPos, 4>, permNum> arrayOfPairOrders;
	for (unsigned int i = 0; i < permNum; ++i)
	{
		std::array<unsigned int, 4> perm = permutationArray.at(i);
		arrayOfPairOrders.at(i) = { static_cast<F2LPairTargetPos>(perm.at(0)), static_cast<F2LPairTargetPos>(perm.at(1)) , static_cast<F2LPairTargetPos>(perm.at(2)) , static_cast<F2LPairTargetPos>(perm.at(3)) };
	}

	////////////////////////////////////////////////////////////
	//Use arrayOfPairOrders to construct several solving threads

	//Initialize needed variables
	std::array<std::thread*, permNum> arrayOfThreadPointers; //This will contain the thread pointers
	std::array<bool, permNum> arrayOfHasThisFunctionFoundTheSolutionObjects; //This will save for each thread its own solution found variable
	std::array<TurnTypeOrder, permNum> arrayOfSolvingTurnTypeOrders; //This will save for each thread its own solving turns
	for (unsigned int i = 0; i < permNum; ++i) //Initialize all thisFunctionFoundObjects to false
	{
		arrayOfHasThisFunctionFoundTheSolutionObjects.at(i) = false;
	}
	MutexProtectedClass<bool> solutionFound; //This is concurrency save! It is used to signal if a solution is found and when all threads need to return!
	solutionFound.set(false); //It is initialized to false

	//Create Threads
	for (unsigned int i = 0; i < permNum; ++i)
	{
		arrayOfThreadPointers.at(i) = new std::thread(Solver::solveF2LusingPairOrderForMultithreading, std::cref(c), std::ref(arrayOfSolvingTurnTypeOrders.at(i)), std::cref(arrayOfPairOrders.at(i)), std::ref(solutionFound), std::ref(arrayOfHasThisFunctionFoundTheSolutionObjects.at(i)));
	}

	//Let them all join! When the first one solved, all others will return and all threads will join!
	for (auto threadPointer : arrayOfThreadPointers)
	{
		threadPointer->join();
	}

	//After all threads have joined, they can be savely deleted!
	for (auto threadPointer : arrayOfThreadPointers)
	{
		delete threadPointer;
	}

	//Search the array position which found the solution and return the value!
	for (unsigned int i = 0; i < permNum; ++i)
	{
		if (arrayOfHasThisFunctionFoundTheSolutionObjects.at(i))
		{
			TurnTypeOrder correctSolution = arrayOfSolvingTurnTypeOrders.at(i);
			t = correctSolution;
			return true;
		}
	}
}


bool Solver::solveF2LByCheckingMultiplePairsAtOnce(Cube const & c, std::ostream & log, TurnTypeOrder& t) //This function has put out "Remaining pairs: 4 Remaining pairs: 4 Remaining pairs: 4 ..."! But I can't find the error in this function! So it is probably in the solve function!
{
	std::function<Comparator const & (F2LPairTargetPos const &)> mapPairPosToComparator = [](F2LPairTargetPos const & pairPos) -> Comparator const &
	{
		switch (pairPos)
		{
		case F2LPairTargetPos::LEFT_BACK:
			return Comparator::Pair::BackLeft;
			break;
		case F2LPairTargetPos::LEFT_FRONT:
			return Comparator::Pair::FrontLeft;
			break;
		case F2LPairTargetPos::RIGHT_BACK:
			return Comparator::Pair::BackRight;
			break;
		case F2LPairTargetPos::RIGHT_FRONT:
			return Comparator::Pair::FrontRight;
			break;
		}
	};

	Cube cubeCopy(c);

	std::list<F2LPairTargetPos> const listOfAllPairTargetPositions{F2LPairTargetPos::LEFT_BACK, F2LPairTargetPos::LEFT_FRONT, F2LPairTargetPos::RIGHT_BACK, F2LPairTargetPos::RIGHT_FRONT};
	std::set<F2LPairTargetPos> setOfRemainingPairTargetPositions(listOfAllPairTargetPositions.begin(), listOfAllPairTargetPositions.end());
	std::set<F2LPairTargetPos> setOfAlreadyUsedPairTargetPositions;

	t = {};

	while (!setOfRemainingPairTargetPositions.empty())
	{
		Comparator baseComparator = Comparator::FullWhiteCross;
		for (auto pairPos : setOfAlreadyUsedPairTargetPositions)
		{
			baseComparator = baseComparator && mapPairPosToComparator(pairPos);
		}

		std::list<std::pair<Comparator, F2LPairTargetPos>> extraComparatorPairs;
		for (auto pairPos : setOfRemainingPairTargetPositions)
		{
			extraComparatorPairs.push_back(std::make_pair(mapPairPosToComparator(pairPos), pairPos));
		}

		F2LPairTargetPos solvedPairPos;
		TurnTypeOrder solvingTurns = Solver::solveOneOfSeveralComparators(cubeCopy, SolveAttributes(SolveTurns::ALL, baseComparator, InterruptProperties(InterruptProperties::Type::NEVER)), extraComparatorPairs, {}, solvedPairPos, log);
		t += solvingTurns;
		cubeCopy.turn(solvingTurns);

		setOfRemainingPairTargetPositions.erase(solvedPairPos);
		setOfAlreadyUsedPairTargetPositions.insert(solvedPairPos);

		std::cout << "Remaining pairs: " << setOfRemainingPairTargetPositions.size() << std::endl;
	}

	return true;
}


bool Solver::solveF2LByCheckingMultiplePairsAtOnceAndFirstInsertingCorners(Cube const & c, std::ostream & log, TurnTypeOrder& t) //This function has put out "Remaining pairs: 4 Remaining pairs: 4 Remaining pairs: 4 ..."! But I can't find the error in this function! So it is probably in the solve function!
{
	std::function<Comparator const & (F2LPairTargetPos const &)> mapPairPosToComparator = [](F2LPairTargetPos const & pairPos) -> Comparator const &
	{
		switch (pairPos)
		{
		case F2LPairTargetPos::LEFT_BACK:
			return Comparator::Pair::BackLeft;
			break;
		case F2LPairTargetPos::LEFT_FRONT:
			return Comparator::Pair::FrontLeft;
			break;
		case F2LPairTargetPos::RIGHT_BACK:
			return Comparator::Pair::BackRight;
			break;
		case F2LPairTargetPos::RIGHT_FRONT:
			return Comparator::Pair::FrontRight;
			break;
		}
	};

	std::function<Comparator const & (F2LPairTargetPos const &)> mapPairPosToCornerComparator = [](F2LPairTargetPos const & pairPos) -> Comparator const &
	{
		switch (pairPos)
		{
		case F2LPairTargetPos::LEFT_BACK:
			return Comparator::FullCorner::UpBackLeft;
			break;
		case F2LPairTargetPos::LEFT_FRONT:
			return Comparator::FullCorner::UpFrontLeft;
			break;
		case F2LPairTargetPos::RIGHT_BACK:
			return Comparator::FullCorner::UpBackRight;
			break;
		case F2LPairTargetPos::RIGHT_FRONT:
			return Comparator::FullCorner::UpFrontRight;
			break;
		}
	};

	std::function<Comparator const & (F2LPairTargetPos const &)> mapPairPosToEdgeComparator = [](F2LPairTargetPos const & pairPos) -> Comparator const &
	{
		switch (pairPos)
		{
		case F2LPairTargetPos::LEFT_BACK:
			return Comparator::Edge::BackLeft;
			break;
		case F2LPairTargetPos::LEFT_FRONT:
			return Comparator::Edge::FrontLeft;
			break;
		case F2LPairTargetPos::RIGHT_BACK:
			return Comparator::Edge::BackRight;
			break;
		case F2LPairTargetPos::RIGHT_FRONT:
			return Comparator::Edge::FrontRight;
			break;
		}
	};

	Cube cubeCopy(c);
	t = {};
	std::list<F2LPairTargetPos> const listOfAllPairTargetPositions{ F2LPairTargetPos::LEFT_BACK, F2LPairTargetPos::LEFT_FRONT, F2LPairTargetPos::RIGHT_BACK, F2LPairTargetPos::RIGHT_FRONT };

	//First, insert all corners
	{
		std::set<F2LPairTargetPos> setOfRemainingPairTargetPositions(listOfAllPairTargetPositions.begin(), listOfAllPairTargetPositions.end());
		std::set<F2LPairTargetPos> setOfAlreadyUsedPairTargetPositions;

		while (!setOfRemainingPairTargetPositions.empty())
		{
			Comparator baseComparator = Comparator::FullWhiteCross;
			for (auto pairPos : setOfAlreadyUsedPairTargetPositions)
			{
				baseComparator = baseComparator && mapPairPosToCornerComparator(pairPos);
			}

			std::list<std::pair<Comparator, F2LPairTargetPos>> extraComparatorPairs;
			for (auto pairPos : setOfRemainingPairTargetPositions)
			{
				extraComparatorPairs.push_back(std::make_pair(mapPairPosToCornerComparator(pairPos), pairPos));
			}

			F2LPairTargetPos solvedPairPos;
			TurnTypeOrder solvingTurns = Solver::solveOneOfSeveralComparators(cubeCopy, SolveAttributes(SolveTurns::QUARTER_TURNS, baseComparator, InterruptProperties(InterruptProperties::Type::NEVER)), extraComparatorPairs, {}, solvedPairPos, log);
			t += solvingTurns;
			cubeCopy.turn(solvingTurns);

			setOfRemainingPairTargetPositions.erase(solvedPairPos);
			setOfAlreadyUsedPairTargetPositions.insert(solvedPairPos);

			//std::cout << "Remaining pairs: " << setOfRemainingPairTargetPositions.size() << std::endl;
		}
	}

	//Secondly, solve full pairs
	{
		std::list<F2LPairTargetPos> listOfCurrentPairPos;
		for (auto pairPos : listOfAllPairTargetPositions)
		{
			listOfCurrentPairPos.push_back(pairPos);

			Comparator cmp = Comparator::FirstLayer;
			for (auto cmpPairPos : listOfCurrentPairPos)
			{
				cmp = cmp && mapPairPosToEdgeComparator(cmpPairPos);
			}

			std::list<TurnTypeOrder> listOfSetupMoves = { {}, {TurnType::Down}, {TurnType::DownInverse}, {TurnType::Down2 } };
			std::list<TurnTypeOrder> const listOfExtractions = F2L::getListOfExtractions();
			for (auto const & extraction : listOfExtractions)
			{
				listOfSetupMoves.push_back(extraction);
				listOfSetupMoves.push_back(extraction + TurnType::Down);
				listOfSetupMoves.push_back(extraction + TurnType::DownInverse);
				listOfSetupMoves.push_back(extraction + TurnType::Down2);
			}

			TurnTypeOrder solvingTurns;
			if (!Solver::tryTurnsToSolve(cubeCopy, cmp, F2L::listOfSecondLayerAlgorithms, listOfSetupMoves, solvingTurns))
			{
				std::cout << "Could not solve! Learn programming!" << std::endl;
			}
			t += solvingTurns;
			cubeCopy.turn(solvingTurns);
		}

		
	}
	
	//Old full pairs solving algorithm
	//{
	//	std::set<F2LPairTargetPos> setOfRemainingPairTargetPositions(listOfAllPairTargetPositions.begin(), listOfAllPairTargetPositions.end());
	//	std::set<F2LPairTargetPos> setOfAlreadyUsedPairTargetPositions;
	//
	//	while (!setOfRemainingPairTargetPositions.empty())
	//	{
	//		Comparator baseComparator = Comparator::FullWhiteCross;
	//		for (auto pairPos : setOfAlreadyUsedPairTargetPositions)
	//		{
	//			baseComparator = baseComparator && mapPairPosToComparator(pairPos);
	//		}
	//
	//		std::list<std::pair<Comparator, F2LPairTargetPos>> extraComparatorPairs;
	//		for (auto pairPos : setOfRemainingPairTargetPositions)
	//		{
	//			extraComparatorPairs.push_back(std::make_pair(mapPairPosToComparator(pairPos), pairPos));
	//		}
	//
	//		F2LPairTargetPos solvedPairPos;
	//		TurnTypeOrder solvingTurns = Solver::solveOneOfSeveralComparators(cubeCopy, SolveAttributes(SolveTurns::QUARTER_TURNS, baseComparator, InterruptProperties(InterruptProperties::Type::NEVER)), extraComparatorPairs, {}, solvedPairPos, log);
	//		t += solvingTurns;
	//		cubeCopy.turn(solvingTurns);
	//
	//		setOfRemainingPairTargetPositions.erase(solvedPairPos);
	//		setOfAlreadyUsedPairTargetPositions.insert(solvedPairPos);
	//
	//		std::cout << "Remaining pairs: " << setOfRemainingPairTargetPositions.size() << std::endl;
	//	}
	//}

	return true;
}


bool Solver::solveOLL(Cube const & c, std::ostream & log, TurnTypeOrder& t)
{
	std::function<std::pair<unsigned int, unsigned int>(Cube const &)> assuming2YellowEdgesGetYellowPositions = [](Cube const & cube) -> std::pair<unsigned int, unsigned int>
	{
		std::pair<unsigned int, unsigned int> pair;
		unsigned int found = 0u;
		for (unsigned int i = 1; i < 8; i = i + 2)
		{
			if (cube.at(PlanePos::DOWN).at(i) == Color::YELLOW)
			{
				if (found == 0u)
				{
					pair.first = i;
				}
				else
				{
					pair.second = i;
				}
				++found;
				if (found >= 2u)
				{
					return pair;
				}
			}
		}
	};

	std::function<unsigned int(std::pair<unsigned int, unsigned int> const &)> assuming2YellowEdgesAsCurveGetCorrespondingCornerPosition = [](std::pair<unsigned int, unsigned int> const & yellowPositions) -> unsigned int
	{
		unsigned int cornerPos = (yellowPositions.first + yellowPositions.second) / 2; //Yields corner pos if pair is not (7, 1)! Then it yields the same as (3, 5)!
		if (cornerPos == 4u)
		{
			int difference = std::abs(static_cast<int>(yellowPositions.first) - static_cast<int>(yellowPositions.second)); //Yields 6 or 2!
			if (difference == 2)
			{
				return 4u;
			}
			else
			{
				return 0u;
			}
		}
		else
		{
			return cornerPos;
		}
	};

	//Solve yellow cross
	TurnTypeOrder crossSolvingTurns;
	unsigned int numberOfYellows = 0;
	Plane yellowPlane = c.at(PlanePos::DOWN);
	for (unsigned int i = 1; i < 8; i = i + 2)
	{
		numberOfYellows += ((yellowPlane.at(i) == Color::YELLOW) ? 1 : 0);
	}
	switch (numberOfYellows)
	{
	case 1:
	case 3:
		throw Cube::InvalidCubeException(Cube::InvalidCubeException::Reason::ONE_EDGE_IS_TILTED);
		break;
	case 0:
		crossSolvingTurns = OLL::CrossPoint;
		break;
	case 2:
	{
		std::pair<unsigned int, unsigned int> yellowPositions = assuming2YellowEdgesGetYellowPositions(c);
		if (std::abs(static_cast<int>(yellowPositions.first) - static_cast<int>(yellowPositions.second)) == 4) //Line
		{
			if (yellowPositions.first % 4 == 3) //Correct oriented
			{
				crossSolvingTurns = OLL::CrossLine;
			}
			else //Incorrect oriented
			{
				crossSolvingTurns = TurnType::Down + OLL::CrossLine;
			}
		}
		else //Curve
		{
			int cornerPos = assuming2YellowEdgesAsCurveGetCorrespondingCornerPosition(yellowPositions);
			switch (cornerPos)
			{
			case 0:
				crossSolvingTurns = OLL::CrossCurve;
				break;
			case 2:
				crossSolvingTurns = TurnType::DownInverse + OLL::CrossCurve;
				break;
			case 4:
				crossSolvingTurns = TurnType::Down2 + OLL::CrossCurve;
				break;
			case 6:
				crossSolvingTurns = TurnType::Down + OLL::CrossCurve;
				break;
			}
		}
		break;
	}
	case 4:
		break;
	}
	Cube cubeCopy(c);
	cubeCopy.turn(crossSolvingTurns);

	//Solve yellow corners
	std::function<bool(Cube const &)> assumingThatOneCornerIsRotatedDetermineIfCubeCornerIsRotatedClockwise = [](Cube const & c) -> bool
	{
		//This function assumes that the cube has already a solved second layer and a yellow cross and passed the first two validity checks!
		//Goal: Determine the direction into which a corner is rotated!
		//Idea: Count clockwise rotations and use modulo calculus
		int clockwiseCounter = 0;

		//Check Bottom 0
		if (c.at(PlanePos::DOWN).at(0) != Color::YELLOW)
		{
			if (c.at(PlanePos::LEFT).at(4) == Color::YELLOW)
			{
				++clockwiseCounter;
			}
			else
			{
				--clockwiseCounter;
			}
		}

		//Check Bottom 2
		if (c.at(PlanePos::DOWN).at(2) != Color::YELLOW)
		{
			if (c.at(PlanePos::FRONT).at(4) == Color::YELLOW)
			{
				++clockwiseCounter;
			}
			else
			{
				--clockwiseCounter;
			}
		}

		//Check Bottom 4
		if (c.at(PlanePos::DOWN).at(4) != Color::YELLOW)
		{
			if (c.at(PlanePos::RIGHT).at(4) == Color::YELLOW)
			{
				++clockwiseCounter;
			}
			else
			{
				--clockwiseCounter;
			}
		}

		//Check Bottom 6
		if (c.at(PlanePos::DOWN).at(6) != Color::YELLOW)
		{
			if (c.at(PlanePos::BACK).at(4) == Color::YELLOW)
			{
				++clockwiseCounter;
			}
			else
			{
				--clockwiseCounter;
			}
		}

		int globalClockwiseRotation = ((clockwiseCounter + 6) % 3);
		if (globalClockwiseRotation == 0)
		{
			std::cout << "Lambda in Solver::solveOLL has been called, although there is no global rotation of corners! Learn programming!" << std::endl;
			std::cout << c;
		}
		else
		{
			return (globalClockwiseRotation == 1);
		}

	};

	TurnTypeOrder cornersSolvingTurns;
	if (!Solver::tryTurnsToSolve(cubeCopy, Comparator::SecondLayer && Comparator::YellowPlane, OLL::listOfCornerOLLs, { {}, {TurnType::Down}, {TurnType::DownInverse}, {TurnType::Down2} }, cornersSolvingTurns))
	{
		throw Cube::InvalidCubeException(Cube::InvalidCubeException::Reason::ONE_CORNER_IS_ROTATED, Cube::InvalidCubeException::Info(assumingThatOneCornerIsRotatedDetermineIfCubeCornerIsRotatedClockwise(c)));
	}

	t = (crossSolvingTurns + cornersSolvingTurns);
	return true;
}


bool Solver::solvePLL(Cube const & c, std::ostream & log, TurnTypeOrder& t)
{
	TurnTypeOrder pllTurns;
	if (!Solver::tryTurnsToSolve(c, Comparator::FullCube, PLL::listOfPLLs, { {},{ TurnType::Down },{ TurnType::DownInverse },{ TurnType::Down2 } }, pllTurns))
	{
		throw Cube::InvalidCubeException(Cube::InvalidCubeException::Reason::TWO_PIECES_ARE_EXCHANGED);
	}

	t = pllTurns;
	return true;
}


bool Solver::tryTurnsToSolve(Cube const & c, Comparator const & comparator, std::list<TurnTypeOrder const *> const & listOfTurns, std::list<TurnTypeOrder> const & listOfSetupMoves, TurnTypeOrder& turnsReturn)
{
	std::list<TurnTypeOrder const *> listOfTurnsIncludingEmptyTurn(listOfTurns);
	listOfTurnsIncludingEmptyTurn.push_front(&EmptyTurnTypeOrder::Empty);

	Cube const solvedCube;
	Cube cubeCopy(c);
	for (auto turns : listOfTurnsIncludingEmptyTurn)
	{
		for (auto const & setup : listOfSetupMoves)
		{
			for (auto const & postSetup : listOfSetupMoves)
			{
				cubeCopy = c;
				cubeCopy.turn(setup);
				cubeCopy.turn(*turns);
				cubeCopy.turn(postSetup);
				if (comparator(cubeCopy, solvedCube))
				{
					turnsReturn = (setup + *turns + postSetup);
					return true;
				}
			}
		}
	}
	return false;
}


Solver::SolveAttributes Solver::askForUserInput()
{
	//Enter solving subprogram
	std::cout << "Enter solving subprogram (For abortion type \"quit\")" << std::endl;

	//Ask for solver mode
	std::string experimentalModeString = Solver::demandUserInput( 
		"Which mode do you want to use?\n"
		"1: Classic mode\n"
		"2: RAM mode\n",
		{ "1", "2" });
	SolverMode solverMode;
	if (experimentalModeString == "1")
	{
		solverMode = SolverMode::Classic;
	}
	if (experimentalModeString == "2")
	{
		solverMode = SolverMode::RAM;
	}

	//Find out SolveTurns
	std::string solveTurnsString = Solver::demandUserInput(
		"Which turns are allowed?\n"
		"1: Only quarter turns\n"
		"2: All turns\n",
		{ "1", "2" }
	);
	int solveTurnsNumber = std::stoi(solveTurnsString);
	SolveTurns solveTurns = ((solveTurnsNumber == 1) ? SolveTurns::QUARTER_TURNS : SolveTurns::ALL);


	//Find out comparator
	std::string comparatorString = Solver::demandUserInput(
		"Which part of the cube shall be solved?\n"
		"1: White cross\n"
		"2: First layer\n"
		"3: First two layers\n"
		"4: First two layers + orientation of last layer\n"
		"5: Full cube\n",
		{ "1", "2", "3", "4", "5" }
	);
	int comparatorNumber = std::stoi(comparatorString);
	Comparator cmp = Comparator::FullCube;
	switch (comparatorNumber)
	{
	case 1:
		cmp = Comparator::FullWhiteCross;
		break;
	case 2:
		cmp = Comparator::FirstLayer;
		break;
	case 3:
		cmp = Comparator::SecondLayer;
		break;
	case 4:
		cmp = (Comparator::SecondLayer && Comparator::YellowPlane);
		break;
	case 5:
		cmp = Comparator::FullCube;
		break;
	}


	//Find out interrupt properties
	InterruptProperties interruptProp = Solver::askForInterruptPropertiesUserInput();

	//Return SolveAttributes
	return SolveAttributes(solveTurns, cmp, interruptProp, solverMode);
}


Solver::InterruptProperties Solver::askForInterruptPropertiesUserInput()
{
	InterruptProperties interruptProp;
	std::string interruptString = Solver::demandUserInput(
		"Which interrupt behaviour do you want?\n"
		"1: No interruption\n"
		"2: Interrupt after some time\n"
		"3: Interrupt after having checked all turns of some length\n",
		{ "1", "2", "3" }
	);
	int interruptInt = std::stoi(interruptString);
	switch (interruptInt)
	{
	case 1:
		interruptProp = InterruptProperties(InterruptProperties::Type::NEVER);
		break;
	case 2:
	{
		bool correctInput;
		int secondsInt;
		do
		{
			correctInput = true;
			std::string secondsString = Solver::demandUserInput("Interrupt after how many seconds?\n", {});
			try
			{
				secondsInt = std::stoi(secondsString);
			}
			catch (...)
			{
				correctInput = false;
			}
		} while (!correctInput);
		interruptProp = InterruptProperties(InterruptProperties::Type::AFTER_SECONDS, secondsInt);
		break;
	}
	case 3:
	{
		bool correctInput;
		int turnsInt;
		do
		{
			correctInput = true;
			std::string turnsString = Solver::demandUserInput("Interrupt after how many turns?\n", {});
			try
			{
				turnsInt = std::stoi(turnsString);
			}
			catch (...)
			{
				correctInput = false;
			}
		} while (!correctInput);
		interruptProp = InterruptProperties(InterruptProperties::Type::AFTER_TURNS, turnsInt);
		break;
	}
	}
	return interruptProp;
}


bool Solver::askIfUserWantsToContinueSolving(InterruptProperties& newInterruptProperties)
{
	std::string userInput = demandUserInput("Do you want to continue solving? (y or n)\n", { "y", "n" });
	if (userInput == "y")
	{
		InterruptProperties interruptProperties = Solver::askForInterruptPropertiesUserInput();
		newInterruptProperties = interruptProperties;
		return true;
	}
	else
	{
		return false;
	}
}


std::string Solver::demandUserInput(std::string const & message, std::list<std::string> listOfValidStrings)
{
	while (true)
	{
		std::cout << message;
		std::string line;
		std::getline(std::cin, line);
		if (line == "quit")
		{
			throw Solver::QuitException();
		}
		if (listOfValidStrings.empty())
		{
			return line;
		}
		for (auto const & validString : listOfValidStrings)
		{
			if (line == validString)
			{
				return line;
			}
		}
		std::cout << "Input was invalid! Try again!" << std::endl;
	}
}



void Solver::createNextVectorOfTurnNumbersForSolving(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged)
{
	if (vecOfTurnNumbers.empty())
	{
		vecOfTurnNumbers = std::vector<unsigned int>({ 0u });
		turnNumberVecSizeChanged = true;
		return;
	}

	vecOfTurnNumbers.at(vecOfTurnNumbers.size() - 1) += 1;
	bool hasSizeOfTurnNumbersVecChanged = false;
	bool hasVecChanged = false;
	Solver::handleOverflowOfTurnNumbersVec(vecOfTurnNumbers, useOnlyQuarterTurns, hasSizeOfTurnNumbersVecChanged, hasVecChanged);
	turnNumberVecSizeChanged = hasSizeOfTurnNumbersVecChanged;
}



void Solver::createNextVectorOfTurnNumbersForSolvingSmart(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged)
{
	//Difference to Cube::createNextVectorOfTurnNummbersForSolving: Hidden inverses are skipped!

	if (vecOfTurnNumbers.empty())
	{
		vecOfTurnNumbers = std::vector<unsigned int>({ 0u });
		turnNumberVecSizeChanged = true;
		return;
	}

	vecOfTurnNumbers.at(vecOfTurnNumbers.size() - 1) += 1;

	bool hasSizeOfTurnNumbersVecChangedDueToOverflowHandling = false;
	bool hasVecChangedDueToOverflowHandling = false;
	Solver::handleOverflowOfTurnNumbersVec(vecOfTurnNumbers, useOnlyQuarterTurns, hasSizeOfTurnNumbersVecChangedDueToOverflowHandling, hasVecChangedDueToOverflowHandling);

	bool hasSizeOfTurnNumbersVecChangedDueToHiddenInverseSkipping = false;
	bool hasVecChangedDueToHiddenInverseSkipping = false;
	Solver::skipHiddenInverses(vecOfTurnNumbers, useOnlyQuarterTurns, hasSizeOfTurnNumbersVecChangedDueToHiddenInverseSkipping, hasVecChangedDueToHiddenInverseSkipping);

	turnNumberVecSizeChanged = hasSizeOfTurnNumbersVecChangedDueToOverflowHandling || hasSizeOfTurnNumbersVecChangedDueToHiddenInverseSkipping;
}






void Solver::createNextVectorOfTurnNumbersForSolvingSmarter(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged)
{
	//Difference to Cube::createNextVectorOfTurnNummbersForSolvingSmart: Sometimes, following equal numbers are skipped!

	if (vecOfTurnNumbers.empty())
	{
		vecOfTurnNumbers = std::vector<unsigned int>({ 0u });
		turnNumberVecSizeChanged = true;
		return;
	}

	vecOfTurnNumbers.at(vecOfTurnNumbers.size() - 1) += 1;

	bool hasSizeOfTurnNumbersVecChangedDueToOverflowHandling = false;
	bool hasVecChangedDueToOverflowHandlingPlaceholder;
	Solver::handleOverflowOfTurnNumbersVec(vecOfTurnNumbers, useOnlyQuarterTurns, hasSizeOfTurnNumbersVecChangedDueToOverflowHandling, hasVecChangedDueToOverflowHandlingPlaceholder);

	bool hasSizeOfTurnNumbersVecChangedDueToHiddenInverseSkipping;
	bool hasVecChangedDueToHiddenInverseSkipping;

	bool hasSizeOfTurnNumbersVecChangedDueToEqualNumbersSkipping;
	bool hasVecChangedDueToEqualNumberSkipping;

	bool accumulateSizeChange = hasSizeOfTurnNumbersVecChangedDueToOverflowHandling;

	do
	{
		hasSizeOfTurnNumbersVecChangedDueToHiddenInverseSkipping = false;
		hasVecChangedDueToHiddenInverseSkipping = false;
		Solver::skipHiddenInverses(vecOfTurnNumbers, useOnlyQuarterTurns, hasSizeOfTurnNumbersVecChangedDueToHiddenInverseSkipping, hasVecChangedDueToHiddenInverseSkipping);

		hasSizeOfTurnNumbersVecChangedDueToEqualNumbersSkipping = false;
		hasVecChangedDueToEqualNumberSkipping = false;
		Solver::skipEqualNumbers(vecOfTurnNumbers, useOnlyQuarterTurns, hasSizeOfTurnNumbersVecChangedDueToEqualNumbersSkipping, hasVecChangedDueToEqualNumberSkipping);

		if (hasSizeOfTurnNumbersVecChangedDueToHiddenInverseSkipping || hasSizeOfTurnNumbersVecChangedDueToEqualNumbersSkipping)
		{
			accumulateSizeChange = true;
		}

		//std::cout << hasVecChangedDueToHiddenInverseSkipping << hasSizeOfTurnNumbersVecChangedDueToEqualNumbersSkipping << std::endl;
	} while (hasVecChangedDueToHiddenInverseSkipping || hasVecChangedDueToEqualNumberSkipping);

	turnNumberVecSizeChanged = accumulateSizeChange;
}








void Solver::handleOverflowOfTurnNumbersVec(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged, bool& vecChanged)
{
	unsigned int const numberOfTurns = Turn::getNumberOfTurnTypes(useOnlyQuarterTurns);

	//Search leftmost invalid number
	bool allNumbersValid = true;
	unsigned int leftmostInvalidNumberPos;
	for (unsigned int i = 0; i < vecOfTurnNumbers.size(); ++i)
	{
		if (vecOfTurnNumbers.at(i) >= numberOfTurns)
		{
			leftmostInvalidNumberPos = i;
			allNumbersValid = false;
			break;
		}
	}
	if (allNumbersValid)
	{
		turnNumberVecSizeChanged = false;
		vecChanged = false;
		return;
	}
	else
	{
		vecChanged = true;
	}

	//Fix numbers step-by-step going to the left (Filling with zeros is ignored here! This will be done afterwards!)
	int actualNumber = leftmostInvalidNumberPos;
	while (vecOfTurnNumbers.at(actualNumber) >= numberOfTurns)
	{
		--actualNumber;
		if (actualNumber > -1) //If vector is big enough simply increase
		{
			++vecOfTurnNumbers.at(actualNumber);
		}
		else //If vector is to small exit the function early
		{
			turnNumberVecSizeChanged = true;
			vecOfTurnNumbers = std::vector<unsigned int>(vecOfTurnNumbers.size() + 1, 0u);
			return;
		}
	}

	//Filling with zeros (actualNumber points now to the leftmost entry to which 1 was added)
	++actualNumber;
	while (actualNumber < vecOfTurnNumbers.size())
	{
		vecOfTurnNumbers.at(actualNumber) = 0u;
		++actualNumber;
	}

	//Communicate the size change (If this function reaches this point, the vector was big enough and size has not changed!)
	turnNumberVecSizeChanged = false;
}





void Solver::skipHiddenInverses(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged, bool& vecChanged)
{
	//Declare Pattern Finding Function
	/*
	std::function<bool(std::vector<unsigned int> const &, std::vector<unsigned int> const &, unsigned int &)> findPatternInVector =
	[](std::vector<unsigned int> const & vecOfTurnNumbers, std::vector<unsigned int> const & patternVec, unsigned int & foundPosition) -> bool
	{
	//Handle special cases
	if (patternVec.empty() || vecOfTurnNumbers.empty())
	{
	return false;
	}
	if (patternVec.size() > vecOfTurnNumbers.size())
	{
	return false;
	}
	if (patternVec.size() == vecOfTurnNumbers.size())
	{
	foundPosition = 0u;
	return (patternVec == vecOfTurnNumbers);
	}

	//Find maximal offset for comparison
	unsigned int const maximalOffset = vecOfTurnNumbers.size() - patternVec.size();

	//Compare vectors
	for (unsigned int offset = 0u; offset <= maximalOffset; ++offset)
	{
	for (unsigned int i = 0u; i < patternVec.size(); ++i)
	{
	if (vecOfTurnNumbers.at(offset + i) != patternVec.at(i))
	{
	break;
	}
	else if (i == patternVec.size() - 1)
	{
	foundPosition = offset;
	return true;
	}
	}
	}

	//Return false if nothing was found
	return false;
	};
	*/

	unsigned int const numberOfTurns = Turn::getNumberOfTurnTypes(useOnlyQuarterTurns);

	//Skip Inverses
	bool accumulateOverflowChangedVecSize = false;
	bool foundHiddenInverse;
	bool accumulateVecChanged = false;
	do
	{
		foundHiddenInverse = false;
		for (unsigned int i = 0; i < vecOfTurnNumbers.size() - 1; ++i)
		{
			if (Turn::getTurnTypeFromNumberInArray(vecOfTurnNumbers.at(i), useOnlyQuarterTurns) == !Turn::getTurnTypeFromNumberInArray(vecOfTurnNumbers.at(i + 1), useOnlyQuarterTurns))
			{
				vecOfTurnNumbers.at(i + 1) += 1;
				Solver::fillVectorWithZerosFrom(vecOfTurnNumbers, i + 2);
				bool overflowChangedVecSize;
				bool overflowChangedVecPlaceholder;
				handleOverflowOfTurnNumbersVec(vecOfTurnNumbers, useOnlyQuarterTurns, overflowChangedVecSize, overflowChangedVecPlaceholder);
				accumulateOverflowChangedVecSize = accumulateOverflowChangedVecSize || overflowChangedVecSize;
				foundHiddenInverse = true;
				accumulateVecChanged = true;
				break;
			}
		}
	} while (foundHiddenInverse);
	turnNumberVecSizeChanged = accumulateOverflowChangedVecSize;
	vecChanged = accumulateVecChanged;
}






void Solver::skipEqualNumbers(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged, bool& vecChanged)
{
	unsigned int const numberOfTurns = Turn::getNumberOfTurnTypes(useOnlyQuarterTurns);

	if (useOnlyQuarterTurns)
	{
		///////////////////////////
		//Only Quarter Turns Branch
		bool accumulateOverflowChangedVecSize = false;
		bool foundHiddenInverse;
		bool accumulateVecChanged = false;
		do
		{
			foundHiddenInverse = false;
			for (unsigned int i = 0; i + 2 < vecOfTurnNumbers.size(); ++i)
			{
				TurnType turnType1 = Turn::getTurnTypeFromNumberInArray(vecOfTurnNumbers.at(i), true);
				TurnType turnType2 = Turn::getTurnTypeFromNumberInArray(vecOfTurnNumbers.at(i + 1), true);
				TurnType turnType3 = Turn::getTurnTypeFromNumberInArray(vecOfTurnNumbers.at(i + 2), true);
				if ((turnType1 == turnType2) && (turnType2 == turnType3))
				{
					vecOfTurnNumbers.at(i + 2) += 1;
					Solver::fillVectorWithZerosFrom(vecOfTurnNumbers, i + 3);
					bool overflowChangedVecSize;
					bool overflowChangedVecPlaceholder;
					handleOverflowOfTurnNumbersVec(vecOfTurnNumbers, useOnlyQuarterTurns, overflowChangedVecSize, overflowChangedVecPlaceholder);
					accumulateOverflowChangedVecSize = accumulateOverflowChangedVecSize || overflowChangedVecSize;
					foundHiddenInverse = true;
					accumulateVecChanged = true;
					break;
				}
			}
		} while (foundHiddenInverse);
		vecChanged = accumulateVecChanged;
		turnNumberVecSizeChanged = accumulateOverflowChangedVecSize;
	}
	else
	{
		//////////////////
		//All Turns Branch
		bool accumulateOverflowChangedVecSize = false;
		bool foundHiddenInverse;
		bool accumulateVecChanged = false;
		do
		{
			foundHiddenInverse = false;
			for (unsigned int i = 0; i + 1 < vecOfTurnNumbers.size(); ++i)
			{
				if (doTurnTypesActOnSamePlane(Turn::getTurnTypeFromNumberInArray(vecOfTurnNumbers.at(i), false), Turn::getTurnTypeFromNumberInArray(vecOfTurnNumbers.at(i + 1), false)))
				{
					vecOfTurnNumbers.at(i + 1) += 1;
					Solver::fillVectorWithZerosFrom(vecOfTurnNumbers, i + 2);
					bool overflowChangedVecSize;
					bool overflowChangedVecPlaceholder;
					handleOverflowOfTurnNumbersVec(vecOfTurnNumbers, useOnlyQuarterTurns, overflowChangedVecSize, overflowChangedVecPlaceholder);
					accumulateOverflowChangedVecSize = accumulateOverflowChangedVecSize || overflowChangedVecSize;
					foundHiddenInverse = true;
					accumulateVecChanged = true;
					break;
				}
			}
		} while (foundHiddenInverse);
		vecChanged = accumulateVecChanged;
		turnNumberVecSizeChanged = accumulateOverflowChangedVecSize;
	}
}







void Solver::fillVectorWithZerosFrom(std::vector<unsigned int>& vec, unsigned int index)
{
	if (index >= vec.size())
	{
		return;
	}
	else
	{
		for (unsigned int i = index; i < vec.size(); ++i)
		{
			vec.at(i) = 0u;
		}
	}
}







TurnTypeOrder Solver::trafoVecOfTurnNumInTurnTypeOrder(std::vector<unsigned int> const & vec, bool useOnlyQuarterTurns)
{
	TurnTypeOrder turnTypeOrder;
	for (auto const & num : vec)
	{
		turnTypeOrder.push_back(Turn::getTurnTypeFromNumberInArray(num, useOnlyQuarterTurns));
	}
	return std::move(turnTypeOrder);
}













Solver::Comparator::Comparator(std::function<bool(Cube const &, Cube const &)> compFunc)
	: compareFunction(compFunc)
{
}


bool Solver::Comparator::operator()(Cube const & cube1, Cube const & cube2) const
{
	return this->compareFunction(cube1, cube2);
}


Solver::Comparator operator&&(Solver::Comparator const & c1, Solver::Comparator const & c2)
{
	return [c1, c2](Cube const & cube1, Cube const & cube2) -> bool
	{
		return (c1(cube1, cube2) && c2(cube1, cube2));
	};
}












const Solver::Comparator Solver::Comparator::FullCube = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	return (c1 == c2);
}
);


const Solver::Comparator Solver::Comparator::PureWhiteCross = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	Plane const & upPlane1 = c1.at(PlanePos::UP);
	Plane const & upPlane2 = c2.at(PlanePos::UP);
	for (unsigned int i = 1; i < 8; i += 2)
	{
		if (upPlane1.at(i) != upPlane2.at(i))
		{
			return false;
		}
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullWhiteCross = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	Plane const & upPlane1 = c1.at(PlanePos::UP);
	Plane const & upPlane2 = c2.at(PlanePos::UP);
	for (unsigned int i = 1; i < 8; i += 2)
	{
		if (upPlane1.at(i) != upPlane2.at(i))
		{
			return false;
		}
	}
	if (c1.at(PlanePos::FRONT).at(1) != c2.at(PlanePos::FRONT).at(1))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(1) != c2.at(PlanePos::RIGHT).at(1))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(1) != c2.at(PlanePos::LEFT).at(1))
	{
		return false;
	}
	if (c1.at(PlanePos::BACK).at(1) != c2.at(PlanePos::BACK).at(1))
	{
		return false;
	}
	return true;
}
);



const Solver::Comparator Solver::Comparator::FullCorner::UpFrontRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::FRONT).at(2) != c2.at(PlanePos::FRONT).at(2))
	{
		return false;
	}
	if (c1.at(PlanePos::UP).at(4) != c2.at(PlanePos::UP).at(4))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(0) != c2.at(PlanePos::RIGHT).at(0))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullCorner::UpFrontLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::FRONT).at(0) != c2.at(PlanePos::FRONT).at(0))
	{
		return false;
	}
	if (c1.at(PlanePos::UP).at(6) != c2.at(PlanePos::UP).at(6))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(2) != c2.at(PlanePos::LEFT).at(2))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullCorner::UpBackRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::BACK).at(0) != c2.at(PlanePos::BACK).at(0))
	{
		return false;
	}
	if (c1.at(PlanePos::UP).at(2) != c2.at(PlanePos::UP).at(2))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(2) != c2.at(PlanePos::RIGHT).at(2))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullCorner::UpBackLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::BACK).at(2) != c2.at(PlanePos::BACK).at(2))
	{
		return false;
	}
	if (c1.at(PlanePos::UP).at(0) != c2.at(PlanePos::UP).at(0))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(0) != c2.at(PlanePos::LEFT).at(0))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullCorner::DownFrontRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::FRONT).at(4) != c2.at(PlanePos::FRONT).at(4))
	{
		return false;
	}
	if (c1.at(PlanePos::DOWN).at(2) != c2.at(PlanePos::DOWN).at(2))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(6) != c2.at(PlanePos::RIGHT).at(6))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullCorner::DownFrontLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::FRONT).at(6) != c2.at(PlanePos::FRONT).at(6))
	{
		return false;
	}
	if (c1.at(PlanePos::DOWN).at(0) != c2.at(PlanePos::DOWN).at(0))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(4) != c2.at(PlanePos::LEFT).at(4))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullCorner::DownBackRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::BACK).at(6) != c2.at(PlanePos::BACK).at(6))
	{
		return false;
	}
	if (c1.at(PlanePos::DOWN).at(4) != c2.at(PlanePos::DOWN).at(4))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(4) != c2.at(PlanePos::RIGHT).at(4))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::FullCorner::DownBackLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::BACK).at(4) != c2.at(PlanePos::BACK).at(4))
	{
		return false;
	}
	if (c1.at(PlanePos::DOWN).at(6) != c2.at(PlanePos::DOWN).at(6))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(6) != c2.at(PlanePos::LEFT).at(6))
	{
		return false;
	}
	return true;
}
);






const Solver::Comparator Solver::Comparator::PureCorner::UpFrontRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(4) != c2.at(PlanePos::UP).at(4))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::PureCorner::UpFrontLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(6) != c2.at(PlanePos::UP).at(6))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::PureCorner::UpBackRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(2) != c2.at(PlanePos::UP).at(2))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::PureCorner::UpBackLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(0) != c2.at(PlanePos::UP).at(0))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::PureCorner::DownFrontRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(2) != c2.at(PlanePos::DOWN).at(2))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::PureCorner::DownFrontLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(0) != c2.at(PlanePos::DOWN).at(0))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::PureCorner::DownBackRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(4) != c2.at(PlanePos::DOWN).at(4))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::PureCorner::DownBackLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(6) != c2.at(PlanePos::DOWN).at(6))
	{
		return false;
	}
	return true;
}
);









const Solver::Comparator Solver::Comparator::Edge::UpFront = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(5) != c2.at(PlanePos::UP).at(5))
	{
		return false;
	}
	if (c1.at(PlanePos::FRONT).at(1) != c2.at(PlanePos::FRONT).at(1))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::UpBack = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(1) != c2.at(PlanePos::UP).at(1))
	{
		return false;
	}
	if (c1.at(PlanePos::BACK).at(1) != c2.at(PlanePos::BACK).at(1))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::UpRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(3) != c2.at(PlanePos::UP).at(3))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(1) != c2.at(PlanePos::RIGHT).at(1))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::UpLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::UP).at(7) != c2.at(PlanePos::UP).at(7))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(1) != c2.at(PlanePos::LEFT).at(1))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::DownFront = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(1) != c2.at(PlanePos::DOWN).at(1))
	{
		return false;
	}
	if (c1.at(PlanePos::FRONT).at(5) != c2.at(PlanePos::FRONT).at(5))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::DownBack = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(5) != c2.at(PlanePos::DOWN).at(5))
	{
		return false;
	}
	if (c1.at(PlanePos::BACK).at(5) != c2.at(PlanePos::BACK).at(5))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::DownRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(3) != c2.at(PlanePos::DOWN).at(3))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(5) != c2.at(PlanePos::RIGHT).at(5))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::DownLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::DOWN).at(7) != c2.at(PlanePos::DOWN).at(7))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(5) != c2.at(PlanePos::LEFT).at(5))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::FrontRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::FRONT).at(3) != c2.at(PlanePos::FRONT).at(3))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(7) != c2.at(PlanePos::RIGHT).at(7))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::FrontLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::FRONT).at(7) != c2.at(PlanePos::FRONT).at(7))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(3) != c2.at(PlanePos::LEFT).at(3))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::BackRight = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::BACK).at(7) != c2.at(PlanePos::BACK).at(7))
	{
		return false;
	}
	if (c1.at(PlanePos::RIGHT).at(3) != c2.at(PlanePos::RIGHT).at(3))
	{
		return false;
	}
	return true;
}
);


const Solver::Comparator Solver::Comparator::Edge::BackLeft = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	if (c1.at(PlanePos::BACK).at(3) != c2.at(PlanePos::BACK).at(3))
	{
		return false;
	}
	if (c1.at(PlanePos::LEFT).at(7) != c2.at(PlanePos::LEFT).at(7))
	{
		return false;
	}
	return true;
}
);







const Solver::Comparator Solver::Comparator::Pair::FrontRight = (Solver::Comparator::FullCorner::UpFrontRight && Solver::Comparator::Edge::FrontRight);
const Solver::Comparator Solver::Comparator::Pair::FrontLeft = (Solver::Comparator::FullCorner::UpFrontLeft && Solver::Comparator::Edge::FrontLeft);
const Solver::Comparator Solver::Comparator::Pair::BackRight = (Solver::Comparator::FullCorner::UpBackRight && Solver::Comparator::Edge::BackRight);
const Solver::Comparator Solver::Comparator::Pair::BackLeft = (Solver::Comparator::FullCorner::UpBackLeft && Solver::Comparator::Edge::BackLeft);





const Solver::Comparator Solver::Comparator::YellowCross = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	for (unsigned int i = 1; i < 8; i += 2)
	{
		if (c1.at(PlanePos::DOWN).at(i) != c2.at(PlanePos::DOWN).at(i))
		{
			return false;
		}
	}
	return true;
}
);



const Solver::Comparator Solver::Comparator::YellowPlane = Solver::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	for (unsigned int i = 0; i < 8; ++i)
	{
		if (c1.at(PlanePos::DOWN).at(i) != c2.at(PlanePos::DOWN).at(i))
		{
			return false;
		}
	}
	return true;
}
);




const Solver::Comparator Solver::Comparator::FirstLayer = (
	Solver::Comparator::FullWhiteCross && Solver::Comparator::FullCorner::UpFrontRight && Solver::Comparator::FullCorner::UpFrontLeft && Solver::Comparator::FullCorner::UpBackRight && Solver::Comparator::FullCorner::UpBackLeft
	);




const Solver::Comparator Solver::Comparator::SecondLayer = (
	Solver::Comparator::FirstLayer && Solver::Comparator::Edge::FrontRight && Solver::Comparator::Edge::FrontLeft && Solver::Comparator::Edge::BackRight && Solver::Comparator::Edge::BackLeft
	);




const Solver::Comparator Solver::Comparator::FullCubeComposed =
(
	Solver::Comparator::Edge::UpLeft &&
	Solver::Comparator::Edge::UpRight &&
	Solver::Comparator::Edge::UpFront &&
	Solver::Comparator::Edge::UpBack &&
	Solver::Comparator::Edge::DownLeft  &&
	Solver::Comparator::Edge::DownRight &&
	Solver::Comparator::Edge::DownFront &&
	Solver::Comparator::Edge::DownBack  &&
	Solver::Comparator::Edge::FrontLeft &&
	Solver::Comparator::Edge::FrontRight &&
	Solver::Comparator::Edge::BackLeft &&
	Solver::Comparator::Edge::BackRight &&
	Solver::Comparator::FullCorner::UpFrontLeft &&
	Solver::Comparator::FullCorner::UpFrontRight &&
	Solver::Comparator::FullCorner::UpBackLeft &&
	Solver::Comparator::FullCorner::UpBackRight &&
	Solver::Comparator::FullCorner::DownFrontLeft &&
	Solver::Comparator::FullCorner::DownFrontRight &&
	Solver::Comparator::FullCorner::DownBackLeft &&
	Solver::Comparator::FullCorner::DownBackRight
	);





std::vector<TurnType> Solver::getExtendingTurnTypesFor(TurnTypeOrder const & turnTypeOrder, bool useOnlyQuarterTurns)
{
	std::vector<TurnType> extendingTurnTypes;

	for (unsigned int i = 0; i < Turn::getNumberOfTurnTypes(useOnlyQuarterTurns); ++i)
	{
		TurnType newTurnType = Turn::getTurnTypeFromNumberInArray(i, useOnlyQuarterTurns);
		if (true || turnTypeOrder.empty())
		{
			extendingTurnTypes.push_back(newTurnType);
		}
	}

	return std::move(extendingTurnTypes);
}




