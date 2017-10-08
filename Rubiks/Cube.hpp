#ifndef CUBE_HPP
#define CUBE_HPP


#include "Plane.hpp"
#include "PlanePos.hpp"
#include "TurnOrder.hpp"
#include "TurnTypeOrder.hpp"
#include "Clock.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <map>

class Turn;

using Pieces = std::list<std::list<Color>>;
using StickerPos = std::pair<PlanePos, unsigned int>;
using PiecePos = std::list<StickerPos>;

class Cube
{
public:
	class InvalidCubeException;

private:
	Plane mUpPlane;
	Plane mDownPlane;
	Plane mRightPlane;
	Plane mLeftPlane;
	Plane mFrontPlane;
	Plane mBackPlane;


public:
	Cube(); //Creates solved cube
	Cube(std::array<Plane, 6> const & arrayOfPlanes);
	~Cube() = default;
	Cube(Cube const & cube) = default;
	Cube& operator=(Cube const & cube) = default;


public:
	Plane& at(PlanePos planePos);
	Plane const & at(PlanePos planePos) const;

	Color& at(StickerPos const & stickerPos);
	Color const & at(StickerPos const & stickerPos) const;
	Color& at(PlanePos planePos, unsigned int stickerNumber);
	Color const & at(PlanePos planePos, unsigned int stickerNumber) const;

	void turn(Turn const & turn);
	void turn(TurnType turnType);
	void turn(TurnTypeOrder const & turnTypeOrder);
	void turn(TurnOrder const & turnOrder);

	void askForConsoleInput();
	bool isValid(InvalidCubeException & returnExceptionIfNotValid) const;
	bool numberOfAllColorsIs8(std::map<Color, unsigned int> & colorNumbers) const;
	Pieces getAllPieces() const;
	bool allPiecesValid(Pieces& invalidPieces) const;


public:
	static const std::list<PiecePos> listOfAllPiecePositions;
	static bool arePiecesEqual(std::list<Color> const & p1, std::list<Color> const & p2);

public:
	static Color getCorrectColorOfPlane(PlanePos planePos);
	static const std::array<Color, 6> arrayOfCorrectColorsPerPlane;


public:
	enum class TerminalOutputOptions
	{
		OUTPUT_AS_UDFBRL,
		OUTPUT_AS_CUBE_NET
	};
	static TerminalOutputOptions terminalOutputOptions;


public:
	friend std::ostream& operator<<(std::ostream& oStream, Cube const & cube);
	friend bool operator==(Cube const & cube1, Cube const & cube2);


private:
	void writeUDFBRLTerminalOutput(std::ostream& oStream) const;
	void writeCubeNetTerminalOutput(std::ostream& oStream) const;

public:
	struct InvalidCubeException
	{
		enum class Reason
		{
			NUMBER_PER_COLOR_IS_NOT_8,
			PIECES_ARE_INVALID,
			ONE_EDGE_IS_TILTED,
			ONE_CORNER_IS_ROTATED,
			TWO_PIECES_ARE_EXCHANGED
		} reason;

		struct Info
		{
			std::map<Color, unsigned int> colorNumbers;
			Pieces listOfInvalidPieces;
			bool cornerRotatedClockwise;

			Info() = default;
			Info(std::map<Color, unsigned int> const & _colorNumbers);
			Info(Pieces const & _listOfInvalidPieces);
			Info(bool _cornerRotatedClockwise);

			static const Info NO_INFO;
		} info;

		InvalidCubeException() = default;
		InvalidCubeException(Reason const & _reason, Info const & _info = Info::NO_INFO);

		std::string report() const;
	};

};


#endif //CUBE_HPP

