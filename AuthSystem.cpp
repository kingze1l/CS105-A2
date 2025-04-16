#include "AuthSystem.h"
#include "Admin.h"
#include "Teacher.h"
#include "StudentUser.h"
#include <iostream>
#include <fstream>

AuthSystem::AuthSystem() {
    // Check if users.txt exists and is not empty
    std::ifstream file("users.txt");
    if (file.good()) {
        std::string line;
        if (std::getline(file, line) && !line.empty()) {
            // File exists and has data, don't create default users
            file.close();
            return;
        }
    }
    file.close();

    // If users.txt doesn't exist or is empty, create default users
    users.push_back(std::make_unique<Admin>("admin", "admin123"));
    users.push_back(std::make_unique<Teacher>("teacher", "teacher123", "deafualt teacher"));
}


User* AuthSystem::login(const std::string& username, const std::string& password) {
    for (auto& user : users) {
        if (user->getUsername() == username && user->authenticate(password)) {
            std::cout << "Logged in as: " << user->getRole() << std::endl;

            // Handle student-specific menu
            if (user->getRole() == "student") {
                StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
                if (studentUser) {
                    studentUser->showMenu();
                }
            }
            return user.get();
        }
    }
    return nullptr;
}