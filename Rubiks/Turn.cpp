#include "Turn.hpp"
#include "Cube.hpp"

Turn::Turn(std::function<void(Cube& cube)> turnFunction)
	: mTurnFunction(turnFunction)
{
}




const Turn & Turn::getTurn(TurnType turnType)
{
	switch (turnType)
	{
	case TurnType::Up:
		return Turn::Up;
		break;
	case TurnType::UpInverse:
		return Turn::UpInverse;
		break;
	case TurnType::Up2:
		return Turn::Up2;
		break;
	case TurnType::Down:
		return Turn::Down;
		break;
	case TurnType::DownInverse:
		return Turn::DownInverse;
		break;
	case TurnType::Down2:
		return Turn::Down2;
		break;
	case TurnType::Front:
		return Turn::Front;
		break;
	case TurnType::FrontInverse:
		return Turn::FrontInverse;
		break;
	case TurnType::Front2:
		return Turn::Front2;
		break;
	case TurnType::Back:
		return Turn::Back;
		break;
	case TurnType::BackInverse:
		return Turn::BackInverse;
		break;
	case TurnType::Back2:
		return Turn::Back2;
		break;
	case TurnType::Right:
		return Turn::Right;
		break;
	case TurnType::RightInverse:
		return Turn::RightInverse;
		break;
	case TurnType::Right2:
		return Turn::Right2;
		break;
	case TurnType::Left:
		return Turn::Left;
		break;
	case TurnType::LeftInverse:
		return Turn::LeftInverse;
		break;
	case TurnType::Left2:
		return Turn::Left2;
		break;
	}
}





const Turn Turn::Up = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::UP).rotateClockwise();

	for (unsigned int i = 0; i < 3; ++i)
	{
		Color f = cube.at(PlanePos::FRONT).at(i);
		cube.at(PlanePos::FRONT).at(i) = cube.at(PlanePos::RIGHT).at(i);
		cube.at(PlanePos::RIGHT).at(i) = cube.at(PlanePos::BACK).at(i);
		cube.at(PlanePos::BACK).at(i) = cube.at(PlanePos::LEFT).at(i);
		cube.at(PlanePos::LEFT).at(i) = f;
	}
}
);


const Turn Turn::UpInverse = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::UP).rotateCounterClockwise();

	for (unsigned int i = 0; i < 3; ++i)
	{
		Color f = cube.at(PlanePos::FRONT).at(i);
		cube.at(PlanePos::FRONT).at(i) = cube.at(PlanePos::LEFT).at(i);
		cube.at(PlanePos::LEFT).at(i) = cube.at(PlanePos::BACK).at(i);
		cube.at(PlanePos::BACK).at(i) = cube.at(PlanePos::RIGHT).at(i);
		cube.at(PlanePos::RIGHT).at(i) = f;
	}
}
);


const Turn Turn::Up2 = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::UP).rotateTwice();

	for (unsigned int i = 0; i < 3; ++i)
	{
		Color f = cube.at(PlanePos::FRONT).at(i);
		cube.at(PlanePos::FRONT).at(i) = cube.at(PlanePos::BACK).at(i);
		cube.at(PlanePos::BACK).at(i) = f;

		Color r = cube.at(PlanePos::RIGHT).at(i);
		cube.at(PlanePos::RIGHT).at(i) = cube.at(PlanePos::LEFT).at(i);
		cube.at(PlanePos::LEFT).at(i) = r;
	}
}
);







const Turn Turn::Down = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::DOWN).rotateClockwise();

	for (unsigned int i = 4; i < 7; ++i)
	{
		Color f = cube.at(PlanePos::FRONT).at(i);
		cube.at(PlanePos::FRONT).at(i) = cube.at(PlanePos::LEFT).at(i);
		cube.at(PlanePos::LEFT).at(i) = cube.at(PlanePos::BACK).at(i);
		cube.at(PlanePos::BACK).at(i) = cube.at(PlanePos::RIGHT).at(i);
		cube.at(PlanePos::RIGHT).at(i) = f;
	}
}
);


const Turn Turn::DownInverse = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::DOWN).rotateCounterClockwise();

	for (unsigned int i = 4; i < 7; ++i)
	{
		Color f = cube.at(PlanePos::FRONT).at(i);
		cube.at(PlanePos::FRONT).at(i) = cube.at(PlanePos::RIGHT).at(i);
		cube.at(PlanePos::RIGHT).at(i) = cube.at(PlanePos::BACK).at(i);
		cube.at(PlanePos::BACK).at(i) = cube.at(PlanePos::LEFT).at(i);
		cube.at(PlanePos::LEFT).at(i) = f;
	}
}
);


const Turn Turn::Down2 = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::DOWN).rotateTwice();

	for (unsigned int i = 4; i < 7; ++i)
	{
		Color f = cube.at(PlanePos::FRONT).at(i);
		cube.at(PlanePos::FRONT).at(i) = cube.at(PlanePos::BACK).at(i);
		cube.at(PlanePos::BACK).at(i) = f;

		Color r = cube.at(PlanePos::RIGHT).at(i);
		cube.at(PlanePos::RIGHT).at(i) = cube.at(PlanePos::LEFT).at(i);
		cube.at(PlanePos::LEFT).at(i) = r;
	}
}
);



const Turn Turn::Front = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::FRONT).rotateClockwise();

	//u6->r0->d2->l4->
	Color u6 = cube.at(PlanePos::UP).at(6);
	cube.at(PlanePos::UP).at(6) = cube.at(PlanePos::LEFT).at(4);
	cube.at(PlanePos::LEFT).at(4) = cube.at(PlanePos::DOWN).at(2);
	cube.at(PlanePos::DOWN).at(2) = cube.at(PlanePos::RIGHT).at(0);
	cube.at(PlanePos::RIGHT).at(0) = u6;

	//u5->r7->d1->l3->
	Color u5 = cube.at(PlanePos::UP).at(5);
	cube.at(PlanePos::UP).at(5) = cube.at(PlanePos::LEFT).at(3);
	cube.at(PlanePos::LEFT).at(3) = cube.at(PlanePos::DOWN).at(1);
	cube.at(PlanePos::DOWN).at(1) = cube.at(PlanePos::RIGHT).at(7);
	cube.at(PlanePos::RIGHT).at(7) = u5;

	//u4->r6->d0->l2->
	Color u4 = cube.at(PlanePos::UP).at(4);
	cube.at(PlanePos::UP).at(4) = cube.at(PlanePos::LEFT).at(2);
	cube.at(PlanePos::LEFT).at(2) = cube.at(PlanePos::DOWN).at(0);
	cube.at(PlanePos::DOWN).at(0) = cube.at(PlanePos::RIGHT).at(6);
	cube.at(PlanePos::RIGHT).at(6) = u4;
}
);






const Turn Turn::FrontInverse = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::FRONT).rotateCounterClockwise();

	//u6->l4->d2->r0->
	Color u6 = cube.at(PlanePos::UP).at(6);
	cube.at(PlanePos::UP).at(6) = cube.at(PlanePos::RIGHT).at(0);
	cube.at(PlanePos::RIGHT).at(0) = cube.at(PlanePos::DOWN).at(2);
	cube.at(PlanePos::DOWN).at(2) = cube.at(PlanePos::LEFT).at(4);
	cube.at(PlanePos::LEFT).at(4) = u6;

	//u5->l3->d1->r7->
	Color u5 = cube.at(PlanePos::UP).at(5);
	cube.at(PlanePos::UP).at(5) = cube.at(PlanePos::RIGHT).at(7);
	cube.at(PlanePos::RIGHT).at(7) = cube.at(PlanePos::DOWN).at(1);
	cube.at(PlanePos::DOWN).at(1) = cube.at(PlanePos::LEFT).at(3);
	cube.at(PlanePos::LEFT).at(3) = u5;

	//u4->l2->d0->r6->
	Color u4 = cube.at(PlanePos::UP).at(4);
	cube.at(PlanePos::UP).at(4) = cube.at(PlanePos::RIGHT).at(6);
	cube.at(PlanePos::RIGHT).at(6) = cube.at(PlanePos::DOWN).at(0);
	cube.at(PlanePos::DOWN).at(0) = cube.at(PlanePos::LEFT).at(2);
	cube.at(PlanePos::LEFT).at(2) = u4;
}
);









const Turn Turn::Front2 = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::FRONT).rotateTwice();
	
	//u6<->d2
	//l4<->r0	
	Color u6 = cube.at(PlanePos::UP).at(6);
	cube.at(PlanePos::UP).at(6) = cube.at(PlanePos::DOWN).at(2);
	cube.at(PlanePos::DOWN).at(2) = u6;

	Color l4 = cube.at(PlanePos::LEFT).at(4);
	cube.at(PlanePos::LEFT).at(4) = cube.at(PlanePos::RIGHT).at(0);
	cube.at(PlanePos::RIGHT).at(0) = l4;


	//u5<->d1
	//l3<->r7
	Color u5 = cube.at(PlanePos::UP).at(5);
	cube.at(PlanePos::UP).at(5) = cube.at(PlanePos::DOWN).at(1);
	cube.at(PlanePos::DOWN).at(1) = u5;

	Color l3 = cube.at(PlanePos::LEFT).at(3);
	cube.at(PlanePos::LEFT).at(3) = cube.at(PlanePos::RIGHT).at(7);
	cube.at(PlanePos::RIGHT).at(7) = l3;


	//u4<->d0
	//l2<->r6
	Color u4 = cube.at(PlanePos::UP).at(4);
	cube.at(PlanePos::UP).at(4) = cube.at(PlanePos::DOWN).at(0);
	cube.at(PlanePos::DOWN).at(0) = u4;

	Color l2 = cube.at(PlanePos::LEFT).at(2);
	cube.at(PlanePos::LEFT).at(2) = cube.at(PlanePos::RIGHT).at(6);
	cube.at(PlanePos::RIGHT).at(6) = l2;
}
);









const Turn Turn::Back = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::BACK).rotateClockwise();

	//u0->l6->d4->r2->
	Color u0 = cube.at(PlanePos::UP).at(0);
	cube.at(PlanePos::UP).at(0) = cube.at(PlanePos::RIGHT).at(2);
	cube.at(PlanePos::RIGHT).at(2) = cube.at(PlanePos::DOWN).at(4);
	cube.at(PlanePos::DOWN).at(4) = cube.at(PlanePos::LEFT).at(6);
	cube.at(PlanePos::LEFT).at(6) = u0;

	//u1->l7->d5->r3->
	Color u1 = cube.at(PlanePos::UP).at(1);
	cube.at(PlanePos::UP).at(1) = cube.at(PlanePos::RIGHT).at(3);
	cube.at(PlanePos::RIGHT).at(3) = cube.at(PlanePos::DOWN).at(5);
	cube.at(PlanePos::DOWN).at(5) = cube.at(PlanePos::LEFT).at(7);
	cube.at(PlanePos::LEFT).at(7) = u1;

	//u2->l0->d6->r4->
	Color u2 = cube.at(PlanePos::UP).at(2);
	cube.at(PlanePos::UP).at(2) = cube.at(PlanePos::RIGHT).at(4);
	cube.at(PlanePos::RIGHT).at(4) = cube.at(PlanePos::DOWN).at(6);
	cube.at(PlanePos::DOWN).at(6) = cube.at(PlanePos::LEFT).at(0);
	cube.at(PlanePos::LEFT).at(0) = u2;
}
);










const Turn Turn::BackInverse = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::BACK).rotateCounterClockwise();

	//u0->r2->d4->l6->
	Color u0 = cube.at(PlanePos::UP).at(0);
	cube.at(PlanePos::UP).at(0) = cube.at(PlanePos::LEFT).at(6);
	cube.at(PlanePos::LEFT).at(6) = cube.at(PlanePos::DOWN).at(4);
	cube.at(PlanePos::DOWN).at(4) = cube.at(PlanePos::RIGHT).at(2);
	cube.at(PlanePos::RIGHT).at(2) = u0;

	//u1->r3->d5->l7->
	Color u1 = cube.at(PlanePos::UP).at(1);
	cube.at(PlanePos::UP).at(1) = cube.at(PlanePos::LEFT).at(7);
	cube.at(PlanePos::LEFT).at(7) = cube.at(PlanePos::DOWN).at(5);
	cube.at(PlanePos::DOWN).at(5) = cube.at(PlanePos::RIGHT).at(3);
	cube.at(PlanePos::RIGHT).at(3) = u1;

	//u2->r4->d6->l0->
	Color u2 = cube.at(PlanePos::UP).at(2);
	cube.at(PlanePos::UP).at(2) = cube.at(PlanePos::LEFT).at(0);
	cube.at(PlanePos::LEFT).at(0) = cube.at(PlanePos::DOWN).at(6);
	cube.at(PlanePos::DOWN).at(6) = cube.at(PlanePos::RIGHT).at(4);
	cube.at(PlanePos::RIGHT).at(4) = u2;
}
);









const Turn Turn::Back2 = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::BACK).rotateTwice();

	//u0<->d4
	//r2<->l6	
	Color u0 = cube.at(PlanePos::UP).at(0);
	cube.at(PlanePos::UP).at(0) = cube.at(PlanePos::DOWN).at(4);
	cube.at(PlanePos::DOWN).at(4) = u0;

	Color r2 = cube.at(PlanePos::RIGHT).at(2);
	cube.at(PlanePos::RIGHT).at(2) = cube.at(PlanePos::LEFT).at(6);
	cube.at(PlanePos::LEFT).at(6) = r2;


	//u1<->d5
	//r3<->l7
	Color u1 = cube.at(PlanePos::UP).at(1);
	cube.at(PlanePos::UP).at(1) = cube.at(PlanePos::DOWN).at(5);
	cube.at(PlanePos::DOWN).at(5) = u1;

	Color r3 = cube.at(PlanePos::RIGHT).at(3);
	cube.at(PlanePos::RIGHT).at(3) = cube.at(PlanePos::LEFT).at(7);
	cube.at(PlanePos::LEFT).at(7) = r3;


	//u2<->d6
	//r4<->l0
	Color u2 = cube.at(PlanePos::UP).at(2);
	cube.at(PlanePos::UP).at(2) = cube.at(PlanePos::DOWN).at(6);
	cube.at(PlanePos::DOWN).at(6) = u2;

	Color r4 = cube.at(PlanePos::RIGHT).at(4);
	cube.at(PlanePos::RIGHT).at(4) = cube.at(PlanePos::LEFT).at(0);
	cube.at(PlanePos::LEFT).at(0) = r4;
}
);









const Turn Turn::Right = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::RIGHT).rotateClockwise();

	//u4->b0->d4->f4->
	Color u4 = cube.at(PlanePos::UP).at(4);
	cube.at(PlanePos::UP).at(4) = cube.at(PlanePos::FRONT).at(4);
	cube.at(PlanePos::FRONT).at(4) = cube.at(PlanePos::DOWN).at(4);
	cube.at(PlanePos::DOWN).at(4) = cube.at(PlanePos::BACK).at(0);
	cube.at(PlanePos::BACK).at(0) = u4;

	//u3->b7->d3->f3->
	Color u3 = cube.at(PlanePos::UP).at(3);
	cube.at(PlanePos::UP).at(3) = cube.at(PlanePos::FRONT).at(3);
	cube.at(PlanePos::FRONT).at(3) = cube.at(PlanePos::DOWN).at(3);
	cube.at(PlanePos::DOWN).at(3) = cube.at(PlanePos::BACK).at(7);
	cube.at(PlanePos::BACK).at(7) = u3;

	//u2->b6->d2->f2->
	Color u2 = cube.at(PlanePos::UP).at(2);
	cube.at(PlanePos::UP).at(2) = cube.at(PlanePos::FRONT).at(2);
	cube.at(PlanePos::FRONT).at(2) = cube.at(PlanePos::DOWN).at(2);
	cube.at(PlanePos::DOWN).at(2) = cube.at(PlanePos::BACK).at(6);
	cube.at(PlanePos::BACK).at(6) = u2;
}
);









const Turn Turn::RightInverse = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::RIGHT).rotateCounterClockwise();

	//u4->f4->d4->b0->
	Color u4 = cube.at(PlanePos::UP).at(4);
	cube.at(PlanePos::UP).at(4) = cube.at(PlanePos::BACK).at(0);
	cube.at(PlanePos::BACK).at(0) = cube.at(PlanePos::DOWN).at(4);
	cube.at(PlanePos::DOWN).at(4) = cube.at(PlanePos::FRONT).at(4);
	cube.at(PlanePos::FRONT).at(4) = u4;

	//u3->f3->d3->b7->
	Color u3 = cube.at(PlanePos::UP).at(3);
	cube.at(PlanePos::UP).at(3) = cube.at(PlanePos::BACK).at(7);
	cube.at(PlanePos::BACK).at(7) = cube.at(PlanePos::DOWN).at(3);
	cube.at(PlanePos::DOWN).at(3) = cube.at(PlanePos::FRONT).at(3);
	cube.at(PlanePos::FRONT).at(3) = u3;

	//u2->f2->d2->b6->
	Color u2 = cube.at(PlanePos::UP).at(2);
	cube.at(PlanePos::UP).at(2) = cube.at(PlanePos::BACK).at(6);
	cube.at(PlanePos::BACK).at(6) = cube.at(PlanePos::DOWN).at(2);
	cube.at(PlanePos::DOWN).at(2) = cube.at(PlanePos::FRONT).at(2);
	cube.at(PlanePos::FRONT).at(2) = u2;
}
);










const Turn Turn::Right2 = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::RIGHT).rotateTwice();

	//u4<->d4
	//f4<->b0	
	Color u4 = cube.at(PlanePos::UP).at(4);
	cube.at(PlanePos::UP).at(4) = cube.at(PlanePos::DOWN).at(4);
	cube.at(PlanePos::DOWN).at(4) = u4;

	Color f4 = cube.at(PlanePos::FRONT).at(4);
	cube.at(PlanePos::FRONT).at(4) = cube.at(PlanePos::BACK).at(0);
	cube.at(PlanePos::BACK).at(0) = f4;


	//u3<->d3
	//f3<->b7
	Color u3 = cube.at(PlanePos::UP).at(3);
	cube.at(PlanePos::UP).at(3) = cube.at(PlanePos::DOWN).at(3);
	cube.at(PlanePos::DOWN).at(3) = u3;

	Color f3 = cube.at(PlanePos::FRONT).at(3);
	cube.at(PlanePos::FRONT).at(3) = cube.at(PlanePos::BACK).at(7);
	cube.at(PlanePos::BACK).at(7) = f3;


	//u2<->d2
	//f2<->b6
	Color u2 = cube.at(PlanePos::UP).at(2);
	cube.at(PlanePos::UP).at(2) = cube.at(PlanePos::DOWN).at(2);
	cube.at(PlanePos::DOWN).at(2) = u2;

	Color f2 = cube.at(PlanePos::FRONT).at(2);
	cube.at(PlanePos::FRONT).at(2) = cube.at(PlanePos::BACK).at(6);
	cube.at(PlanePos::BACK).at(6) = f2;
}
);











const Turn Turn::Left = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::LEFT).rotateClockwise();

	//u0->f0->d0->b4->
	Color u0 = cube.at(PlanePos::UP).at(0);
	cube.at(PlanePos::UP).at(0) = cube.at(PlanePos::BACK).at(4);
	cube.at(PlanePos::BACK).at(4) = cube.at(PlanePos::DOWN).at(0);
	cube.at(PlanePos::DOWN).at(0) = cube.at(PlanePos::FRONT).at(0);
	cube.at(PlanePos::FRONT).at(0) = u0;

	//u7->f7->d7->b3->
	Color u7 = cube.at(PlanePos::UP).at(7);
	cube.at(PlanePos::UP).at(7) = cube.at(PlanePos::BACK).at(3);
	cube.at(PlanePos::BACK).at(3) = cube.at(PlanePos::DOWN).at(7);
	cube.at(PlanePos::DOWN).at(7) = cube.at(PlanePos::FRONT).at(7);
	cube.at(PlanePos::FRONT).at(7) = u7;

	//u6->f6->d6->b2->
	Color u6 = cube.at(PlanePos::UP).at(6);
	cube.at(PlanePos::UP).at(6) = cube.at(PlanePos::BACK).at(2);
	cube.at(PlanePos::BACK).at(2) = cube.at(PlanePos::DOWN).at(6);
	cube.at(PlanePos::DOWN).at(6) = cube.at(PlanePos::FRONT).at(6);
	cube.at(PlanePos::FRONT).at(6) = u6;
}
);








const Turn Turn::LeftInverse = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::LEFT).rotateCounterClockwise();

	//u0->b4->d0->f0->
	Color u0 = cube.at(PlanePos::UP).at(0);
	cube.at(PlanePos::UP).at(0) = cube.at(PlanePos::FRONT).at(0);
	cube.at(PlanePos::FRONT).at(0) = cube.at(PlanePos::DOWN).at(0);
	cube.at(PlanePos::DOWN).at(0) = cube.at(PlanePos::BACK).at(4);
	cube.at(PlanePos::BACK).at(4) = u0;

	//u7->b3->d7->f7->
	Color u7 = cube.at(PlanePos::UP).at(7);
	cube.at(PlanePos::UP).at(7) = cube.at(PlanePos::FRONT).at(7);
	cube.at(PlanePos::FRONT).at(7) = cube.at(PlanePos::DOWN).at(7);
	cube.at(PlanePos::DOWN).at(7) = cube.at(PlanePos::BACK).at(3);
	cube.at(PlanePos::BACK).at(3) = u7;

	//u6->b2->d6->f6->
	Color u6 = cube.at(PlanePos::UP).at(6);
	cube.at(PlanePos::UP).at(6) = cube.at(PlanePos::FRONT).at(6);
	cube.at(PlanePos::FRONT).at(6) = cube.at(PlanePos::DOWN).at(6);
	cube.at(PlanePos::DOWN).at(6) = cube.at(PlanePos::BACK).at(2);
	cube.at(PlanePos::BACK).at(2) = u6;
}
);










const Turn Turn::Left2 = Turn(
	[](Cube& cube)
{
	cube.at(PlanePos::LEFT).rotateTwice();

	//u0<->d0
	//b4<->f0	
	Color u0 = cube.at(PlanePos::UP).at(0);
	cube.at(PlanePos::UP).at(0) = cube.at(PlanePos::DOWN).at(0);
	cube.at(PlanePos::DOWN).at(0) = u0;

	Color b4 = cube.at(PlanePos::BACK).at(4);
	cube.at(PlanePos::BACK).at(4) = cube.at(PlanePos::FRONT).at(0);
	cube.at(PlanePos::FRONT).at(0) = b4;


	//u7<->d7
	//b3<->f7
	Color u7 = cube.at(PlanePos::UP).at(7);
	cube.at(PlanePos::UP).at(7) = cube.at(PlanePos::DOWN).at(7);
	cube.at(PlanePos::DOWN).at(7) = u7;

	Color b3 = cube.at(PlanePos::BACK).at(3);
	cube.at(PlanePos::BACK).at(3) = cube.at(PlanePos::FRONT).at(7);
	cube.at(PlanePos::FRONT).at(7) = b3;


	//u6<->d6
	//b2<->f6
	Color u6 = cube.at(PlanePos::UP).at(6);
	cube.at(PlanePos::UP).at(6) = cube.at(PlanePos::DOWN).at(6);
	cube.at(PlanePos::DOWN).at(6) = u6;

	Color b2 = cube.at(PlanePos::BACK).at(2);
	cube.at(PlanePos::BACK).at(2) = cube.at(PlanePos::FRONT).at(6);
	cube.at(PlanePos::FRONT).at(6) = b2;
}
);





const std::array<TurnType, 18> Turn::arrayOfAllTurnTypes = std::array<TurnType, 18>({
	TurnType::Up,
	TurnType::UpInverse,
	TurnType::Up2,
	TurnType::Down,
	TurnType::DownInverse,
	TurnType::Down2,
	TurnType::Front,
	TurnType::FrontInverse,
	TurnType::Front2,
	TurnType::Back,
	TurnType::BackInverse,
	TurnType::Back2,
	TurnType::Right,
	TurnType::RightInverse,
	TurnType::Right2,
	TurnType::Left,
	TurnType::LeftInverse,
	TurnType::Left2
});



const std::array<TurnType, 12> Turn::arrayOfQuarterTurnTypes = std::array<TurnType, 12>({
	TurnType::Up,
	TurnType::UpInverse,
	TurnType::Down,
	TurnType::DownInverse,
	TurnType::Front,
	TurnType::FrontInverse,
	TurnType::Back,
	TurnType::BackInverse,
	TurnType::Right,
	TurnType::RightInverse,
	TurnType::Left,
	TurnType::LeftInverse
});







std::string Turn::getTurnTypeString(TurnType turnType)
{
	switch (turnType)
	{
	case TurnType::Up:
		return "U";
	case TurnType::UpInverse:
		return "U'";
	case TurnType::Up2:
		return "U2";
	case TurnType::Down:
		return "D";
	case TurnType::DownInverse:
		return "D'";
	case TurnType::Down2:
		return "D2";
	case TurnType::Front:
		return "F";
	case TurnType::FrontInverse:
		return "F'";
	case TurnType::Front2:
		return "F2";
	case TurnType::Back:
		return "B";
	case TurnType::BackInverse:
		return "B'";
	case TurnType::Back2:
		return "B2";
	case TurnType::Right:
		return "R";
	case TurnType::RightInverse:
		return "R'";
	case TurnType::Right2:
		return "R2";
	case TurnType::Left:
		return "L";
	case TurnType::LeftInverse:
		return "L'";
	case TurnType::Left2:
		return "L2";
	}
}






TurnType Turn::getTurnTypeFromNumberInArray(unsigned int number, bool useOnlyQuarterTurns)
{
	return (useOnlyQuarterTurns ? Turn::arrayOfQuarterTurnTypes.at(number) : Turn::arrayOfAllTurnTypes.at(number));
}

unsigned int Turn::getNumberOfTurnTypes(bool useOnlyQuarterTurns)
{
	return (useOnlyQuarterTurns ? Turn::arrayOfQuarterTurnTypes.size() : Turn::arrayOfAllTurnTypes.size());
}

TurnType Turn::getRandomTurnType()
{
	int posInArray = Random::getRandomIntBetween(0, Turn::arrayOfAllTurnTypes.size() - 1);
	return Turn::getTurnTypeFromNumberInArray(posInArray, false);
}






TurnType Turn::transformTurnTypeViaZ2Rotation(TurnType turnType)
{
	switch (turnType)
	{
	case TurnType::Up:
		return TurnType::Down;
	case TurnType::UpInverse:
		return TurnType::DownInverse;
	case TurnType::Up2:
		return TurnType::Down2;
	case TurnType::Down:
		return TurnType::Up;
	case TurnType::DownInverse:
		return TurnType::UpInverse;
	case TurnType::Down2:
		return TurnType::Up2;
	case TurnType::Front:
		return TurnType::Front;
	case TurnType::FrontInverse:
		return TurnType::FrontInverse;
	case TurnType::Front2:
		return TurnType::Front2;
	case TurnType::Back:
		return TurnType::Back;
	case TurnType::BackInverse:
		return TurnType::BackInverse;
	case TurnType::Back2:
		return TurnType::Back2;
	case TurnType::Right:
		return TurnType::Left;
	case TurnType::RightInverse:
		return TurnType::LeftInverse;
	case TurnType::Right2:
		return TurnType::Left2;
	case TurnType::Left:
		return TurnType::Right;
	case TurnType::LeftInverse:
		return TurnType::RightInverse;
	case TurnType::Left2:
		return TurnType::Right2;
	}
}




TurnTypeOrder Turn::transformTurnTypeOrderViaZ2Rotation(TurnTypeOrder const & turnTypeOrder)
{
	TurnTypeOrder t;
	t.reserve(turnTypeOrder.size());
	for (auto const & turn : turnTypeOrder)
	{
		t.push_back(Turn::transformTurnTypeViaZ2Rotation(turn));
	}
	return std::move(t);
}







TurnType Turn::transformTurnTypeViaYRotation(TurnType turnType)
{
	switch (turnType)
	{
	case TurnType::Up:
		return TurnType::Up;
	case TurnType::UpInverse:
		return TurnType::UpInverse;
	case TurnType::Up2:
		return TurnType::Up2;
	case TurnType::Down:
		return TurnType::Down;
	case TurnType::DownInverse:
		return TurnType::DownInverse;
	case TurnType::Down2:
		return TurnType::Down2;
	case TurnType::Front:
		return TurnType::Right;
	case TurnType::FrontInverse:
		return TurnType::RightInverse;
	case TurnType::Front2:
		return TurnType::Right2;
	case TurnType::Back:
		return TurnType::Left;
	case TurnType::BackInverse:
		return TurnType::LeftInverse;
	case TurnType::Back2:
		return TurnType::Left2;
	case TurnType::Right:
		return TurnType::Back;
	case TurnType::RightInverse:
		return TurnType::BackInverse;
	case TurnType::Right2:
		return TurnType::Back2;
	case TurnType::Left:
		return TurnType::Front;
	case TurnType::LeftInverse:
		return TurnType::FrontInverse;
	case TurnType::Left2:
		return TurnType::Front2;
	}
}




TurnTypeOrder Turn::transformTurnTypeOrderViaYRotation(TurnTypeOrder const & turnTypeOrder)
{
	TurnTypeOrder t;
	t.reserve(turnTypeOrder.size());
	for (auto const & turn : turnTypeOrder)
	{
		t.push_back(Turn::transformTurnTypeViaYRotation(turn));
	}
	return std::move(t);
}









bool Turn::isStringConvertibleIntoTurns(std::string const & string, TurnTypeOrder& turns)
{
	//Handle empty turns
	if (string == "-")
	{
		TurnTypeOrder turnTypeOrder;
		turnTypeOrder.clear();
		turns = turnTypeOrder;
		return true;
	}

	//Handle PLL case
	if (string.size() >= 5)
	{
		std::string firstPart = string.substr(0, 4);
		if (firstPart == "PLL:")
		{
			std::string secondPart = string.substr(4);
			TurnTypeOrder turnTypeOrder;
			if (PLL::isStringPLLName(secondPart, turnTypeOrder))
			{
				turns = turnTypeOrder;
				return true;
			}
		}
	}

	//Handle OLL case
	if (string.size() >= 5)
	{
		std::string firstPart = string.substr(0, 4);
		if (firstPart == "OLL:")
		{
			std::string secondPart = string.substr(4);
			TurnTypeOrder turnTypeOrder;
			if (OLL::isStringOLLName(secondPart, turnTypeOrder))
			{
				turns = turnTypeOrder;
				return true;
			}
		}
	}

	//Fill list with chars
	std::list<char> listOfChars;
	for (auto c : string)
	{
		listOfChars.push_back(c);
	}

	//Iterate over listOfChars to fill TurnTypeOrder
	TurnTypeOrder turnTypeOrder;
	while (true)
	{
		if (listOfChars.size() > 1)
		{
			//Handle normal iterations
			std::list<char>::iterator it = listOfChars.begin();
			char first = *it;
			char second = *(++it);
			if (Turn::isCharEqualToOneCharInList(second, { '2', '\'' }))
			{
				//Two chars are one turn
				TurnType turnType;
				if (Turn::isStringConvertibleIntoTurnType(std::string({ first, second }), turnType))
				{
					turnTypeOrder.push_back(turnType);
					listOfChars.pop_front();
					listOfChars.pop_front();
				}
				else
				{
					return false;
				}
			}
			else
			{
				//One char is one turn
				TurnType turnType;
				if (Turn::isStringConvertibleIntoTurnType(std::string({ first }), turnType))
				{
					turnTypeOrder.push_back(turnType);
					listOfChars.pop_front();
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			//Handle last iteration
			if (listOfChars.empty())
			{
				break;
			}
			else
			{
				//If the program arrives here, there is exactly one element in listOfChars
				TurnType turnType;
				if (Turn::isStringConvertibleIntoTurnType(std::string({ listOfChars.front() }), turnType))
				{
					turnTypeOrder.push_back(turnType);
					listOfChars.pop_front();
				}
				else
				{
					return false;
				}
			}
		}
	}

	//Return result
	turns = turnTypeOrder;
	return true;
}


bool Turn::isStringConvertibleIntoTurnType(std::string const & string, TurnType& turnType)
{
	if (string == "R")
	{
		turnType = TurnType::Right;
		return true;
	}
	if (string == "R\'")
	{
		turnType = TurnType::RightInverse;
		return true;
	}
	if (string == "R2")
	{
		turnType = TurnType::Right2;
		return true;
	}
	if (string == "L")
	{
		turnType = TurnType::Left;
		return true;
	}
	if (string == "L\'")
	{
		turnType = TurnType::LeftInverse;
		return true;
	}
	if (string == "L2")
	{
		turnType = TurnType::Left2;
		return true;
	}
	if (string == "U")
	{
		turnType = TurnType::Up;
		return true;
	}
	if (string == "U\'")
	{
		turnType = TurnType::UpInverse;
		return true;
	}
	if (string == "U2")
	{
		turnType = TurnType::Up2;
		return true;
	}
	if (string == "D")
	{
		turnType = TurnType::Down;
		return true;
	}
	if (string == "D\'")
	{
		turnType = TurnType::DownInverse;
		return true;
	}
	if (string == "D2")
	{
		turnType = TurnType::Down2;
		return true;
	}
	if (string == "F")
	{
		turnType = TurnType::Front;
		return true;
	}
	if (string == "F\'")
	{
		turnType = TurnType::FrontInverse;
		return true;
	}
	if (string == "F2")
	{
		turnType = TurnType::Front2;
		return true;
	}
	if (string == "B")
	{
		turnType = TurnType::Back;
		return true;
	}
	if (string == "B\'")
	{
		turnType = TurnType::BackInverse;
		return true;
	}
	if (string == "B2")
	{
		turnType = TurnType::Back2;
		return true;
	}
	return false;
}










bool Turn::isCharEqualToOneCharInList(char c, std::list<char> listOfChars)
{
	for (auto ch : listOfChars)
	{
		if (c == ch)
		{
			return true;
		}
	}
	return false;
}





void Turn::appendTurnTypeOrder(TurnTypeOrder& t1, TurnTypeOrder const & t2)
{
	t1.insert(t1.end(), t2.begin(), t2.end());
}


TurnTypeOrder operator+(TurnTypeOrder const & t1, TurnTypeOrder const & t2)
{
	TurnTypeOrder t(t1);
	Turn::appendTurnTypeOrder(t, t2);
	return std::move(t);
}

TurnTypeOrder& operator+=(TurnTypeOrder & t1, TurnTypeOrder const & t2)
{
	Turn::appendTurnTypeOrder(t1, t2);
	return t1;
}

TurnTypeOrder operator+(TurnTypeOrder const & t1, TurnType const & t2)
{
	TurnTypeOrder t(t1);
	t.reserve(t1.size() + 1);
	t.push_back(t2);
	return t;
}

TurnTypeOrder operator+(TurnType const & t1, TurnTypeOrder const & t2)
{
	TurnTypeOrder t;
	t.reserve(1 + t2.size());
	t.push_back(t1);
	Turn::appendTurnTypeOrder(t, t2);
	return t;
}

TurnTypeOrder operator+(TurnType const & t1, TurnType const & t2)
{
	TurnTypeOrder t(2);
	t.at(0) = t1;
	t.at(1) = t2;
	return std::move(t);
}




