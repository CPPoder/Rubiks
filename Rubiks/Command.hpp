#ifndef COMMAND_HPP
#define COMMAND_HPP


#include <string>


class Command
{
public:
	enum class Type
	{
		Invalid,
		DoNothing,
		Solve,
		Exit,
		CreateCube,
		CreateTurns,
		Echo,
		Assign,
		Clear,
		Turn,
		Input,
		Quicksolve,
		Help
	};

	struct InvalidSpecification
	{
		std::string message;

		InvalidSpecification() = default;
		InvalidSpecification(std::string const & _message);
	};

	struct SolveSpecification
	{
		std::string cube;
		std::string writeTo;

		SolveSpecification() = default;
		SolveSpecification(std::string const & _cube, std::string const & _writeTo);
	};

	struct CreateCubeSpecification
	{
		std::string name;

		CreateCubeSpecification() = default;
		CreateCubeSpecification(std::string const & _name);
	};

	struct CreateTurnsSpecification
	{
		std::string name;

		CreateTurnsSpecification() = default;
		CreateTurnsSpecification(std::string const & _name);
	};

	struct EchoSpecification
	{
		std::string name;

		EchoSpecification() = default;
		EchoSpecification(std::string const & _name);
	};

	struct AssignSpecification
	{
		std::string from;
		std::string to;

		AssignSpecification() = default;
		AssignSpecification(std::string const & _from, std::string const & _to);
	};

	struct TurnSpecification
	{
		std::string what;
		std::string turn;

		TurnSpecification() = default;
		TurnSpecification(std::string const & _what, std::string const & _turn);
	};

	struct InputSpecification
	{
		std::string name;

		InputSpecification() = default;
		InputSpecification(std::string const & _name);
	};

	struct QuicksolveSpecification
	{
		std::string cube;
		std::string writeTo;

		QuicksolveSpecification() = default;
		QuicksolveSpecification(std::string const & _cube, std::string const & _writeTo);
	};

	struct HelpSpecification
	{
		std::string message;

		HelpSpecification() = default;
		HelpSpecification(std::string const & _message);

	};


private:
	Type mType;

	InvalidSpecification invalidSpecification;
	SolveSpecification solveSpecification;
	CreateCubeSpecification createCubeSpecification;
	CreateTurnsSpecification createTurnsSpecification;
	EchoSpecification echoSpecification;
	AssignSpecification assignSpecification;
	TurnSpecification turnSpecification;
	InputSpecification inputSpecification;
	QuicksolveSpecification quicksolveSpecification;
	HelpSpecification helpSpecification;

public:
	Command();
	~Command();
	Command(Command const & c);
	Command& operator=(Command const & c);

public:
	Type getType() const;
	InvalidSpecification getInvalidSpecification() const;
	SolveSpecification getSolveSpecification() const;
	CreateCubeSpecification getCreateCubeSpecification() const;
	CreateTurnsSpecification getCreateTurnsSpecification() const;
	EchoSpecification getEchoSpecification() const;
	AssignSpecification getAssignSpecification() const;
	TurnSpecification getTurnSpecification() const;
	InputSpecification getInputSpecification() const;
	QuicksolveSpecification getQuicksolveSpecification() const;
	HelpSpecification getHelpSpecification() const;

public:
	void setInvalid(InvalidSpecification const & invalidSpecification);
	void setSolve(SolveSpecification const & solveSpecification);
	void setExit();
	void setDoNothing();
	void setCreateCube(CreateCubeSpecification const & createCubeSpecification);
	void setCreateTurns(CreateTurnsSpecification const & createTurnsSpecification);
	void setEcho(EchoSpecification const & echoSpecification);
	void setAssign(AssignSpecification const & assignSpecification);
	void setClear();
	void setTurn(TurnSpecification const & turnSpecification);
	void setInput(InputSpecification const & inputSpecification);
	void setQuicksolve(QuicksolveSpecification const & quicksolveSpecification);
	void setHelp(HelpSpecification const & helpSpecification);

private:
	void clearSpecifications();
	static void assignRightCommandToLeftCommand(Command & l, Command const & r);

public:
	static const Command EXIT;
	static const Command DO_NOTHING;
	static const Command INVALID;
	static const Command CLEAR;


};









#endif //COMMAND_HPP

