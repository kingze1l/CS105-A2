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
    for (auto* enrolment : enrolments) {
        enrolment->generateReport();
    }
    std::cout << "Enter any key to logout: ";
    std::string tmp;
    std::cin >> tmp;
}