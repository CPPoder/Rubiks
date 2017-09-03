#ifndef COLOR_HPP
#define COLOR_HPP


#include <iostream>


enum class Color
{
	WHITE,
	YELLOW,
	RED,
	ORANGE,
	BLUE,
	GREEN
};


char getColorChar(Color color);
Color getCharColor(char c);


std::ostream& operator<<(std::ostream & oStream, Color color);




#endif //COLOR_HPP

