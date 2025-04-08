// Teacher.cpp
#include "Teacher.h"
#include <iostream>

Teacher::Teacher(const std::string& uname, const std::string& pwd)
    : User(uname, pwd, "teacher") {
}

void Teacher::showMenu() {
    int choice;
    Student testStudent(101, "Test Student", "Test Address", "123456");  // Temporary
    Course testCourse("Math", "M101", "Mathematics", "Mr. Smith");       // Temporary
    Enrolment currentEnrolment(testStudent, testCourse);                 // Temporary

    do {
        std::cout << "\n=== TEACHER MENU ===\n"
            << "1. Input grades\n"
            << "2. Exit\n"
            << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            gradeAssignment(currentEnrolment);
            currentEnrolment.generateReport();  // Show the result
        }
    } while (choice != 2);
}
// In Teacher.cpp
void Teacher::gradeAssignment(Enrolment& enrolment) {
    float internal, final;

    std::cout << "\nGrading for: " << enrolment.getStudent().getName()
        << " in " << enrolment.getCourse().getCourseName() << "\n";

    std::cout << "Enter internal marks (0-100): ";
    std::cin >> internal;

    std::cout << "Enter final marks (0-100): ";
    std::cin >> final;

    PrimaryGrade* grade = new PrimaryGrade();
    grade->setInternalMark(internal);
    grade->setFinalMark(final);
    enrolment.addGrade(grade);

    std::cout << "Grades submitted successfully!\n";
};