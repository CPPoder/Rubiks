#ifndef USER_HPP
#define USER_HPP


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

#include "Record.hpp"


class User
{
private:
	std::string name;
	std::string dataPath;
	std::list<Record> listOfRecords;

public:
	User() = delete;
	User(std::string const & _name, std::string const & _dataPath);

public:
	std::string getName() const;
	std::string getDataPath() const;
	std::list<Record>::const_iterator getBeginOfRecords() const;
	std::list<Record>::const_iterator getEndOfRecords() const;
	std::list<Record> const & getListOfRecords() const;

public:
	void addRecord(Record const & record);
	void eraseLastRecord();
	void saveRecords() const;
	void createEmptyRecordFile() const;

private:
	bool loadRecords();
	
private:
	static std::string generateDataPath(std::string const & name);
	static bool checkIfNameIsUnique(std::string const & name);
	static void saveUsers(std::vector<User> const & vecOfUsers);
	
public:
	static std::vector<User> loadUsers();
	static bool registerUser(std::string const & name);
	static bool eraseUser(std::string const & name);


};



#endif //USER_HPP

