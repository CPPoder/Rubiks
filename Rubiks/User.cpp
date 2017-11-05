#include "User.hpp"

#include <cstdio>


User::User(std::string const & _name, std::string const & _dataPath)
	: name(_name), dataPath(_dataPath)
{
	if (!this->loadRecords())
	{
		this->createEmptyRecordFile();
		this->loadRecords();
	}
}


std::string User::getName() const
{
	return name;
}


std::string User::getDataPath() const
{
	return dataPath;
}


std::list<Record>::const_iterator User::getBeginOfRecords() const
{
	return listOfRecords.cbegin();
}


std::list<Record>::const_iterator User::getEndOfRecords() const
{
	return listOfRecords.cend();
}


std::list<Record> const & User::getListOfRecords() const
{
	return listOfRecords;
}


TimePair User::getAverageOverLast(unsigned int number) const
{
	//Handle special cases
	if (number == 0u)
	{
		return TimePair(0, 0);
	}
	if (listOfRecords.empty())
	{
		return TimePair(0, 0);
	}

	//Calculate average
	float sum = 0.f;
	unsigned int counter = 0u;
	for (std::list<Record>::const_reverse_iterator it = listOfRecords.crbegin(); (it != listOfRecords.crend() && counter < number); ++it, ++counter)
	{
		sum += it->getTime().getTimeAsFloatInSeconds();
	}
	float average = sum / counter;
	return TimePair::trafoSecondsIntoTimePair(average);
}


TimePair User::getAverage() const
{
	//Handle special cases
	if (listOfRecords.empty())
	{
		return TimePair(0, 0);
	}

	//Calculate average
	float sum = 0.f;
	for (auto const & rec : listOfRecords)
	{
		sum += rec.getTime().getTimeAsFloatInSeconds();
	}
	float average = sum / listOfRecords.size();
	return TimePair::trafoSecondsIntoTimePair(average);
}


Record User::getBestRecord() const
{
	Record best(Date::getCurrentDate(), TimePair(0, 0), {});
	bool bestIsInitiatilzed = false;
	for (auto const & rec : listOfRecords)
	{
		if (bestIsInitiatilzed)
		{
			if (rec.getTime() < best.getTime())
			{
				best = rec;
			}
		}
		else
		{
			bestIsInitiatilzed = true;
			best = rec;
		}
	}
	return std::move(best);
}


void User::addRecord(Record const & record)
{
	listOfRecords.push_back(record);
}


void User::eraseLastRecord()
{
	if (!listOfRecords.empty())
	{
		listOfRecords.pop_back();
	}
}


bool User::loadRecords()
{
	//Open fileStream
	std::ifstream fileStream(dataPath);
	if (!fileStream)
	{
		return false;
	}

	//Extract lines
	std::vector<std::string> lines;
	while (!fileStream.eof())
	{
		std::string line;
		std::getline(fileStream, line);
		lines.push_back(line);
	}
	fileStream.close();

	//Extract Records from lines
	std::list<Record> listOfRecords;
	for (auto const & line : lines)
	{
		if (line.empty())
		{
			continue;
		}
		std::stringstream stringstream(line);
		std::string date;
		std::string time;
		std::string scramble;
		stringstream >> date >> time >> scramble;
		listOfRecords.push_back(Record::makeRecordFromFileStrings(date, time, scramble));
	}

	//Return listOfRecords
	this->listOfRecords = listOfRecords;
	return true;
}


void User::saveRecords() const
{
	//Open file stream
	std::ofstream fileStream(dataPath, std::ofstream::trunc);
	if (!fileStream)
	{
		std::cout << "User::saveRecords() could not open file " << dataPath << "!" << std::endl;
	}

	//Write into file stream
	for (auto const & record : listOfRecords)
	{
		fileStream << Record::makeDateStringFromRecord(record) << '\t' << Record::makeTimeStringFromRecord(record) << '\t' << Record::makeScrambleStringFromRecord(record) << std::endl;
	}
	fileStream.close();
}


void User::createEmptyRecordFile() const
{
	std::ofstream outputStream(this->getDataPath());
	outputStream.close();
}


std::string User::generateDataPath(std::string const & name)
{
	return ("./Data/Records/" + name + ".usr");
}


bool User::checkIfNameIsUnique(std::string const & name)
{
	std::vector<User> vecOfUsers = User::loadUsers();
	for (auto const & user : vecOfUsers)
	{
		if (user.name == name)
		{
			return false;
		}
	}
	return true;
}


void User::saveUsers(std::vector<User> const & vecOfUsers)
{
	//Create .rec file
	std::ofstream outputStream("./Data/Records/users.rec", std::ofstream::trunc);
	if (!outputStream)
	{
		std::cout << "User::saveUsers(std::vector<User> const & vecOfUsers) could not open users.rec file!" << std::endl;
	}
	for (auto const & user : vecOfUsers)
	{
		outputStream << user.name << '\t' << user.dataPath << std::endl;
	}
	outputStream.close();
}


bool User::registerUser(std::string const & name)
{
	if (User::checkIfNameIsUnique(name))
	{
		User user(name, User::generateDataPath(name));
		std::vector<User> vecOfUsers = User::loadUsers();
		vecOfUsers.push_back(user);
		User::saveUsers(vecOfUsers);
		return true;
	}
	else
	{
		return false;
	}
}


bool User::eraseUser(std::string const & name)
{
	if (User::checkIfNameIsUnique(name)) //Check if there exist such a user name
	{
		return false;
	}
	else //If it exist, erase it
	{
		//Erase user
		std::vector<User> vecOfUsers = User::loadUsers();
		unsigned int posOfUserToErase;
		for (unsigned int i = 0; i < vecOfUsers.size(); ++i)
		{
			if (vecOfUsers.at(i).name == name)
			{
				posOfUserToErase = i;
				break;
			}
		}
		std::vector<User> newVecOfUsers;
		std::string usersDataPath;
		for (unsigned int i = 0; i < vecOfUsers.size(); ++i)
		{
			if (i == posOfUserToErase)
			{
				usersDataPath = vecOfUsers.at(i).getDataPath();
				continue;
			}
			newVecOfUsers.push_back(vecOfUsers.at(i));
		}
		User::saveUsers(newVecOfUsers);

		//Delete the corresponding .usr file
		std::remove(usersDataPath.c_str());

		return true;
	}
}


std::vector<User> User::loadUsers()
{
	//Open input file stream
	std::ifstream inputStream("./Data/Records/users.rec");
	if (!inputStream)
	{
		std::cout << "User::loadUsers() could not open users.rec file!" << std::endl;
	}

	//Extract lines
	std::vector<std::string> vectorOfLines;
	while (!inputStream.eof())
	{
		std::string str;
		std::getline(inputStream, str);
		vectorOfLines.push_back(str);
	}
	inputStream.close();

	//Get users
	std::vector<User> vecOfUsers;
	for (auto line : vectorOfLines)
	{
		if (line.empty())
		{
			continue;
		}
		std::stringstream stream(line);
		std::string name;
		std::string path;
		stream >> name >> path;
		vecOfUsers.push_back(User(name, path));
	}
	
	//Return vecOfUsers
	return std::move(vecOfUsers);
}

