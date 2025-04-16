#include "StudentUser.h"
#include "Enrolment.h"
#include <iostream>

StudentUser::StudentUser(const std::string& uname,
    const std::string& pwd,
    const Student& s)
    : User(uname, pwd, "student"), student(s) {
}

void StudentUser::addEnrolment(Enrolment* enrolment) {
    enrolments.push_back(enrolment);
}

void StudentUser::showMenu() {
    std::cout << "\n=== STUDENT PORTAL ===\n";
    std::cout << "Logged in as: " << getUsername() << "\n";
    std::cout << "Student Name: " << student.getName() << "\n";
    std::cout << "Roll Number: " << student.getRollno() << "\n";
    displayEnrolledCourses();
    for (auto* enrolment : enrolments) {
        enrolment->generateReport();
    }
    std::cout << "Enter any key to logout: ";
    std::string tmp;
    std::getline(std::cin, tmp);
}