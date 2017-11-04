#include "Timer.hpp"


User* Timer::pActualUser = nullptr;


void Timer::setActualUser(User const & user)
{
	if (pActualUser != nullptr)
	{
		pActualUser->saveRecords();
		delete pActualUser;
		pActualUser = nullptr;
	}
	pActualUser = new User(user);
}


void Timer::startSubprogram()
{
	try
	{
		Timer::getUserInput();
		Timer::showData();
		while (true)
		{
			Timer::waitForContinuation();
			Timer::doActualTimerStuff();
			Timer::showData();
		}
	}
	catch (QuitException)
	{
		std::cout << "Aborted solving subprogram!" << std::endl;
		if (pActualUser != nullptr)
		{
			pActualUser->saveRecords();
			delete pActualUser;
			pActualUser = nullptr;
		}
		return;
	}
}



void Timer::getUserInput()
{
	//Show existing users
	std::cout << "Welcome in the Timer subprogram! Existing users are: " << std::endl;
	std::cout << "#\tName" << std::endl;
	std::cout << "----------------------" << std::endl;
	std::vector<User> vecOfUsers = User::loadUsers();
	for (unsigned int i = 0; i < vecOfUsers.size(); ++i)
	{
		std::cout << i << '\t' << vecOfUsers.at(i).getName() << std::endl;
	}
	if (vecOfUsers.empty())
	{
		std::cout << "---None---" << std::endl;
	}
	std::cout << std::endl;

	if (vecOfUsers.empty()) //If vecOfUsers is empty start with creation of a user
	{
		Timer::createUserViaUI();
		Timer::getUserInput();
	}
	else //Otherwise, let the user choose an option
	{
		std::cout << "You can choose from the following options: " << std::endl;
		std::cout << "1" << '\t' << "Choose an user" << std::endl;
		std::cout << "2" << '\t' << "Create an user" << std::endl;
		std::cout << "3" << '\t' << "Delete an user" << std::endl;
		std::string optionString = UI::demand("Choose an option: ", { "1", "2", "3" });
		int option = std::stoi(optionString);
		switch (option)
		{
		case 1:
			Timer::chooseUserViaUI();
			break;
		case 2:
			Timer::createUserViaUI();
			Timer::getUserInput();
			break;
		case 3:
			Timer::deleteUserViaUI();
			Timer::getUserInput();
			break;
		}
	}
}

void Timer::showData()
{
	auto list = pActualUser->getListOfRecords();
	for (auto const & rec : list)
	{
		std::cout << Record::makeDateStringFromRecord(rec) << '\t' << Record::makeTimeStringFromRecord(rec) << '\t' << Record::makeScrambleStringFromRecord(rec) << std::endl;
	}
}

TimePair Timer::doActualTimerStuff()
{
	//Generate Scramble
	TurnTypeOrder scramble = Scrambler::getScramble(20u);

	std::cout << "Scramble: " << scramble << std::endl;
	
	//Pause till space released (Start of solving process)
	Timer::pauseTillSpaceRelease();

	//Start timer clock
	Clock timerClock;
	timerClock.restart();

	//Start observation clock
	Clock observationClock;
	observationClock.restart();

	//Pause till space released (Solving process finished) and show time
	std::cout << "Solve time: " << 0 << "." << 0 << " s";
	KeyWatcher keyWatcherForSpace(sf::Keyboard::Key::Space);
	while (true)
	{
		if (keyWatcherForSpace.checkForKeyRelease())
		{
			break;
		}
		if (observationClock.getElapsedTimeAsMicroseconds() > 100000)
		{
			observationClock.restart();
			TimePair timePair = TimePair::trafoMicrosecsIntoTimePair(timerClock.getElapsedTimeAsMicroseconds());
			std::cout << '\r' << "Solve time: " << timePair;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	//Get time
	TimePair timePair = TimePair::trafoMicrosecsIntoTimePair(timerClock.getElapsedTimeAsMicroseconds());
	std::cout << '\r' << "Solve time: " << timePair << std::endl;
	Timer::pActualUser->addRecord(Record(Date::getCurrentDate(), timePair, scramble));
	return timePair;
}

void Timer::waitForContinuation()
{
	std::cout << "Press Space to continue, ESC to quit..." << std::endl;
	KeyWatcher keyWatcherForESC(sf::Keyboard::Key::Escape);
	KeyWatcher keyWatcherForSpace(sf::Keyboard::Key::Space);
	while (true)
	{
		if (keyWatcherForESC.checkForKeyRelease())
		{
			throw QuitException();
		}
		if (keyWatcherForSpace.checkForKeyRelease())
		{
			return;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}
}


void Timer::chooseUserViaUI()
{
	std::vector<User> vecOfUsers = User::loadUsers();
	std::list<std::string> allowedStrings;
	for (unsigned int i = 0; i < vecOfUsers.size(); ++i)
	{
		allowedStrings.push_back(std::to_string(i));
	}
	std::string userString = UI::demand("Type in the number of the user you want to choose: ", allowedStrings);
	int userNumber = std::stoi(userString);
	Timer::setActualUser(vecOfUsers.at(userNumber));
	std::cout << "Welcome " << Timer::pActualUser->getName() << "!" << std::endl;
	std::cout << std::endl;
}


void Timer::createUserViaUI()
{
	while (true)
	{
		std::string name = UI::demand("Input a name for user creation: ", UI::IS_LETTER_OR_NUMBER);
		bool success = User::registerUser(name);
		if (success)
		{
			std::cout << "User has been successfully created!" << std::endl;
			std::cout << std::endl;
			break;
		}
		else
		{
			std::cout << "User name does already exist! Try again!" << std::endl;
		}
	}
}


void Timer::deleteUserViaUI()
{
	//Find userNumber of user to be deleted
	std::vector<User> vecOfUsers = User::loadUsers();
	std::list<std::string> allowedStrings;
	for (unsigned int i = 0; i < vecOfUsers.size(); ++i)
	{
		allowedStrings.push_back(std::to_string(i));
	}
	std::string userString = UI::demand("Type in the number of the user you want to delete: ", allowedStrings);
	int userNumber = std::stoi(userString);
	
	//Demand assurance and delete in case
	std::string sure = UI::demand("Are you sure? (yes, n) ", { "yes", "n" });
	if (sure == "yes")
	{
		User userToDelete = vecOfUsers.at(userNumber);
		User::eraseUser(userToDelete.getName());
		std::cout << "User " << userToDelete.getName() << " has been successfully deleted!" << std::endl;
		std::cout << std::endl;
	}
	else
	{
		std::cout << "User deletion has been aborted!" << std::endl;
		std::cout << std::endl;
	}
}


void Timer::pauseTillSpaceRelease()
{
	KeyWatcher keyWatcherForSpace(sf::Keyboard::Key::Space);
	while (true)
	{
		if (keyWatcherForSpace.checkForKeyRelease())
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}
}

