#include "KeyWatcher.hpp"

KeyWatcher::KeyWatcher(sf::Keyboard::Key const & key)
	: keyToWatch(key)
{
}


bool KeyWatcher::checkForKeyRelease()
{
	bool result;

	bool newKeyPress = sf::Keyboard::isKeyPressed(keyToWatch);
	if (wasKeyPressed)
	{
		result = !newKeyPress;
	}
	else
	{
		result = false;
	}

	wasKeyPressed = newKeyPress;
	return result;
}

