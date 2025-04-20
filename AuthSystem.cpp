#include "AuthSystem.h"
#include "Admin.h"
#include "Teacher.h"
#include "StudentUser.h"
#include <fstream>
#include <iostream>

AuthSystem::AuthSystem() {
    std::ifstream file("users.txt");
    if (!file.good() || file.peek() == std::ifstream::traits_type::eof()) {
        addUser(std::make_unique<Admin>("admin", "admin123"));
        addUser(std::make_unique<Teacher>("teacher", "teacher123", "Prof. Smith"));
    }
    file.close();
}

void AuthSystem::addUser(std::unique_ptr<User> user) {
    for (const auto& existingUser : users) {
        if (existingUser->getUsername() == user->getUsername()) {
            std::cout << "Error: Username already exists.\n";
            return;
        }
    }
    users.push_back(std::move(user));
}

void AuthSystem::clearUsers() {
    users.clear();
}

User* AuthSystem::login(const std::string& username, const std::string& password) {
    for (auto& user : users) {
        if (user->getUsername() == username && user->authenticate(password)) {
            std::cout << "Logged in as: " << user->getRole() << std::endl;
            return user.get();
        }
    }
    return nullptr;
}