#include "User.h"
#include <iostream>

User::User(const std::string& uanme, const std::string& pwd, const std::string& r)
    : username(uanme), password(pwd), role(r) {
    std::cout << "User created: " << username << "( " << role << ")\n";
}

bool User::authenticate(const std::string& input) const {
    return input == password;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getRole() const {
    return role;
}