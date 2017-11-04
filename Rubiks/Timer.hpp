#ifndef TIMER_HPP
#define TIMER_HPP


#include "Scrambler.hpp"
#include "KeyWatcher.hpp"
#include "Clock.hpp"
#include "Date.hpp"
#include "User.hpp"
#include "UserInput.hpp"
#include "TimePair.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <utility>


class Timer
{
public:
	Timer() = delete;
	~Timer() = delete;

private:
	static User* pActualUser;
	static void setActualUser(User const & user);

public:
	static void startSubprogram();

private:
	static void getUserInput();
	static void showData();
	static TimePair doActualTimerStuff();
	static void waitForContinuation();

private:
	static void chooseUserViaUI();
	static void createUserViaUI();
	static void deleteUserViaUI();

private:
	static void pauseTillSpaceRelease();


private:
	struct QuitException{};


};


#endif //TIMER_HPP

