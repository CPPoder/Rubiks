#ifndef SOLVER_HPP
#define SOLVER_HPP


#include "Cube.hpp"
#include "MutexProtectedClass.hpp"


#include <sstream>
#include <set>



class Solver
{
public:
	enum class SolveTurns
	{
		ALL,
		QUARTER_TURNS
	};

	class Comparator
	{
	private:
		std::function<bool(Cube const &, Cube const &)> compareFunction;

	public:
		Comparator() = delete;
		Comparator(std::function<bool(Cube const &, Cube const &)> compFunc);
		~Comparator() = default;
		Comparator(Comparator const &) = default;
		Comparator& operator=(Comparator const &) = default;

	public:
		bool operator()(Cube const & cube1, Cube const & cube2) const;
		friend Comparator operator&&(Comparator const & c1, Comparator const & c2);

	public:
		static const Comparator FullCube;
		static const Comparator PureWhiteCross;
		static const Comparator FullWhiteCross;
		struct FullCorner
		{
			static const Comparator UpFrontRight;
			static const Comparator UpFrontLeft;
			static const Comparator UpBackRight;
			static const Comparator UpBackLeft;
			static const Comparator DownFrontRight;
			static const Comparator DownFrontLeft;
			static const Comparator DownBackRight;
			static const Comparator DownBackLeft;
		};
		struct PureCorner
		{
			static const Comparator UpFrontRight;
			static const Comparator UpFrontLeft;
			static const Comparator UpBackRight;
			static const Comparator UpBackLeft;
			static const Comparator DownFrontRight;
			static const Comparator DownFrontLeft;
			static const Comparator DownBackRight;
			static const Comparator DownBackLeft;
		};
		struct Edge
		{
			static const Comparator UpFront;
			static const Comparator UpBack;
			static const Comparator UpRight;
			static const Comparator UpLeft;
			static const Comparator DownFront;
			static const Comparator DownBack;
			static const Comparator DownRight;
			static const Comparator DownLeft;
			static const Comparator FrontRight;
			static const Comparator FrontLeft;
			static const Comparator BackRight;
			static const Comparator BackLeft;
		};
		struct Pair
		{
			static const Comparator FrontRight;
			static const Comparator FrontLeft;
			static const Comparator BackRight;
			static const Comparator BackLeft;
		};
		static const Comparator FirstLayer;
		static const Comparator SecondLayer;
		static const Comparator YellowCross;
		static const Comparator YellowPlane;
		static const Comparator FullCubeComposed;
	};

	struct InterruptProperties
	{
		enum class Type 
		{
			NEVER, AFTER_SECONDS, AFTER_TURNS
		} type = Type::NEVER;
		int info = 0;

		InterruptProperties() = default;
		InterruptProperties(Type _type, int _info = 0);
	};

	struct InterruptContinuationInfo
	{
		bool isContinuation;
		std::vector<unsigned int> oldVecOfTurnNumbers;

		InterruptContinuationInfo();
		InterruptContinuationInfo(std::vector<unsigned int> _oldVecOfTurnNumbers);

		static const InterruptContinuationInfo NO_CONTINUATION;
	};

	struct SolveAttributes
	{
		SolveTurns solveTurns;
		Comparator comparator;
		InterruptProperties interruptProperties;

		SolveAttributes();
		SolveAttributes(SolveTurns const & _solveTurns, Comparator const & _comparator, InterruptProperties const & _interruptProperties);
	};

private:
	struct QuitException{};
	struct SolverState
	{
		std::vector<unsigned int> oldVectorOfTurnNumbers;
		Cube initialCube;
		SolveAttributes solveAttributes;

		SolverState(std::vector<unsigned int> const & _oldVectorOfTurnNumbers, Cube const & _initialCube, SolveAttributes const & _solveAttributes);
	};
	struct InterruptException
	{
		SolverState solverState;

		InterruptException(SolverState const & _solverState);
	};

public:
	enum class F2LPairTargetPos
	{
		RIGHT_FRONT,
		RIGHT_BACK,
		LEFT_FRONT,
		LEFT_BACK
	};

public:
	Solver() = delete;
	~Solver() = delete;
	Solver(Solver const &) = delete;
	Solver& operator=(Solver const &) = delete;

public:
	static void solveManager(Cube const & cube, TurnTypeOrder& turns);

private:
	static void solveSubManager(Cube const & cube, TurnTypeOrder& turns, SolveAttributes const & solveAttributes, InterruptContinuationInfo const & interruptContinuationInfo);
	static TurnTypeOrder solve(Cube const & cube, SolveAttributes solveAttributes, std::vector<unsigned int> const & initialVectorOfTurnNumbers, std::ostream & oStream = std::cout);
	static TurnTypeOrder solveForMultithreading(Cube const & cube, SolveAttributes solveAttributes, std::vector<unsigned int> const & initialVectorOfTurnNumbers, MutexProtectedClass<bool>& solutionFound);
	static TurnTypeOrder solveOneOfSeveralComparators(Cube const & cube, SolveAttributes solveAttributesWithComparatorBase, std::list<std::pair<Comparator, F2LPairTargetPos>> const & listOfExtraComparatorPairs, std::vector<unsigned int> const & initialVectorOfTurnNumbers,  F2LPairTargetPos& solvedPairPosReturn, std::ostream & oStream = std::cout);
public:
	static TurnTypeOrder quicksolve(Cube const & cube, bool giveOutputOnTerminal);
	static bool solveWhiteCross(Cube const & c, std::ostream & log, TurnTypeOrder& t);
	static bool solveF2L(Cube const & c, std::ostream & log, TurnTypeOrder& t);
	static bool solveF2LusingPairOrder(Cube const & c, std::ostream & log, TurnTypeOrder& t, std::array<F2LPairTargetPos, 4> const & pairOrder);
	static void solveF2LusingPairOrderForMultithreading(Cube const & c, TurnTypeOrder& t, std::array<F2LPairTargetPos, 4> const & pairOrder, MutexProtectedClass<bool>& solutionFound, bool& hasThisFunctionFoundTheSolution);
	static bool solveF2LMultithreaded(Cube const & c, std::ostream & log, TurnTypeOrder& t);
	static bool solveF2LByCheckingMultiplePairsAtOnce(Cube const & c, std::ostream & log, TurnTypeOrder& t);
	static bool solveF2LByCheckingMultiplePairsAtOnceAndFirstInsertingCorners(Cube const & c, std::ostream & log, TurnTypeOrder& t);
	static bool solveOLL(Cube const & c, std::ostream & log, TurnTypeOrder& t);
	static bool solvePLL(Cube const & c, std::ostream & log, TurnTypeOrder& t);

	static bool tryTurnsToSolve(Cube const & c, Comparator const & comparator, std::list<TurnTypeOrder const *> const & listOfTurns, std::list<TurnTypeOrder> const & listOfSetupMoves, TurnTypeOrder& turnsReturn);
private:
	static SolveAttributes askForUserInput();
	static InterruptProperties askForInterruptPropertiesUserInput();
	static bool askIfUserWantsToContinueSolving(InterruptProperties& newInterruptProperties);
	static std::string demandUserInput(std::string const & message, std::list<std::string> listOfValidStrings);

private:
	static void createNextVectorOfTurnNumbersForSolving(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged);
	static void createNextVectorOfTurnNumbersForSolvingSmart(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged);
	static void createNextVectorOfTurnNumbersForSolvingSmarter(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged);
	static void handleOverflowOfTurnNumbersVec(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged, bool& vecChanged);
	static void skipHiddenInverses(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged, bool& vecChanged);
	static void skipEqualNumbers(std::vector<unsigned int>& vecOfTurnNumbers, bool useOnlyQuarterTurns, bool& turnNumberVecSizeChanged, bool& vecChanged);
	static void fillVectorWithZerosFrom(std::vector<unsigned int>& vec, unsigned int index);
	static TurnTypeOrder trafoVecOfTurnNumInTurnTypeOrder(std::vector<unsigned int> const & vec, bool useOnlyQuarterTurns);


};







#endif //SOLVER_HPP

