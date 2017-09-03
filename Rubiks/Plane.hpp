#ifndef PLANE_HPP
#define PLANE_HPP



#include <array>


#include "Color.hpp"



class Plane
{
private:
	std::array<Color, 8> mArrayOfColors;

public:
	enum class ArrayToPlaneMode
	{
		CYCLIC,
		ROWWISE
	};

public:
	Plane() = delete;
	Plane(std::array<Color, 8> const & arrayOfColors, ArrayToPlaneMode mode = ArrayToPlaneMode::CYCLIC);
	Plane(Color color);
	~Plane() = default;
	Plane(Plane const & plane) = default;
	Plane& operator=(Plane const & plane) = default;


public:
	Color& at(unsigned int sticker);
	Color const & at(unsigned int sticker) const;

	void rotateClockwise();
	void rotateCounterClockwise();
	void rotateTwice();

private:
	static std::array<Color, 8> mapArrayFromAnyModeToCyclicMode(std::array<Color, 8> arrayInAnyMode, ArrayToPlaneMode wantedMode);

public:
	friend bool operator==(Plane const & plane1, Plane const & plane2);






};







#endif //PLANE_HPP

