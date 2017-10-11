#include "Cube.hpp"
#include "Turn.hpp"
#include "Solver.hpp"



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


Color& Cube::at(StickerPos const & stickerPos)
{
	return this->at(stickerPos.first).at(stickerPos.second);
}

Color const & Cube::at(StickerPos const & stickerPos) const
{
	return this->at(stickerPos.first).at(stickerPos.second);
}

Color& Cube::at(PlanePos planePos, unsigned int stickerNumber)
{
	return this->at(planePos).at(stickerNumber);
}

Color const & Cube::at(PlanePos planePos, unsigned int stickerNumber) const
{
	return this->at(planePos).at(stickerNumber);
}



void Cube::turn(Turn const & turn)
{
	turn.mTurnFunction(*this);
}


void Cube::turn(TurnType turnType)
{
	this->turn(Turn::getTurn(turnType));
}

void Cube::turn(TurnTypeOrder const & turnTypeOrder)
{
	for (auto const & turn : turnTypeOrder)
	{
		this->turn(turn);
	}
}

void Cube::turn(TurnOrder const & turnOrder)
{
	for (auto turn : turnOrder.mTurns)
	{
		this->turn(turn);
	}
}


bool Cube::numberOfAllColorsIs8(std::map<Color, unsigned int> & colorNumbers) const
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

	std::function<void(Plane const &)> planeCountingFunction = [&colorCounts](Plane const & plane)
	{
		for (unsigned int i = 0; i < 8; ++i)
		{
			colorCounts.at(plane.at(i)) += 1;
		}
	};

	planeCountingFunction(this->mUpPlane);
	planeCountingFunction(this->mDownPlane);
	planeCountingFunction(this->mFrontPlane);
	planeCountingFunction(this->mBackPlane);
	planeCountingFunction(this->mRightPlane);
	planeCountingFunction(this->mLeftPlane);

	bool valid = true;
	for (auto num : colorCounts)
	{
		valid = valid && (num.second == 8u);
	}
	colorNumbers = colorCounts;
	return valid;
}


bool Cube::isValid(InvalidCubeException & returnExceptionIfNotValid) const
{
	try
	{
		Solver::quicksolve(*this, false);
	}
	catch (Cube::InvalidCubeException exception)
	{
		returnExceptionIfNotValid = exception;
		return false;
	}
	return true;
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
	class QuitInputException{};

	std::function<Plane(std::string const &)> planeLoadingFunction = [](std::string const & planeName) -> Plane
	{
		std::function<std::array<Color, 3>()> colorRowLodingFunction = []() -> std::array<Color, 3>
		{
			std::array<Color, 3> array;
			std::string line;
			std::getline(std::cin, line);
			if (line.size() != 3)
			{
				if (line == "quit")
				{
					throw QuitInputException();
				}
				else
				{
					throw "Type in 3 chars!";
				}
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
		Cube inputCube(planeArray);
		InvalidCubeException invalidCubeException;
		if (!inputCube.isValid(invalidCubeException))
		{
			throw invalidCubeException;
		}
		*this = inputCube;
	}
	catch (char* message)
	{
		std::cout << message << std::endl;
		std::cout << "Try again!" << std::endl;
		this->askForConsoleInput();
	}
	catch (QuitInputException)
	{
		std::cout << "Cube input canceled!" << std::endl;
		return;
	}
	catch (InvalidCubeException invalidCubeException)
	{
		std::cout << "Cube input was invalid!" << std::endl;
		std::cout << invalidCubeException.report();
		std::cout << "Cube input canceled!" << std::endl;
		return;
	}
}




Pieces Cube::getAllPieces() const
{
	Pieces pieces;
	for (auto const & piecePos : Cube::listOfAllPiecePositions)
	{
		std::list<Color> piece;
		for (auto const & stickerPos : piecePos)
		{
			piece.push_back(this->at(stickerPos));
		}
		pieces.push_back(piece);
	}
	return std::move(pieces);
}


bool Cube::allPiecesValid(Pieces& invalidPieces) const
{
	Pieces correctPieces = Cube().getAllPieces();
	Pieces thisPieces = this->getAllPieces();

	Pieces invalidPiecesCollector;

	for (auto const & piece : thisPieces)
	{
		bool partnerFound = false;
		for (auto const & correctPiece : correctPieces)
		{
			if (Cube::arePiecesEqual(piece, correctPiece))
			{
				partnerFound = true;
				break;
			}
		}
		if (partnerFound)
		{
			continue;
		}
		invalidPiecesCollector.push_back(piece);
	}

	invalidPieces = invalidPiecesCollector;
	return invalidPiecesCollector.empty();
}




const std::list<PiecePos> Cube::listOfAllPiecePositions = {
	//Corners
	{ StickerPos(PlanePos::UP, 0u), StickerPos(PlanePos::LEFT, 0u) , StickerPos(PlanePos::BACK, 2u) },
	{ StickerPos(PlanePos::UP, 2u), StickerPos(PlanePos::BACK, 0u) , StickerPos(PlanePos::RIGHT, 2u) },
	{ StickerPos(PlanePos::UP, 4u), StickerPos(PlanePos::RIGHT, 0u) , StickerPos(PlanePos::FRONT, 2u) },
	{ StickerPos(PlanePos::UP, 6u), StickerPos(PlanePos::FRONT, 0u) , StickerPos(PlanePos::LEFT, 2u) },
	{ StickerPos(PlanePos::DOWN, 0u), StickerPos(PlanePos::LEFT, 4u) , StickerPos(PlanePos::FRONT, 6u) },
	{ StickerPos(PlanePos::DOWN, 2u), StickerPos(PlanePos::FRONT, 4u) , StickerPos(PlanePos::RIGHT, 6u) },
	{ StickerPos(PlanePos::DOWN, 4u), StickerPos(PlanePos::RIGHT, 4u) , StickerPos(PlanePos::BACK, 6u) },
	{ StickerPos(PlanePos::DOWN, 6u), StickerPos(PlanePos::BACK, 4u) , StickerPos(PlanePos::LEFT, 6u) },

	//Edges
	{ StickerPos(PlanePos::UP, 1u), StickerPos(PlanePos::BACK, 1u) },
	{ StickerPos(PlanePos::UP, 3u), StickerPos(PlanePos::RIGHT, 1u) },
	{ StickerPos(PlanePos::UP, 5u), StickerPos(PlanePos::FRONT, 1u) },
	{ StickerPos(PlanePos::UP, 7u), StickerPos(PlanePos::LEFT, 1u) },
	{ StickerPos(PlanePos::FRONT, 3u), StickerPos(PlanePos::RIGHT, 7u) },
	{ StickerPos(PlanePos::RIGHT, 3u), StickerPos(PlanePos::BACK, 7u) },
	{ StickerPos(PlanePos::BACK, 3u), StickerPos(PlanePos::LEFT, 7u) },
	{ StickerPos(PlanePos::LEFT, 3u), StickerPos(PlanePos::FRONT, 7u) },
	{ StickerPos(PlanePos::DOWN, 1u), StickerPos(PlanePos::FRONT, 5u) },
	{ StickerPos(PlanePos::DOWN, 3u), StickerPos(PlanePos::RIGHT, 5u) },
	{ StickerPos(PlanePos::DOWN, 5u), StickerPos(PlanePos::BACK, 5u) },
	{ StickerPos(PlanePos::DOWN, 7u), StickerPos(PlanePos::LEFT, 5u) }
};




bool Cube::arePiecesEqual(std::list<Color> const & p1, std::list<Color> const & p2)
{
	if (p1.size() != p2.size())
	{
		return false;
	}
	unsigned int const size = p1.size();

	if (size == 2u)
	{
		bool exactlyEqual = (p1 == p2);
		std::list<Color> reversedPiece2(p2);
		reversedPiece2.reverse();
		bool reversedEqual = (p1 == reversedPiece2);
		return (exactlyEqual || reversedEqual);
	}

	if (size == 3u)
	{
		std::list<Color>::const_iterator middleIteratorOfP2 = p2.begin();
		middleIteratorOfP2++;
		std::list<Color> onceCycledP2 = { *middleIteratorOfP2, p2.back(), p2.front() };
		std::list<Color> twiceCycledP2 = { p2.back(), p2.front(), *middleIteratorOfP2 };
		return ((p1 == p2) || (p1 == onceCycledP2) || (p1 == twiceCycledP2));
	}

	if ((size != 2u) && (size != 3u))
	{
		std::cout << "Pieces should contain 2 or 3 stickers! Learn programming!" << std::endl;
		throw 0;
	}
}








Cube::InvalidCubeException::Info::Info(std::map<Color, unsigned int> const & _colorNumbers)
	: colorNumbers(_colorNumbers)
{
}

Cube::InvalidCubeException::Info::Info(Pieces const & _listOfInvalidPieces)
	: listOfInvalidPieces(_listOfInvalidPieces)
{
}

Cube::InvalidCubeException::Info::Info(bool _cornerRotatedClockwise)
	: cornerRotatedClockwise(_cornerRotatedClockwise)
{
}

const Cube::InvalidCubeException::Info Cube::InvalidCubeException::Info::NO_INFO = Cube::InvalidCubeException::Info();

Cube::InvalidCubeException::InvalidCubeException(Reason const & _reason, Info const & _info)
	: reason(_reason), info(_info)
{
}

std::string Cube::InvalidCubeException::report() const
{
	std::stringstream stringStream;
	stringStream << "Reason for invalidity: ";
	switch (this->reason)
	{
	case InvalidCubeException::Reason::NUMBER_PER_COLOR_IS_NOT_8:
	{
		stringStream << "There are colors that don't occur 8 times!" << std::endl;
		std::map<Color, unsigned int> colorMap = this->info.colorNumbers;
		for (auto color : listOfAllColors)
		{
			stringStream << mapOfColorNames.at(color) << ": " << colorMap.at(color) << std::endl;
		}
		break;
	}
	case InvalidCubeException::Reason::PIECES_ARE_INVALID:
	{
		stringStream << "There are invalid pieces!" << std::endl;
		Pieces invalidPieces = this->info.listOfInvalidPieces;
		for (auto const & piece : invalidPieces)
		{
			if (piece.size() == 2)
			{
				stringStream << "Edge: ";
			}
			else
			{
				stringStream << "Corner: ";
			}
			for (auto const & color : piece)
			{
				stringStream << mapOfColorNames.at(color) << " ";
			}
			stringStream << std::endl;
		}
		break;
	}
	case InvalidCubeException::Reason::ONE_EDGE_IS_TILTED:
		stringStream << "One edge es tilted!" << std::endl;
		break;
	case InvalidCubeException::Reason::ONE_CORNER_IS_ROTATED:
		stringStream << "One corner is rotated " << ((this->info.cornerRotatedClockwise) ? "clockwise" : "counter clockwise") << "!" << std::endl;
		break;
	case InvalidCubeException::Reason::TWO_PIECES_ARE_EXCHANGED:
		stringStream << "Two pieces are exchanged!" << std::endl;
		break;
	}

	return stringStream.str();
}









