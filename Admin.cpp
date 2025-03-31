#include "Admin.h"
#include "Student.h"
#include "Course.h"
#include <iostream>

Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd, "admin") {
}

void Admin::showMenu() {
    printAdminHeader();
    std::cout << "1. Create Student Account\n"
        << "2. Create Course\n"
        << "3. View All Users\n"
        << "4. Exit\n"
        << "Enter your choice: ";
}
void Admin::createCourse(std::vector<Course>& courses) {
    std::string name, code, description, teacher;

    std::cout << "\n--- Create New Course ---\n";
    std::cout << "Course Name: ";
    std::getline(std::cin, name);

    std::cout << "Course Code: ";
    std::getline(std::cin, code);

    std::cout << "Description: ";
    std::getline(std::cin, description);

    std::cout << "Teacher: ";
    std::getline(std::cin, teacher);

    courses.emplace_back(name, code, description, teacher);
    std::cout << "Course created successfully!\n";
}
void Admin::createStudent(std::vector<Student>& students) {
    std::string name, address, contact;
    int rollno;

    std::cout << "\n---- Create Student Account ----\n";
    std::cout << "Enter Roll No: "; std::cin >> rollno;
    std::cin.ignore();

    std::cout << "Enter Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Address: ";
    std::getline(std::cin, address);

    std::cout << "Enter Contact: ";
    std::getline(std::cin, contact);

    students.emplace_back(rollno, name, address, contact);
    std::cout << "Student account created successfully!\n";
}

void Admin::ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const {
    printAdminHeader();
    std::cout << "---- System Users ----\n";

    for (const auto& user : users) {
        std::cout << "- " << user->getUsername()
            << " (" << user->getRole() << ")\n";
    }
}

void Admin::printAdminHeader() const {
    std::cout << "\n---- Admin Menu ----\n";
    std::cout << "Welcome, " << getUsername() << "!\n";
}