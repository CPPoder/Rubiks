#include "Random.hpp"



int Random::getRandomIntBetween(int first, int last)
{
	static std::default_random_engine engine;
	std::uniform_int_distribution<int> distr(first, last);
	return distr(engine);
}


double Random::getRandomDoubleBetween(double first, double last)
{
	static std::default_random_engine engine;
	std::uniform_real_distribution<double> distr(first, last);
	return distr(engine);
}

