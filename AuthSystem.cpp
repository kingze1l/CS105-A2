#include "AuthSystem.h"
#include "Admin.h"
#include "Teacher.h"
#include "StudentUser.h"
#include <iostream>

AuthSystem::AuthSystem() {
    // Create a test student first
    Student testStudent(1, "John Doe", "123 Main St", "555-1234");

    // Add default users
    users.push_back(std::make_unique<Admin>("admin", "admin123"));
    users.push_back(std::make_unique<Teacher>("teacher", "teacher123", "deafualt teacher"));
    users.push_back(std::make_unique<StudentUser>("student1", "student123", testStudent));
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