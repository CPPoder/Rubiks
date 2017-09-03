#ifndef TURNORDER_HPP
#define TURNORDER_HPP


#include "Turn.hpp"

#include <vector>


template <typename T> using Container = std::vector<T>;


class TurnOrder
{
public:
	Container<Turn> mTurns;
	
public:
	TurnOrder() = default; //Creates Empty Container
	TurnOrder(Container<Turn> const & turns);
	~TurnOrder() = default;
	TurnOrder(TurnOrder const &) = default;
	TurnOrder& operator=(TurnOrder const &) = default;

public:
	static const TurnOrder T;

};



#endif //TURNORDER_HPP

