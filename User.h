#pragma once
#include <string>
#include <memory>
#include <iostream>

class User {
protected:
    std::string username;
    std::string password;
    std::string role;

public:
    User(const std::string& uanme, const std::string& pwd, const std::string& r);
    virtual ~User() = default;
    virtual void showMenu() = 0;
    bool authenticate(const std::string& input) const;

    std::string getUsername() const; // Declaration only
    std::string getRole() const;     // Declaration only
    std::string getPassword() const { return password; } 
    
	// Setters
	void setUsername(const std::string& uname);
	void setPassword(const std::string& pwd);


    User(const User&) = delete;
    User& operator=(const User&) = delete;
};