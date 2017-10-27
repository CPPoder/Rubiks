#include "Framework.hpp"
#include <cstdlib>


Framework::Framework()
{
	this->outputASCIIArt();
	std::cout << "-----Welcome to Rubiks-----" << std::endl;
	
	//This was my initial test
	Cube c;
	c.turn(TurnType::Left + TurnType::Back + TurnType::Down + TurnType::Back2 + TurnType::Left + TurnType::UpInverse + TurnType::Left + TurnType::Back + TurnType::Right + TurnType::FrontInverse);
	//Solver::quicksolve(c);
}


Framework::~Framework()
{
	this->clearCubeMap();
}


void Framework::run()
{
	while (mRun)
	{
		Command cmd = Parser::waitForInput();
		switch (cmd.getType())
		{
		case Command::Type::DoNothing:
			break;
		case Command::Type::Exit:
			this->terminate();
			break;
		case Command::Type::Invalid:
			std::cout << cmd.getInvalidSpecification().message << std::endl;
			break;
		case Command::Type::Solve:
			this->solveCube(cmd.getSolveSpecification().cube, cmd.getSolveSpecification().writeTo);
			break;
		case Command::Type::Quicksolve:
			this->quicksolveCube(cmd.getQuicksolveSpecification().cube, cmd.getQuicksolveSpecification().writeTo);
			break;
		case Command::Type::CreateCube:
			if (!this->addCube(cmd.getCreateCubeSpecification().name))
			{
				std::cout << "Cube name already exists!" << std::endl;
			}
			break;
		case Command::Type::CreateTurns:
			if (!this->addTurns(cmd.getCreateTurnsSpecification().name))
			{
				std::cout << "Turn name already exists!" << std::endl;
			}
			break;
		case Command::Type::Echo:
			if (!this->echoCubeOrTurns(cmd.getEchoSpecification().name))
			{
				std::cout << "There is no Cube or Turns object named " << cmd.getEchoSpecification().name << "!" << std::endl;
			}
			break;
		case Command::Type::Assign:
			this->assignCubeOrTurns(cmd.getAssignSpecification().from, cmd.getAssignSpecification().to);
			break;
		case Command::Type::Clear:
			this->clear();
			break;
		case Command::Type::Turn:
			this->turn(cmd.getTurnSpecification().what, cmd.getTurnSpecification().turn);
			break;
		case Command::Type::Input:
			this->inputCube(cmd.getInputSpecification().name);
			break;
		case Command::Type::Help:
			std::cout << cmd.getHelpSpecification().message;
			break;
		case Command::Type::Show:
			if (!this->showCube(cmd.getShowSpecification().name))
			{
				std::cout << "There is no Cube object named " << cmd.getShowSpecification().name << "!" << std::endl;
			}
			break;
		case Command::Type::Timer:
			Timer::startSubprogram();
			break;
		}
	}
}

void Framework::terminate()
{
	mRun = false;
}

bool Framework::addCube(std::string const & name)
{
	if ((mMapOfCubePointers.count(name) == 1) || (mMapOfTurns.count(name) == 1))
	{
		return false;
	}
	else
	{
		Cube* cube = new Cube;
		mMapOfCubePointers.insert(std::make_pair(name, cube));
		return true;
	}
}

bool Framework::addTurns(std::string const & name)
{
	if ((mMapOfCubePointers.count(name) == 1) || (mMapOfTurns.count(name) == 1))
	{
		return false;
	}
	else
	{
		mMapOfTurns.insert(std::make_pair(name, TurnTypeOrder()));
		return true;
	}
}

void Framework::clear()
{
	this->clearCubeMap();
	this->clearTurnsMap();
	std::system("cls");
}

void Framework::clearCubeMap()
{
	for (auto & pair : mMapOfCubePointers)
	{
		delete pair.second;
	}
	mMapOfCubePointers.clear();
}

void Framework::clearTurnsMap()
{
	mMapOfTurns.clear();
}

bool Framework::echoCubeOrTurns(std::string const & name)
{
	Cube* cube;
	if (this->findCubeNamed(name, cube))
	{
		std::cout << *cube;
		return true;
	}
	TurnTypeOrder turnTypeOrder;
	if (this->findTurnTypeOrderNamed(name, turnTypeOrder))
	{
		std::cout << turnTypeOrder << std::endl;
		return true;
	}
	return false;
}

bool Framework::assignCubeOrTurns(std::string const & from, std::string const & to)
{
	//Check if from == to
	if (from == to)
	{
		std::cout << "Don't try to crash this program!" << std::endl;
		return false;
	}

	//First, check if to is cube
	if (mMapOfCubePointers.count(to) == 1)
	{
		//Get toCube pointer
		Cube* toCube = mMapOfCubePointers.at(to);

		//Check if from is cube
		if (mMapOfCubePointers.count(from) == 1)
		{ //from is cube
			Cube* fromCube = mMapOfCubePointers.at(from);
			*toCube = *fromCube;
			return true;
		}
		else
		{ //from isn't cube
			//Check if from is solved
			if (from == "solved")
			{
				*toCube = Cube();
				return true;
			}
			else
			{
				std::cout << from << " is not convertible into cube!" << std::endl;
				return false;
			}
		}

	}
	
	//Then, check if to is turns
	if (mMapOfTurns.count(to) == 1)
	{
		//Get turns reference
		TurnTypeOrder& toTurns = mMapOfTurns.at(to);

		//Check if from is turns
		if (mMapOfTurns.count(from) == 1)
		{ //from is turns
			TurnTypeOrder& fromTurns = mMapOfTurns.at(from);
			toTurns = fromTurns;
			return true;
		}
		else
		{ //from isn't turns
			//Check if from is convertible into turns
			TurnTypeOrder turns;
			if (Turn::isStringConvertibleIntoTurns(from, turns))
			{
				toTurns = turns;
				return true;
			}
			else
			{
				std::cout << from << " is not convertible into turns!" << std::endl;
				return false;
			}
		}
	}

	//Return false if none is the case
	std::cout << to << " is neither cube nor turns variables name!" << std::endl;
	return false;
}

bool Framework::turn(std::string const & which, std::string const & turns)
{
	//Try to find cube which is called which
	if (mMapOfCubePointers.count(which) == 1)
	{ //Cube exists
		//Get cube
		Cube* cube = mMapOfCubePointers.at(which);

		//Try to find turns
		if (mMapOfTurns.count(turns) == 1)
		{ //Turns found
			//Get turns
			TurnTypeOrder turnTypeOrder = mMapOfTurns.at(turns);

			//Turn cube
			cube->turn(turnTypeOrder);
			return true;
		}
		else
		{ //Turns not found
			//Try to convert string into TurnTypeOrder
			TurnTypeOrder turnTypeOrder;
			if (Turn::isStringConvertibleIntoTurns(turns, turnTypeOrder))
			{ //is convertible
				//Turn cube
				cube->turn(turnTypeOrder);
				return true;
			}
			else
			{ //is not convertible
				std::cout << turns << " is not convertible into Turns" << std::endl;
				return false;
			}
		}

	}
	else
	{ //Cube doesn't exist
		std::cout << which << " is not a cube!" << std::endl;
		return false;
	}

}

bool Framework::inputCube(std::string const & name)
{
	//Find cube
	if (mMapOfCubePointers.count(name) == 1)
	{ //Cube found
		Cube* cube = mMapOfCubePointers.at(name);
		cube->askForConsoleInput();
		return true;
	}
	else
	{ //Cube not found
		std::cout << name << " is not a cube!" << std::endl;
		return false;
	}
}

bool Framework::solveCube(std::string const & cube, std::string const & writeTo)
{
	//Find cube
	if (mMapOfCubePointers.count(cube) == 1)
	{ //Cube found
		Cube* cubePointer = mMapOfCubePointers.at(cube);

		//Find turnsToWriteTo
		if (mMapOfTurns.count(writeTo) == 1)
		{ //Found turns
			Solver::solveManager(*cubePointer, mMapOfTurns.at(writeTo));
			return true;
		}
		else
		{ // Couldn't find turns
			std::cout << writeTo << " is not a turns!" << std::endl;
			return false;
		}
	}
	else
	{ //Cube not found
		std::cout << cube << " is not a cube!" << std::endl;
		return false;
	}
}


bool Framework::quicksolveCube(std::string const & cube, std::string const & writeTo)
{
	//Find cube
	if (mMapOfCubePointers.count(cube) == 1)
	{ //Cube found
		Cube* cubePointer = mMapOfCubePointers.at(cube);

		//Find turnsToWriteTo
		if (mMapOfTurns.count(writeTo) == 1)
		{ //Found turns
			try
			{
				mMapOfTurns.at(writeTo) = Solver::quicksolve(*cubePointer, true);
				return true;
			}
			catch (Cube::InvalidCubeException exception)
			{
				std::cout << "Cube is invalid!" << std::endl;
				std::cout << exception.report();
				std::cout << "Quicksolve canceled!" << std::endl;
				return false;
			}
		}
		else
		{ // Couldn't find turns
			std::cout << writeTo << " is not a turns!" << std::endl;
			return false;
		}
	}
	else
	{ //Cube not found
		std::cout << cube << " is not a cube!" << std::endl;
		return false;
	}
}

bool Framework::showCube(std::string const & name)
{
	Cube* cube;
	if (this->findCubeNamed(name, cube))
	{
		Renderer::showCube(*cube);
		return true;
	}
	return false;
}









bool Framework::findCubeNamed(std::string const & name, Cube*& cube)
{
	auto it = mMapOfCubePointers.find(name);
	if (it != mMapOfCubePointers.end())
	{
		cube = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

bool Framework::findTurnTypeOrderNamed(std::string const & name, TurnTypeOrder& turnTypeOrder)
{
	auto it = mMapOfTurns.find(name);
	if (it != mMapOfTurns.end())
	{
		turnTypeOrder = it->second;
		return true;
	}
	else
	{
		return false;
	}
}






void Framework::outputASCIIArt()
{
	std::ifstream stream("asciiArt.txt");
	std::stringstream stringstream;
	stringstream << stream.rdbuf();
	std::string string = stringstream.str();
	std::cout << string << std::endl;
	stream.close();
}

