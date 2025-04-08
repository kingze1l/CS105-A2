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
};