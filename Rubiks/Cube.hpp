#ifndef CUBE_HPP
#define CUBE_HPP


#include "Plane.hpp"
#include "PlanePos.hpp"
#include "Turn.hpp"
#include "TurnOrder.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <map>


class Cube
{
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
	enum class SolveTurns
	{
		ALL,
		QUARTER_TURNS
	};


public:
	class Comparator
	{
	private:
		std::function<bool(Cube const &, Cube const &)> compareFunction;

	public:
		Comparator() = delete;
		Comparator(std::function<bool(Cube const &, Cube const &)> compFunc);
		~Comparator() = default;
		Comparator(Comparator const &) = default;
		Comparator& operator=(Comparator const &) = default;

	public:
		bool operator()(Cube const & cube1, Cube const & cube2) const;
		friend Comparator operator&&(Comparator const & c1, Comparator const & c2);

	public:
		static const Comparator FullCube;
		static const Comparator PureWhiteCross;
		static const Comparator FullWhiteCross;
		struct FullCorner
		{
			static const Comparator UpFrontRight;
			static const Comparator UpFrontLeft;
			static const Comparator UpBackRight;
			static const Comparator UpBackLeft;
			static const Comparator DownFrontRight;
			static const Comparator DownFrontLeft;
			static const Comparator DownBackRight;
			static const Comparator DownBackLeft;
		};
		struct PureCorner
		{
			static const Comparator UpFrontRight;
			static const Comparator UpFrontLeft;
			static const Comparator UpBackRight;
			static const Comparator UpBackLeft;
			static const Comparator DownFrontRight;
			static const Comparator DownFrontLeft;
			static const Comparator DownBackRight;
			static const Comparator DownBackLeft;
		};
		struct Edge
		{
			static const Comparator UpFront;
			static const Comparator UpBack;
			static const Comparator UpRight;
			static const Comparator UpLeft;
			static const Comparator DownFront;
			static const Comparator DownBack;
			static const Comparator DownRight;
			static const Comparator DownLeft;
			static const Comparator FrontRight;
			static const Comparator FrontLeft;
			static const Comparator BackRight;
			static const Comparator BackLeft;
		};
		struct Pair
		{
			static const Comparator FrontRight;
			static const Comparator FrontLeft;
			static const Comparator BackRight;
			static const Comparator BackLeft;
		};
		static const Comparator YellowCross;
		static const Comparator YellowPlane;
	};


public:
	Plane& at(PlanePos planePos);
	Plane const & at(PlanePos planePos) const;
	void turn(Turn const & turn);
	void turn(TurnType turnType);
	void turn(TurnOrder const & turnOrder);
	void solve(SolveTurns solveTurns, Comparator comparator) const;
	void askForConsoleInput();
	bool countColors() const;


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


};


#endif //CUBE_HPP

