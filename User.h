#pragma once
#include <string>
#include <memory>  // For std::unique_ptr or smart pointers
#include <iostream> // For std::cout

class User {
protected:
	std::string username;
	std::string password;
	std::string role;  // admin, teacher  or user

public:
	//constructor for User
	User(const std::string& uanme, const std::string& pwd, const std::string& r);

	//virtual destructor for the proper cleanup of the memory
	virtual ~User() = default;

	virtual void showMenu() = 0; // pure virtual function to implement in derived classes ie. child classes

	bool authenticate(const std::string& input) const; // authenticate the user 

	//getters for the username, password and role
	std::string getUsername() const;
	std::string getRole() const;

	User(const User&) = delete; //copy constructor deleted 
	User& operator=(const User&) = delete; //copy assignment operator deleted because we don't want to copy the user object


};
