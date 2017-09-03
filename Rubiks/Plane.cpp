#include "Plane.hpp"



Plane::Plane(std::array<Color, 8> const & arrayOfColors, ArrayToPlaneMode mode)
	: mArrayOfColors(Plane::mapArrayFromAnyModeToCyclicMode(arrayOfColors, mode))
{
}

Plane::Plane(Color color)
	: Plane({color, color, color, color, color, color, color, color})
{
}



Color& Plane::at(unsigned int sticker)
{
	return mArrayOfColors.at(sticker);
}


Color const & Plane::at(unsigned int sticker) const
{
	return mArrayOfColors.at(sticker);
}




void Plane::rotateClockwise()
{
	Color zero = this->at(0);
	Color one = this->at(1);

	this->at(0) = this->at(6);
	this->at(6) = this->at(4);
	this->at(4) = this->at(2);
	this->at(2) = zero;

	this->at(1) = this->at(7);
	this->at(7) = this->at(5);
	this->at(5) = this->at(3);
	this->at(3) = one;
}

void Plane::rotateCounterClockwise()
{
	Color zero = this->at(0);
	Color one = this->at(1);

	this->at(0) = this->at(2);
	this->at(2) = this->at(4);
	this->at(4) = this->at(6);
	this->at(6) = zero;

	this->at(1) = this->at(3);
	this->at(3) = this->at(5);
	this->at(5) = this->at(7);
	this->at(7) = one;
}

void Plane::rotateTwice()
{
	Color zero = this->at(0);
	Color one = this->at(1);
	Color two = this->at(2);
	Color three = this->at(3);

	this->at(0) = this->at(4);
	this->at(1) = this->at(5);
	this->at(2) = this->at(6);
	this->at(3) = this->at(7);

	this->at(4) = zero;
	this->at(5) = one;
	this->at(6) = two;
	this->at(7) = three;
}



std::array<Color, 8> Plane::mapArrayFromAnyModeToCyclicMode(std::array<Color, 8> arrayInAnyMode, ArrayToPlaneMode arrayToPlaneMode)
{
	switch (arrayToPlaneMode)
	{
	case ArrayToPlaneMode::CYCLIC:
		return arrayInAnyMode;
	case ArrayToPlaneMode::ROWWISE:
		std::array<Color, 8> newArray;
		newArray.at(0) = arrayInAnyMode.at(0);
		newArray.at(1) = arrayInAnyMode.at(1);
		newArray.at(2) = arrayInAnyMode.at(2);
		newArray.at(3) = arrayInAnyMode.at(4);
		newArray.at(4) = arrayInAnyMode.at(7);
		newArray.at(5) = arrayInAnyMode.at(6);
		newArray.at(6) = arrayInAnyMode.at(5);
		newArray.at(7) = arrayInAnyMode.at(3);
		return newArray;
	}
}



bool operator==(Plane const & plane1, Plane const & plane2)
{
	return (plane1.mArrayOfColors == plane2.mArrayOfColors);
}












