#ifndef KEYWATCHER_HPP
#define KEYWATCHER_HPP

#include "SFML\Graphics.hpp"

class KeyWatcher
{
public:
	KeyWatcher() = delete;
	KeyWatcher(sf::Keyboard::Key const & key);
	~KeyWatcher() = default;
	KeyWatcher(KeyWatcher const &) = delete;
	KeyWatcher& operator=(KeyWatcher const &) = delete;

private:
	sf::Keyboard::Key const keyToWatch;
	bool wasKeyPressed = false;
	
public:
	bool checkForKeyRelease();



};



#endif //KEYWATCHER_HPP

