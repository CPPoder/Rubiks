#include "Timer.hpp"

void Timer::startSubprogram()
{
	try
	{
		Timer::selectData();
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
		return;
	}
}




void Timer::selectData()
{

}

void Timer::showData()
{

}

void Timer::doActualTimerStuff()
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
	//Clock observationClock;
	//observationClock.restart();

	//Pause till space released (Solving process finished)
	Timer::pauseTillSpaceRelease();

	//Get time
	long long solvingTimeInMicroseconds = timerClock.getElapsedTimeAsMicroseconds();
	int solvingTimeInHundredthOfASecond = solvingTimeInMicroseconds / 10000;
	int hundredths = solvingTimeInHundredthOfASecond % 100;
	int seconds = solvingTimeInHundredthOfASecond / 100;
	std::cout << "Solve time: " << seconds << "." << hundredths << " s" << std::endl;
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

