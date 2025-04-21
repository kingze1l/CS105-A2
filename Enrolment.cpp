#include "Enrolment.h"
#include <ctime>
#include <iomanip>
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
void Enrolment::generateReport(std::ofstream& outFile) const {
    // Get current timestamp
    std::time_t now = std::time(nullptr);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now);
    std::string timestamp(buffer);
    timestamp.pop_back(); // Remove trailing newline

    // Console output
    std::cout << "\n=== Grade Report ===\n";
    std::cout << "Generated: " << timestamp << "\n";
    std::cout << "Student: " << student.getName() << " (Roll No: " << student.getRollno() << ")\n";
    std::cout << "Course: " << course.getCourseName() << " (" << course.getCourseCode() << ")\n";
    if (grade) {
        std::cout << "Internal Marks: " << grade->getInternalMark() << "\n";
        std::cout << "Final Marks: " << grade->getFinalMark() << "\n";
        std::cout << "Total Grade: " << grade->calculateGrade() << "\n";
    }
    else {
        std::cout << "No grades assigned yet.\n";
    }
    std::cout << "===================\n";

    // File output
    outFile << "=== Grade Report ===\n";
    outFile << "Generated: " << timestamp << "\n";
    outFile << "Student: " << student.getName() << " (Roll No: " << student.getRollno() << ")\n";
    outFile << "Course: " << course.getCourseName() << " (" << course.getCourseCode() << ")\n";
    if (grade) {
        outFile << "Internal Marks: " << grade->getInternalMark() << "\n";
        outFile << "Final Marks: " << grade->getFinalMark() << "\n";
        outFile << "Total Grade: " << grade->calculateGrade() << "\n";
    }
    else {
        outFile << "No grades assigned yet.\n";
    }
    outFile << "===================\n\n";
}