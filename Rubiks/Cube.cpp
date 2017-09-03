#include "Cube.hpp"



Cube::Cube()
	: Cube({
	Plane(Cube::getCorrectColorOfPlane(PlanePos::UP)),
	Plane(Cube::getCorrectColorOfPlane(PlanePos::DOWN)),
	Plane(Cube::getCorrectColorOfPlane(PlanePos::FRONT)),
	Plane(Cube::getCorrectColorOfPlane(PlanePos::BACK)),
	Plane(Cube::getCorrectColorOfPlane(PlanePos::RIGHT)),
	Plane(Cube::getCorrectColorOfPlane(PlanePos::LEFT))
})
{
}



Cube::Cube(std::array<Plane, 6> const & arrayOfPlanes)
	: mUpPlane(arrayOfPlanes.at(0)),
	  mDownPlane(arrayOfPlanes.at(1)),
	  mFrontPlane(arrayOfPlanes.at(2)),
	  mBackPlane(arrayOfPlanes.at(3)),
	  mRightPlane(arrayOfPlanes.at(4)),
	  mLeftPlane(arrayOfPlanes.at(5))
{
}



Plane& Cube::at(PlanePos planePos)
{
	switch (planePos)
	{
	case PlanePos::UP:
		return mUpPlane;
	case PlanePos::DOWN:
		return mDownPlane;
	case PlanePos::FRONT:
		return mFrontPlane;
	case PlanePos::BACK:
		return mBackPlane;
	case PlanePos::RIGHT:
		return mRightPlane;
	case PlanePos::LEFT:
		return mLeftPlane;
	}
}


Plane const & Cube::at(PlanePos planePos) const
{
	switch (planePos)
	{
	case PlanePos::UP:
		return mUpPlane;
	case PlanePos::DOWN:
		return mDownPlane;
	case PlanePos::FRONT:
		return mFrontPlane;
	case PlanePos::BACK:
		return mBackPlane;
	case PlanePos::RIGHT:
		return mRightPlane;
	case PlanePos::LEFT:
		return mLeftPlane;
	}
}


void Cube::turn(Turn const & turn)
{
	turn.mTurnFunction(*this);
}


void Cube::turn(TurnType turnType)
{
	this->turn(Turn::getTurn(turnType));
}

void Cube::turn(TurnOrder const & turnOrder)
{
	for (auto turn : turnOrder.mTurns)
	{
		this->turn(turn);
	}
}


void Cube::solve(SolveTurns solveTurns, Cube::Comparator comparator) const
{
	bool useOnlyQuarterTurns;
	switch (solveTurns)
	{
	case SolveTurns::ALL:
		useOnlyQuarterTurns = false;
		break;
	case SolveTurns::QUARTER_TURNS:
		useOnlyQuarterTurns = true;
		break;
	}

	std::function<void(std::vector<unsigned int>&, unsigned int)> createNextVectorOfTurnNumbers = [](std::vector<unsigned int>& vecOfTurnNumbers, unsigned int numberOfTurns)
	{
		if (vecOfTurnNumbers.empty())
		{
			vecOfTurnNumbers = std::vector<unsigned int>({ 0u });
			std::cout << "Check 1 Turn..." << std::endl;
			return;
		}
		vecOfTurnNumbers.at(0) += 1;
		unsigned int actualNumber = 0u;
		while (true)
		{
			if (vecOfTurnNumbers.at(actualNumber) == numberOfTurns)
			{
				//Increase next Turn Number and check if vec is great enough
				vecOfTurnNumbers.at(actualNumber) = 0u;
				actualNumber += 1;
				if (actualNumber < vecOfTurnNumbers.size())
				{
					vecOfTurnNumbers.at(actualNumber) += 1;
				}
				else
				{
					vecOfTurnNumbers = std::vector<unsigned int>(vecOfTurnNumbers.size() + 1, 0u);
					vecOfTurnNumbers.back() = 0u;
					std::cout << "Check " << vecOfTurnNumbers.size() << " Turns..." << std::endl;
				}
			}
			else
			{
				break;
			}
		}
	};

	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	Cube const solvedCube;
	Cube newCube(*this);
	if (comparator(newCube, solvedCube))
	{
		std::cout << "Cube is already solved!" << std::endl;
	}
	unsigned int const size = (useOnlyQuarterTurns ? Turn::arrayOfQuarterTurnTypes.size() : Turn::arrayOfAllTurnTypes.size());
	std::vector<unsigned int> vectorOfTurnNumbers;
	while (true)
	{
		createNextVectorOfTurnNumbers(vectorOfTurnNumbers, size);
		for (std::vector<unsigned int>::reverse_iterator it = vectorOfTurnNumbers.rbegin(); it != vectorOfTurnNumbers.rend(); ++it)
		{
			TurnType turnType = (useOnlyQuarterTurns ? Turn::arrayOfQuarterTurnTypes.at(*it) : Turn::arrayOfAllTurnTypes.at(*it));
			newCube.turn(turnType);
		}
		if (comparator(newCube, solvedCube))
		{
			std::cout << "Solved! Turns: ";
			for (std::vector<unsigned int>::reverse_iterator it = vectorOfTurnNumbers.rbegin(); it != vectorOfTurnNumbers.rend(); ++it)
			{
				TurnType turnType = (useOnlyQuarterTurns ? Turn::arrayOfQuarterTurnTypes.at(*it) : Turn::arrayOfAllTurnTypes.at(*it));
				std::cout << Turn::getTurnTypeString(turnType);
			}
			std::chrono::steady_clock::time_point solvedTime = std::chrono::steady_clock::now();
			std::cout << " (Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(solvedTime - startTime).count() << " ms)";
			std::cout << std::endl;
		}
		newCube = *this;
	}
}


bool Cube::countColors() const
{
	std::map<Color, unsigned int> colorCounts
	{
		std::make_pair(Color::WHITE, 0u),
		std::make_pair(Color::YELLOW, 0u),
		std::make_pair(Color::RED, 0u),
		std::make_pair(Color::ORANGE, 0u),
		std::make_pair(Color::BLUE, 0u),
		std::make_pair(Color::GREEN, 0u)
	};

	std::function<void(std::map<Color, unsigned int>&, Plane const &)> planeCountingFunction = [](std::map<Color, unsigned int>& colorCounts, Plane const & plane)
	{
		for (unsigned int i = 0; i < 8; ++i)
		{
			colorCounts.at(plane.at(i)) += 1;
		}
	};

	planeCountingFunction(colorCounts, this->mUpPlane);
	planeCountingFunction(colorCounts, this->mDownPlane);
	planeCountingFunction(colorCounts, this->mFrontPlane);
	planeCountingFunction(colorCounts, this->mBackPlane);
	planeCountingFunction(colorCounts, this->mRightPlane);
	planeCountingFunction(colorCounts, this->mLeftPlane);

	std::cout << "Number of white stickers: " << colorCounts.at(Color::WHITE) << std::endl;
	std::cout << "Number of yellow stickers: " << colorCounts.at(Color::YELLOW) << std::endl;
	std::cout << "Number of red stickers: " << colorCounts.at(Color::RED) << std::endl;
	std::cout << "Number of orange stickers: " << colorCounts.at(Color::ORANGE) << std::endl;
	std::cout << "Number of blue stickers: " << colorCounts.at(Color::BLUE) << std::endl;
	std::cout << "Number of green stickers: " << colorCounts.at(Color::GREEN) << std::endl;

	bool valid = true;
	for (auto num : colorCounts)
	{
		valid = valid && (num.second == 8u);
	}
	return valid;
}


Color Cube::getCorrectColorOfPlane(PlanePos planePos)
{
	switch (planePos)
	{
	case PlanePos::UP:
		return Color::WHITE;
	case PlanePos::DOWN:
		return Color::YELLOW;
	case PlanePos::FRONT:
		return Color::RED;
	case PlanePos::BACK:
		return Color::ORANGE;
	case PlanePos::RIGHT:
		return Color::BLUE;
	case PlanePos::LEFT:
		return Color::GREEN;
	}
}


const std::array<Color, 6> Cube::arrayOfCorrectColorsPerPlane = std::array<Color, 6>({
	Color::WHITE,
	Color::YELLOW,
	Color::RED,
	Color::ORANGE,
	Color::BLUE,
	Color::GREEN
});



Cube::TerminalOutputOptions Cube::terminalOutputOptions = Cube::TerminalOutputOptions::OUTPUT_AS_CUBE_NET;



std::ostream& operator<<(std::ostream& oStream, Cube const & cube)
{
	oStream << "-----Cube-----" << std::endl;

	switch (Cube::terminalOutputOptions)
	{
	case Cube::TerminalOutputOptions::OUTPUT_AS_UDFBRL:
		cube.writeUDFBRLTerminalOutput(oStream);
		break;
	case Cube::TerminalOutputOptions::OUTPUT_AS_CUBE_NET:
		cube.writeCubeNetTerminalOutput(oStream);
		break;
	}

	oStream << "--------------" << std::endl;
	return oStream;
}


bool operator==(Cube const & cube1, Cube const & cube2)
{
	if (!(cube1.mUpPlane == cube2.mUpPlane))
	{
		return false;
	}
	if (!(cube1.mDownPlane == cube2.mDownPlane))
	{
		return false;
	}
	if (!(cube1.mFrontPlane == cube2.mFrontPlane))
	{
		return false;
	}
	if (!(cube1.mBackPlane == cube2.mBackPlane))
	{
		return false;
	}
	if (!(cube1.mRightPlane == cube2.mRightPlane))
	{
		return false;
	}
	if (!(cube1.mLeftPlane == cube2.mLeftPlane))
	{
		return false;
	}
	return true;
}



void Cube::writeUDFBRLTerminalOutput(std::ostream& oStream) const
{
	Cube const & cube = *this;

	oStream << cube.mUpPlane.at(0) << cube.mUpPlane.at(1) << cube.mUpPlane.at(2) << std::endl;
	oStream << cube.mUpPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::UP) << cube.mUpPlane.at(3) << std::endl;
	oStream << cube.mUpPlane.at(6) << cube.mUpPlane.at(5) << cube.mUpPlane.at(4) << std::endl;
	oStream << std::endl;

	oStream << cube.mDownPlane.at(0) << cube.mDownPlane.at(1) << cube.mDownPlane.at(2) << std::endl;
	oStream << cube.mDownPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::DOWN) << cube.mDownPlane.at(3) << std::endl;
	oStream << cube.mDownPlane.at(6) << cube.mDownPlane.at(5) << cube.mDownPlane.at(4) << std::endl;
	oStream << std::endl;

	oStream << cube.mFrontPlane.at(0) << cube.mFrontPlane.at(1) << cube.mFrontPlane.at(2) << std::endl;
	oStream << cube.mFrontPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::FRONT) << cube.mFrontPlane.at(3) << std::endl;
	oStream << cube.mFrontPlane.at(6) << cube.mFrontPlane.at(5) << cube.mFrontPlane.at(4) << std::endl;
	oStream << std::endl;

	oStream << cube.mBackPlane.at(0) << cube.mBackPlane.at(1) << cube.mBackPlane.at(2) << std::endl;
	oStream << cube.mBackPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::BACK) << cube.mBackPlane.at(3) << std::endl;
	oStream << cube.mBackPlane.at(6) << cube.mBackPlane.at(5) << cube.mBackPlane.at(4) << std::endl;
	oStream << std::endl;

	oStream << cube.mRightPlane.at(0) << cube.mRightPlane.at(1) << cube.mRightPlane.at(2) << std::endl;
	oStream << cube.mRightPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::RIGHT) << cube.mRightPlane.at(3) << std::endl;
	oStream << cube.mRightPlane.at(6) << cube.mRightPlane.at(5) << cube.mRightPlane.at(4) << std::endl;
	oStream << std::endl;

	oStream << cube.mLeftPlane.at(0) << cube.mLeftPlane.at(1) << cube.mLeftPlane.at(2) << std::endl;
	oStream << cube.mLeftPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::LEFT) << cube.mLeftPlane.at(3) << std::endl;
	oStream << cube.mLeftPlane.at(6) << cube.mLeftPlane.at(5) << cube.mLeftPlane.at(4) << std::endl;
	oStream << std::endl;
}


void Cube::writeCubeNetTerminalOutput(std::ostream& oStream) const
{
	Cube const & cube = *this;

	oStream << "    " << cube.mUpPlane.at(0) << cube.mUpPlane.at(1) << cube.mUpPlane.at(2) << std::endl;
	oStream << "    " << cube.mUpPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::UP) << cube.mUpPlane.at(3) << std::endl;
	oStream << "    " << cube.mUpPlane.at(6) << cube.mUpPlane.at(5) << cube.mUpPlane.at(4) << std::endl;
	oStream << std::endl;

	oStream << cube.mLeftPlane.at(0) << cube.mLeftPlane.at(1) << cube.mLeftPlane.at(2) << " " << cube.mFrontPlane.at(0) << cube.mFrontPlane.at(1) << cube.mFrontPlane.at(2) << " " << cube.mRightPlane.at(0) << cube.mRightPlane.at(1) << cube.mRightPlane.at(2) << " " << cube.mBackPlane.at(0) << cube.mBackPlane.at(1) << cube.mBackPlane.at(2) << std::endl;
	oStream << cube.mLeftPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::LEFT) << cube.mLeftPlane.at(3) << " " << cube.mFrontPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::FRONT) << cube.mFrontPlane.at(3) << " " << cube.mRightPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::RIGHT) << cube.mRightPlane.at(3) << " " << cube.mBackPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::BACK) << cube.mBackPlane.at(3) << std::endl;
	oStream << cube.mLeftPlane.at(6) << cube.mLeftPlane.at(5) << cube.mLeftPlane.at(4) << " " << cube.mFrontPlane.at(6) << cube.mFrontPlane.at(5) << cube.mFrontPlane.at(4) << " " << cube.mRightPlane.at(6) << cube.mRightPlane.at(5) << cube.mRightPlane.at(4) << " " << cube.mBackPlane.at(6) << cube.mBackPlane.at(5) << cube.mBackPlane.at(4) << std::endl;
	oStream << std::endl;

	oStream << "    " << cube.mDownPlane.at(0) << cube.mDownPlane.at(1) << cube.mDownPlane.at(2) << std::endl;
	oStream << "    " << cube.mDownPlane.at(7) << Cube::getCorrectColorOfPlane(PlanePos::DOWN) << cube.mDownPlane.at(3) << std::endl;
	oStream << "    " << cube.mDownPlane.at(6) << cube.mDownPlane.at(5) << cube.mDownPlane.at(4) << std::endl;
}




void Cube::askForConsoleInput()
{
	std::function<Plane(std::string const &)> planeLoadingFunction = [](std::string const & planeName) -> Plane
	{
		std::function<std::array<Color, 3>()> colorRowLodingFunction = []()->std::array<Color, 3>
		{
			std::array<Color, 3> array;
			std::string line;
			std::getline(std::cin, line);
			if (line.size() != 3)
			{
				throw "Type in 3 chars!";
			}
			for (unsigned int i = 0; i < 3; ++i)
			{
				char letter = line.at(i);
				if ((letter != 'w') && (letter != 'y') && (letter != 'r') && (letter != 'o') && (letter != 'b') && (letter != 'g'))
				{
					throw "Use only w, y, r, o, b and g!";
				}
				array.at(i) = getCharColor(letter);
			}
			return array;
		};
		
		
		std::cout << planeName << std::endl;
		std::array<Color, 8> fullArray;
		std::array<Color, 3> array(std::move(colorRowLodingFunction()));
		for (unsigned int i = 0; i < 3; ++i)
		{
			fullArray.at(i) = array.at(i);
		}
		array = std::move(colorRowLodingFunction());
		fullArray.at(3) = array.at(0);
		fullArray.at(4) = array.at(2);
		array = std::move(colorRowLodingFunction());
		for (unsigned int i = 5; i < 8; ++i)
		{
			fullArray.at(i) = array.at(i - 5);
		}
		return Plane(fullArray, Plane::ArrayToPlaneMode::ROWWISE);
	};

	try
	{
		std::cout << "Input cube:" << std::endl;
		std::array<Plane, 6> planeArray
		{
			planeLoadingFunction("Up:"),
			planeLoadingFunction("Down:"),
			planeLoadingFunction("Front:"),
			planeLoadingFunction("Back:"),
			planeLoadingFunction("Right:"),
			planeLoadingFunction("Left:")
		};
		*this = Cube(planeArray);
		if (!this->countColors())
		{
			throw "Invalid numbers of colors!";
		}
	}
	catch (char* message)
	{
		std::cout << message << std::endl;
		std::cout << "Cube input canceled!" << std::endl;
		this->askForConsoleInput();
	}
}









Cube::Comparator::Comparator(std::function<bool(Cube const &, Cube const &)> compFunc)
	: compareFunction(compFunc)
{
}


bool Cube::Comparator::operator()(Cube const & cube1, Cube const & cube2) const
{
	return this->compareFunction(cube1, cube2);
}


Cube::Comparator operator&&(Cube::Comparator const & c1, Cube::Comparator const & c2)
{
	return [c1, c2](Cube const & cube1, Cube const & cube2) -> bool
	{
		return (c1(cube1, cube2) && c2(cube1, cube2));
	};
}












const Cube::Comparator Cube::Comparator::FullCube = Cube::Comparator(
	[](Cube const & c1, Cube const & c2) -> bool
{
	return (c1 == c2);
}
);


const Cube::Comparator Cube::Comparator::PureWhiteCross = Cube::Comparator(
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


const Cube::Comparator Cube::Comparator::FullWhiteCross = Cube::Comparator(
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



const Cube::Comparator Cube::Comparator::FullCorner::UpFrontRight = Cube::Comparator(
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


const Cube::Comparator Cube::Comparator::FullCorner::UpFrontLeft = Cube::Comparator(
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


const Cube::Comparator Cube::Comparator::FullCorner::UpBackRight = Cube::Comparator(
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

//Incorrect!
const Cube::Comparator Cube::Comparator::FullCorner::UpBackLeft = Cube::Comparator(
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


const Cube::Comparator Cube::Comparator::FullCorner::DownFrontRight = Cube::Comparator(
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


const Cube::Comparator Cube::Comparator::FullCorner::DownFrontLeft = Cube::Comparator(
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


const Cube::Comparator Cube::Comparator::FullCorner::DownBackRight = Cube::Comparator(
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


const Cube::Comparator Cube::Comparator::FullCorner::DownBackLeft = Cube::Comparator(
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


