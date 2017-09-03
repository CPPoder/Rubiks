#include "TurnOrder.hpp"


TurnOrder::TurnOrder(Container<Turn> const & turns)
	: mTurns(turns)
{
}



const TurnOrder TurnOrder::T = TurnOrder({
	Turn::Right,
	Turn::Up,
	Turn::RightInverse,
	Turn::UpInverse,
	Turn::RightInverse,
	Turn::Front,
	Turn::Right2,
	Turn::UpInverse,
	Turn::RightInverse,
	Turn::UpInverse,
	Turn::Right,
	Turn::Up,
	Turn::RightInverse,
	Turn::FrontInverse
});