#ifndef COLOR_HPP
#define COLOR_HPP


#include <iostream>
#include <list>
#include <map>


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


extern const std::list<Color> listOfAllColors;
extern const std::map<Color, std::string> mapOfColorNames;



#endif //COLOR_HPP

