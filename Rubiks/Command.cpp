#include "Command.hpp"


Command::InvalidSpecification::InvalidSpecification(std::string const & _message)
	: message(_message)
{
}

Command::SolveSpecification::SolveSpecification(std::string const & _cube, std::string const & _writeTo)
	: cube(_cube), writeTo(_writeTo)
{
}

Command::CreateCubeSpecification::CreateCubeSpecification(std::string const & _name)
	: name(_name)
{
}

Command::CreateTurnsSpecification::CreateTurnsSpecification(std::string const & _name)
	: name(_name)
{
}

Command::EchoSpecification::EchoSpecification(std::string const & _name)
	: name(_name)
{
}

Command::AssignSpecification::AssignSpecification(std::string const & _from, std::string const & _to)
	: from(_from), to(_to)
{
}

Command::TurnSpecification::TurnSpecification(std::string const & _what, std::string const & _turn)
	: what(_what), turn(_turn)
{
}

Command::InputSpecification::InputSpecification(std::string const & _name)
	: name(_name)
{
}

Command::QuicksolveSpecification::QuicksolveSpecification(std::string const & _cube, std::string const & _writeTo)
	: cube(_cube), writeTo(_writeTo)
{
}




Command::Command()
{
	InvalidSpecification spec;
	spec.message = "Internal Error: Command used, although not active! (Union Type)";
	this->setInvalid(spec);
}

Command::~Command()
{
}

Command::Command(Command const & c)
{
	Command::assignRightCommandToLeftCommand(*this, c);
}

Command& Command::operator=(Command const & c)
{
	Command::assignRightCommandToLeftCommand(*this, c);
	return *this;
}





Command::Type Command::getType() const
{
	return mType;
}




Command::InvalidSpecification Command::getInvalidSpecification() const
{
	return this->invalidSpecification;
}

Command::SolveSpecification Command::getSolveSpecification() const
{
	return this->solveSpecification;
}

Command::CreateCubeSpecification Command::getCreateCubeSpecification() const
{
	return this->createCubeSpecification;
}

Command::CreateTurnsSpecification Command::getCreateTurnsSpecification() const
{
	return this->createTurnsSpecification;
}

Command::EchoSpecification Command::getEchoSpecification() const
{
	return this->echoSpecification;
}

Command::AssignSpecification Command::getAssignSpecification() const
{
	return this->assignSpecification;
}

Command::TurnSpecification Command::getTurnSpecification() const
{
	return this->turnSpecification;
}

Command::InputSpecification Command::getInputSpecification() const
{
	return this->inputSpecification;
}

Command::QuicksolveSpecification Command::getQuicksolveSpecification() const
{
	return this->quicksolveSpecification;
}




void Command::setInvalid(InvalidSpecification const & invalidSpecification)
{
	mType = Type::Invalid;
	this->clearSpecifications();
	this->invalidSpecification = invalidSpecification;
}

void Command::setSolve(SolveSpecification const & solveSpecification)
{
	mType = Type::Solve;
	this->clearSpecifications();
	this->solveSpecification = solveSpecification;
}

void Command::setExit()
{
	mType = Type::Exit;
	this->clearSpecifications();
}

void Command::setDoNothing()
{
	mType = Type::DoNothing;
	this->clearSpecifications();
}

void Command::setCreateCube(CreateCubeSpecification const & createCubeSpecification)
{
	mType = Type::CreateCube;
	this->clearSpecifications();
	this->createCubeSpecification = createCubeSpecification;
}

void Command::setCreateTurns(CreateTurnsSpecification const & createTurnsSpecification)
{
	mType = Type::CreateTurns;
	this->clearSpecifications();
	this->createTurnsSpecification = createTurnsSpecification;
}

void Command::setEcho(EchoSpecification const & echoSpecification)
{
	mType = Type::Echo;
	this->clearSpecifications();
	this->echoSpecification = echoSpecification;
}

void Command::setAssign(AssignSpecification const & assignSpecification)
{
	mType = Type::Assign;
	this->clearSpecifications();
	this->assignSpecification = assignSpecification;
}

void Command::setClear()
{
	mType = Type::Clear;
	this->clearSpecifications();
}

void Command::setTurn(TurnSpecification const & turnSpecification)
{
	mType = Type::Turn;
	this->clearSpecifications();
	this->turnSpecification = turnSpecification;
}

void Command::setInput(InputSpecification const & inputSpecification)
{
	mType = Type::Input;
	this->clearSpecifications();
	this->inputSpecification = inputSpecification;
}

void Command::setQuicksolve(QuicksolveSpecification const & quicksolveSpecification)
{
	mType = Type::Quicksolve;
	this->clearSpecifications();
	this->quicksolveSpecification = quicksolveSpecification;
}




void Command::clearSpecifications()
{
	this->invalidSpecification.message = "";
	this->solveSpecification = SolveSpecification("", "");
	this->createCubeSpecification.name = "";
	this->createTurnsSpecification.name = "";
	this->echoSpecification.name = "";
	this->assignSpecification = AssignSpecification("", "");
	this->turnSpecification = TurnSpecification("", "");
	this->inputSpecification.name = "";
	this->quicksolveSpecification = QuicksolveSpecification("", "");
}


void Command::assignRightCommandToLeftCommand(Command & l, Command const & r)
{
	switch (r.getType())
	{
	case Type::Invalid:
		l.setInvalid(r.getInvalidSpecification());
		break;
	case Type::Exit:
		l.setExit();
		break;
	case Type::Solve:
		l.setSolve(r.getSolveSpecification());
		break;
	case Type::DoNothing:
		l.setDoNothing();
		break;
	case Type::CreateCube:
		l.setCreateCube(r.getCreateCubeSpecification());
		break;
	case Type::CreateTurns:
		l.setCreateTurns(r.getCreateTurnsSpecification());
		break;
	case Type::Echo:
		l.setEcho(r.getEchoSpecification());
		break;
	case Type::Assign:
		l.setAssign(r.getAssignSpecification());
		break;
	case Type::Clear:
		l.setClear();
		break;
	case Type::Turn:
		l.setTurn(r.getTurnSpecification());
		break;
	case Type::Input:
		l.setInput(r.getInputSpecification());
		break;
	case Type::Quicksolve:
		l.setQuicksolve(r.getQuicksolveSpecification());
		break;
	}
}







const Command Command::EXIT = []() -> Command {
	Command cmd;
	cmd.setExit();
	return cmd;
}();

const Command Command::DO_NOTHING = []() -> Command {
	Command cmd;
	cmd.setDoNothing();
	return cmd;
}();

const Command Command::INVALID = []() -> Command {
	Command cmd;
	cmd.setInvalid(InvalidSpecification("Command is invalid!"));
	return cmd;
}();

const Command Command::CLEAR = []() -> Command {
	Command cmd;
	cmd.setClear();
	return cmd;
}();



