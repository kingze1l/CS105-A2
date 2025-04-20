// AuthSystem.h
#pragma once
#include "User.h"
#include <vector>
#include <memory>

class AuthSystem {
    std::vector<std::unique_ptr<User>> users;
public:
    AuthSystem();
    User* login(const std::string& username, const std::string& password);
    const std::vector<std::unique_ptr<User>>& getUsers() const { return users; }
    void addUser(std::unique_ptr<User> user);
    void clearUsers();
    bool removeUser(const std::string& username);
};