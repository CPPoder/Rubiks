#include "Color.hpp"
#include "colorOutput.h"
#include <functional>

char getColorChar(Color color)
{
	switch (color)
	{
	case Color::WHITE:
		return 'w';
	case Color::YELLOW:
		return 'y';
	case Color::RED:
		return 'r';
	case Color::ORANGE:
		return 'o';
	case Color::BLUE:
		return 'b';
	case Color::GREEN:
		return 'g';
	}
};

Color getCharColor(char c)
{
	switch (c)
	{
	case 'w':
		return Color::WHITE;
	case 'y':
		return Color::YELLOW;
	case 'r':
		return Color::RED;
	case 'o':
		return Color::ORANGE;
	case 'b':
		return Color::BLUE;
	case 'g':
		return Color::GREEN;
	}
}



std::ostream& operator<<(std::ostream & oStream, Color color)
{
	switch (color)
	{
	case Color::WHITE:
		oStream << white << getColorChar(color) << white;
		break;
	case Color::YELLOW:
		oStream << yellow << getColorChar(color) << white;
		break;
	case Color::RED:
		oStream << red << getColorChar(color) << white;
		break;
	case Color::ORANGE:
		oStream << orange << getColorChar(color) << white;
		break;
	case Color::BLUE:
		oStream << blue << getColorChar(color) << white;
		break;
	case Color::GREEN:
		oStream << green << getColorChar(color) << white;
		break;
	}
	return oStream;
}

