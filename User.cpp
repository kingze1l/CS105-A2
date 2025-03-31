#include "User.h"
#include <iostream>


User::User(const std::string& uanme, const std::string& pwd, const std::string& r)
	: username(uanme), password(pwd), role(r) {
	std::cout << "User created: " << username << "( " << role << ")\n";   //constructor implementation for User
}

//authenticate the user
bool User::authenticate(const std::string& input) const {
	return input == password;  //maybe ill add hashing later
}
//getters for the username, password and role
std::string User::getUsername() const {
	return username;
}

std::string User::getRole() const {
	return role;
} //end of User.cpp