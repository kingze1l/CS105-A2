#include "StudentUser.h"
#include "Enrolment.h"
#include <iostream>
#include <iomanip>

StudentUser::StudentUser(const std::string& uname, const std::string& pwd, const Student& s)
    : User(uname, pwd, "student"), student(s) {
}

void StudentUser::addEnrolment(Enrolment* enrolment) {
    enrolments.push_back(enrolment);
}

void StudentUser::loadEnrollments(const std::vector<Enrolment*>& allEnrollments) {
    enrolments.clear();
    for (auto* enrollment : allEnrollments) {
        if (enrollment->getStudent().getRollno() == student.getRollno()) {
            enrolments.push_back(enrollment);
        }
    }
}

void StudentUser::displayEnrolledCourses() const {
    std::cout << "==== Enrolled Courses ====\n";
    if (enrolments.empty()) {
        std::cout << "Not enrolled in any courses.\n";
        return;
    }
    for (const auto* enrolment : enrolments) {
        std::cout << "- " << enrolment->getCourse().getCourseName()
            << " (" << enrolment->getCourse().getCourseCode() << ")\n";
        auto grade = enrolment->getGrade();
        if (grade) {
            std::cout << "  Grade: Internal = " << grade->getInternalMark()
                << ", Final = " << grade->getFinalMark()
                << ", Total = " << grade->calculateGrade() << "\n";
        }
        else {
            std::cout << "  No grades available.\n";
        }
    }
}

void StudentUser::showMenu() {
    std::cout << "\n------ Student Portal ------\n";
    std::cout << "Logged in as: " << getUsername() << "\n";
    std::cout << "Student Name: " << student.getName() << "\n";
    std::cout << "Roll Number: " << student.getRollno() << "\n";
    std::cout << "Address: " << student.getAddress() << "\n";
    std::cout << "Contact: " << student.getContact() << "\n\n";

    displayEnrolledCourses();

    std::cout << "\nPress Enter to logout...";
    std::string tmp;
    std::getline(std::cin, tmp);
}