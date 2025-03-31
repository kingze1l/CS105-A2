#include "Enrolment.h"
#include <iostream>

Enrolment::Enrolment(const Student& s, const Course& c)
    : student(s), course(c), grade(nullptr) {
}

Enrolment::~Enrolment() {
    if (grade) delete grade;
}

void Enrolment::addGrade(Grade* g) {
    if (grade) delete grade;
    grade = g;
}

void Enrolment::generateReport() const {
    std::cout << "Report for " << student.getName() << " in "
        << course.getCourseName() << ":\n";
    if (grade) {
        std::cout << "Final Grade: " << grade->calculateGrade() << "/100\n";
    }
    else {
        std::cout << "Grade not yet assigned!\n";
    }
}