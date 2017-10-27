#ifndef TIMER_HPP
#define TIMER_HPP


#include "Scrambler.hpp"
#include "KeyWatcher.hpp"
#include "Clock.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>


class Timer
{
public:
	Timer() = delete;
	~Timer() = delete;

public:
	static void startSubprogram();

private:
	static void selectData();
	static void showData();
	static void doActualTimerStuff();
	static void waitForContinuation();

private:
	static void pauseTillSpaceRelease();


private:
	struct QuitException{};


};





#endif //TIMER_HPP

