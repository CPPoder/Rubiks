#ifndef TURNTYPEORDER_HPP
#define TURNTYPEORDER_HPP


#include "TurnType.hpp"


#include <vector>
#include <iostream>
#include <list>
#include <map>


using TurnTypeOrder = std::vector<TurnType>;

std::ostream& operator<<(std::ostream& oStream, TurnTypeOrder const & turnTypeOrder);

TurnTypeOrder operator!(TurnTypeOrder const & turnTypeOrder);

struct EmptyTurnTypeOrder
{
	static const TurnTypeOrder Empty;
};

struct PLL //x <-> R, y <-> U, z <-> F
{
	static const TurnTypeOrder AClock; //A: Swap 3 corners														//Ll'UR'D2RU'R'D2R2x'			R'FR'B2RF'R'B2R2
	static const TurnTypeOrder ACounter;																		//RrU'LD2L'ULD2L2x'				LF'LB2L'FLB2L2
	static const TurnTypeOrder E; //Swap front and back corners													//M'R2U'LD'L'ULRU'R'DRUx		L'R'B'LF'L'BLRB'R'FRB
	static const TurnTypeOrder Z; //Swap edges diagonally														//M2UM2UM'U2M2U2M'U2			L2R2DL2R2UL'RB2L2R2F2L'RU2
	static const TurnTypeOrder H; //Swap 4 edges																//M2UM2U2M2UM2					L2R2DL2R2U2L2R2DL2R2
	static const TurnTypeOrder UClock; //U: Swap 3 edges														//M2U'M'U2MU'M2					L2R2D'L'RF2LR'D'L2R2
	static const TurnTypeOrder UCounter;																		//M2UM'U2MUM2					R2L2DL'RF2LR'DR2L2
	static const TurnTypeOrder L;																				//L'U'LFL'U'LULF'L2ULU			
	static const TurnTypeOrder J; //Mirrored version of L														//RUR'F'RUR'U'R'FR2U'R'U'		
	static const TurnTypeOrder T; //Infamous T-perm																//RUR'U'R'FR2U'R'U'RUR'F'		
	static const TurnTypeOrder RRight; //RRight: Swap front and right edge & the 2 back corners					//R'U2RU2R'FRUR'U'R'F'R2U'		
	static const TurnTypeOrder RLeft; //RRight: Swap front and left edge & the 2 back corners					//LU2L'U2LF'L'U'LULFL2U			
	static const TurnTypeOrder F; //Swap left and right side front pairs										//RU'R'UR2yRUR'U'xU'FfR2URU'x'	RU'R'UR2BUB'U'R'UR2FRF'
	static const TurnTypeOrder V; //Swap left front and right back pairs										//L'URU'LUL'UR'U'LU2RU2R'		
	static const TurnTypeOrder NRight; //Swap right front and left back pairs									//R'UL'U2RU'LR'UL'U2RU'LU		
	static const TurnTypeOrder NLeft; //Swap left front and right back pairs									//LU'RU2L'ULR'U'RU2L'UR'U'		
	static const TurnTypeOrder Y; //Infamous Y-perm																//FRU'R'U'RUR'F'RUR'U'R'FRF'	
	static const TurnTypeOrder GRightMixed; //Solved pieces right front, mixed corner colors on opposite side	//y'R2Uu'RU'RUR'UuR2yRU'R'		F2D'LU'LUL'DF2RU'R'
	static const TurnTypeOrder GRightEqual; //Solved pieces right front, equal corner colors on opposite side	//RUR'y'R2Uu'RU'R'UR'UuR2y		RUR'F2D'LU'L'UL'DF2
	static const TurnTypeOrder GLeftMixed; //Solved pieces left front, mixed corner colors on opposite side		//yL2UuL'UL'U'LUu'L2y'L'UL		F2DR'UR'U'RD'F2L'UL
	static const TurnTypeOrder GLeftEqual; //Solved pieces left front, equal corner colors on opposite side		//L'U'LyL2UuL'ULU'LUu'L2y'		L'U'LF2DR'URU'RD'F2

	static const std::list<TurnTypeOrder const *> listOfEssentialPLLs;
	static const std::list<TurnTypeOrder const *> listOfCornerPLLs;
	static const std::list<TurnTypeOrder const *> listOfEdgePLLs;
	static const std::list<TurnTypeOrder const *> listOfMixedPLLs;
	static const std::list<TurnTypeOrder const *> listOfPLLs;

private:
	static const std::map<std::string, TurnTypeOrder const *> mapOfTurnTypeOrderNames;

public:
	static bool isStringPLLName(std::string const & string, TurnTypeOrder& turnTypeOrder);


};




struct OLL
{
	static const TurnTypeOrder CrossLine;						//FRUR'U'F'
	static const TurnTypeOrder CrossCurve;						//FfRUR'U'Ff'					BULU'L'B'
	static const TurnTypeOrder CrossPoint;						//CrossLine + CrossCurve
	static const TurnTypeOrder Corners2FrontPointingFront;		//R2DR'U2RD'R'U2R'
	static const TurnTypeOrder Corners2LeftPointingFrontBack;	//F'RrUR'U'L'URx'				F'LFR'F'L'FR
	static const TurnTypeOrder Corners2Diagonal;				//RrUR'U'Rr'FRF'				LFR'F'L'FRF'
	static const TurnTypeOrder Corners3RightFront;				//RUR'URU2R'
	static const TurnTypeOrder Corners3LeftFront;				//L'U'LU'L'U2L
	static const TurnTypeOrder Corners4DifferentDirection;		//RU2R2U'R2U'R2U2R
	static const TurnTypeOrder Corners4SameDircetion;			//FRUR'U'RUR'U'RUR'U'F'

	static const std::list<TurnTypeOrder const *> listOfCrossOLLs;
	static const std::list<TurnTypeOrder const *> listOfCornerOLLs;
	static const std::list<TurnTypeOrder const *> listOfOLLs;
	
private:
	static const std::map<std::string, TurnTypeOrder const *> mapOfTurnTypeOrderNames;

public:
	static bool isStringOLLName(std::string const & string, TurnTypeOrder& turnTypeOrder);

};



struct F2L
{
	static const TurnTypeOrder InsertEdgeRight;		//L'U'LUFUF'
	static const TurnTypeOrder InsertEdgeLeft;		//RUR'U'F'U'F
	static const TurnTypeOrder FlipEdge;			//L'ULU'FU2F'U2FU'F'

	static const std::list<TurnTypeOrder const *> listOfSecondLayerAlgorithms;
	
	static const std::list<TurnTypeOrder> getListOfSecondLayerAlgorithms(); //Returns the above 3 Turns, but in all directions! Thus, 12 turns!
	static const std::list<TurnTypeOrder> getListOfExtractions(); //Returns only extraction turns in all directions! Thus, 8 turns!
};




#endif //TURNTYPEORDER_HPP

