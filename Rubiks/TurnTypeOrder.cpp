#include "TurnTypeOrder.hpp"
#include "Framework.hpp"



std::ostream& operator<<(std::ostream& oStream, TurnTypeOrder const & turnTypeOrder)
{
	for (auto const & turnType : turnTypeOrder)
	{
		oStream << turnType;
	}
	return oStream;
}




TurnTypeOrder initializeStaticTurnTypeOrder(std::string const & string)
{
	TurnTypeOrder t;
	if (Turn::isStringConvertibleIntoTurns(string, t))
	{
		return Turn::transformTurnTypeOrderViaZ2Rotation(t);
	}
	else
	{
		std::cout << "Initialization with " << string << " failed!" << std::endl;
		throw "String is not convertible!";
	}
}





const TurnTypeOrder EmptyTurnTypeOrder::Empty = initializeStaticTurnTypeOrder("");



const TurnTypeOrder PLL::AClock = initializeStaticTurnTypeOrder("R'FR'B2RF'R'B2R2");
const TurnTypeOrder PLL::ACounter = initializeStaticTurnTypeOrder("LF'LB2L'FLB2L2");
const TurnTypeOrder PLL::E = initializeStaticTurnTypeOrder("L'R'B'LF'L'BLRB'R'FRB");
const TurnTypeOrder PLL::Z = initializeStaticTurnTypeOrder("L2R2DL2R2UL'RB2L2R2F2L'RU2");
const TurnTypeOrder PLL::H = initializeStaticTurnTypeOrder("L2R2DL2R2U2L2R2DL2R2");
const TurnTypeOrder PLL::UClock = initializeStaticTurnTypeOrder("L2R2D'L'RF2LR'D'L2R2");
const TurnTypeOrder PLL::UCounter = initializeStaticTurnTypeOrder("R2L2DL'RF2LR'DR2L2");
const TurnTypeOrder PLL::L = initializeStaticTurnTypeOrder("L'U'LFL'U'LULF'L2ULU");
const TurnTypeOrder PLL::J = initializeStaticTurnTypeOrder("RUR'F'RUR'U'R'FR2U'R'U'");
const TurnTypeOrder PLL::T = initializeStaticTurnTypeOrder("RUR'U'R'FR2U'R'U'RUR'F'");
const TurnTypeOrder PLL::RRight = initializeStaticTurnTypeOrder("R'U2RU2R'FRUR'U'R'F'R2U'");
const TurnTypeOrder PLL::RLeft = initializeStaticTurnTypeOrder("LU2L'U2LF'L'U'LULFL2U");
const TurnTypeOrder PLL::F = initializeStaticTurnTypeOrder("RU'R'UR2BUB'U'R'UR2FRF'");
const TurnTypeOrder PLL::V = initializeStaticTurnTypeOrder("L'URU'LUL'UR'U'LU2RU2R'");
const TurnTypeOrder PLL::NRight = initializeStaticTurnTypeOrder("R'UL'U2RU'LR'UL'U2RU'LU");
const TurnTypeOrder PLL::NLeft = initializeStaticTurnTypeOrder("LU'RU2L'ULR'U'RU2L'UR'U'");
const TurnTypeOrder PLL::Y = initializeStaticTurnTypeOrder("FRU'R'U'RUR'F'RUR'U'R'FRF'");
const TurnTypeOrder PLL::GRightMixed = initializeStaticTurnTypeOrder("F2D'LU'LUL'DF2RU'R'");
const TurnTypeOrder PLL::GRightEqual = initializeStaticTurnTypeOrder("RUR'F2D'LU'L'UL'DF2");
const TurnTypeOrder PLL::GLeftMixed = initializeStaticTurnTypeOrder("F2DR'UR'U'RD'F2L'UL");
const TurnTypeOrder PLL::GLeftEqual = initializeStaticTurnTypeOrder("L'U'LF2DR'URU'RD'F2");



const TurnTypeOrder OLL::CrossLine = initializeStaticTurnTypeOrder("FRUR'U'F'");
const TurnTypeOrder OLL::CrossCurve = initializeStaticTurnTypeOrder("BULU'L'B'");
const TurnTypeOrder OLL::CrossPoint = OLL::CrossLine + OLL::CrossCurve;
const TurnTypeOrder OLL::Corners2FrontPointingFront = initializeStaticTurnTypeOrder("R2DR'U2RD'R'U2R'");
const TurnTypeOrder OLL::Corners2LeftPointingFrontBack = initializeStaticTurnTypeOrder("F'LFR'F'L'FR");
const TurnTypeOrder OLL::Corners2Diagonal = initializeStaticTurnTypeOrder("LFR'F'L'FRF'");
const TurnTypeOrder OLL::Corners3RightFront = initializeStaticTurnTypeOrder("RUR'URU2R'");
const TurnTypeOrder OLL::Corners3LeftFront = initializeStaticTurnTypeOrder("L'U'LU'L'U2L");
const TurnTypeOrder OLL::Corners4DifferentDirection = initializeStaticTurnTypeOrder("RU2R2U'R2U'R2U2R");
const TurnTypeOrder OLL::Corners4SameDircetion = initializeStaticTurnTypeOrder("FRUR'U'RUR'U'RUR'U'F'");




const std::map<std::string, TurnTypeOrder const *> PLL::mapOfTurnTypeOrderNames =
{
	std::make_pair("A", &PLL::AClock),
	std::make_pair("A'", &PLL::ACounter),
	std::make_pair("E", &PLL::E),
	std::make_pair("Z", &PLL::Z),
	std::make_pair("H", &PLL::H),
	std::make_pair("U", &PLL::UClock),
	std::make_pair("U'", &PLL::UCounter),
	std::make_pair("L", &PLL::L),
	std::make_pair("J", &PLL::J),
	std::make_pair("T", &PLL::T),
	std::make_pair("R_Right", &PLL::RRight),
	std::make_pair("R_Left", &PLL::RLeft),
	std::make_pair("F", &PLL::F),
	std::make_pair("V", &PLL::V),
	std::make_pair("N_Right", &PLL::NRight),
	std::make_pair("N_Left", &PLL::NLeft),
	std::make_pair("Y", &PLL::Y),
	std::make_pair("G_Right_Mixed", &PLL::GRightMixed),
	std::make_pair("G_Right_Equal", &PLL::GRightEqual),
	std::make_pair("G_Left_Mixed", &PLL::GLeftMixed),
	std::make_pair("G_Left_Equal", &PLL::GLeftEqual)
};


const std::list<TurnTypeOrder const *> PLL::listOfEssentialPLLs = { &PLL::AClock, &PLL::ACounter, &PLL::UClock, &PLL::UCounter };
const std::list<TurnTypeOrder const *> PLL::listOfCornerPLLs = { &PLL::AClock, &PLL::ACounter, &PLL::E };
const std::list<TurnTypeOrder const *> PLL::listOfEdgePLLs = { &PLL::Z, &PLL::H, &PLL::UClock, &PLL::UCounter };
const std::list<TurnTypeOrder const *> PLL::listOfMixedPLLs = { &PLL::L, &PLL::J, &PLL::T, &PLL::RRight, &PLL::RLeft, &PLL::F, &PLL::V, &PLL::NRight, &PLL::NLeft, &PLL::Y, &PLL::GRightMixed, &PLL::GRightEqual, &PLL::GLeftMixed, &PLL::GLeftEqual };
const std::list<TurnTypeOrder const *> PLL::listOfPLLs = []() {
	std::list<TurnTypeOrder const *> list;
	list.insert(list.end(), PLL::listOfCornerPLLs.begin(), PLL::listOfCornerPLLs.end());
	list.insert(list.end(), PLL::listOfEdgePLLs.begin(), PLL::listOfEdgePLLs.end());
	list.insert(list.end(), PLL::listOfMixedPLLs.begin(), PLL::listOfMixedPLLs.end());
	return std::move(list);
}();


bool PLL::isStringPLLName(std::string const & string, TurnTypeOrder& turnTypeOrder)
{
	if (PLL::mapOfTurnTypeOrderNames.count(string) == 1)
	{
		turnTypeOrder = *PLL::mapOfTurnTypeOrderNames.at(string);
		return true;
	}
	else
	{
		return false;
	}
}





const std::list<TurnTypeOrder const *> OLL::listOfCrossOLLs = std::list<TurnTypeOrder const *>({ &OLL::CrossPoint, &OLL::CrossLine, &OLL::CrossCurve });
const std::list<TurnTypeOrder const *> OLL::listOfCornerOLLs = std::list<TurnTypeOrder const *>({ &OLL::Corners2FrontPointingFront, &OLL::Corners2LeftPointingFrontBack, &OLL::Corners2Diagonal, &OLL::Corners3RightFront, &OLL::Corners3LeftFront, &OLL::Corners4DifferentDirection, &OLL::Corners4SameDircetion });
const std::list<TurnTypeOrder const *> OLL::listOfOLLs = []()
{
	std::list<TurnTypeOrder const *> list;
	list.insert(list.end(), OLL::listOfCrossOLLs.begin(), OLL::listOfCrossOLLs.end());
	list.insert(list.end(), OLL::listOfCornerOLLs.begin(), OLL::listOfCornerOLLs.end());
	return std::move(list);
}();




const std::map<std::string, TurnTypeOrder const *> OLL::mapOfTurnTypeOrderNames =
{
	std::make_pair("Cross_Point", &OLL::CrossPoint),
	std::make_pair("Cross_Line", &OLL::CrossLine),
	std::make_pair("Cross_Curve", &OLL::CrossCurve),
	std::make_pair("Corners_2_FrontPointingFront", &OLL::Corners2FrontPointingFront),
	std::make_pair("Corners_2_LeftPointingFrontBack", &OLL::Corners2LeftPointingFrontBack),
	std::make_pair("Corners_2_Diagonal", &OLL::Corners2Diagonal),
	std::make_pair("Corners_3_RightFront", &OLL::Corners3RightFront),
	std::make_pair("Corners_3_LeftFront", &OLL::Corners3LeftFront),
	std::make_pair("Corners_4_DifferentDirection", &OLL::Corners4DifferentDirection),
	std::make_pair("Corners_4_SameDirection", &OLL::Corners4SameDircetion)
};



bool OLL::isStringOLLName(std::string const & string, TurnTypeOrder& turnTypeOrder)
{
	if (OLL::mapOfTurnTypeOrderNames.count(string) == 1)
	{
		turnTypeOrder = *OLL::mapOfTurnTypeOrderNames.at(string);
		return true;
	}
	else
	{
		return false;
	}
}


