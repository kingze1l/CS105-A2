#include "Teacher.h"
#include <iostream>
#include <vector>
#include "Enrolment.h"
#include "Course.h"
#include "Student.h"

Teacher::Teacher(const std::string& uname, const std::string& pwd, const std::string& name)
    : User(uname, pwd, "teacher"), fullName(name) {
}

std::string Teacher::getTeacherName() const { return fullName; }

void Teacher::showAssignedCourses(const std::vector<Course>& courses) const {
    std::cout << "\nYour Courses:\n";
    bool hasCourses = false;
    for (const auto& course : courses) {
        if (course.getTeacherUsername() == getUsername()) {
            std::cout << "- " << course.getCourseName() << " (" << course.getCourseCode() << ")\n";
            hasCourses = true;
        }
    }
    if (!hasCourses) {
        std::cout << "No courses assigned to you yet.\n";
    }
}

void Teacher::showMenu() {
    std::cout << "\n=== TEACHER MENU ===\n"
        << "1. View Assigned Courses\n"
        << "2. Input Grades\n"
        << "3. Exit\n"
        << "Choice: ";
}

void Teacher::gradeAssignment(Enrolment& enrolment) {
    float internal, final;

    std::cout << "\nGrading for: " << enrolment.getStudent().getName()
        << " in " << enrolment.getCourse().getCourseName() << "\n";

    std::cout << "Enter internal marks (0-100): ";
    std::cin >> internal;
    std::cout << "Enter final marks (0-100): ";
    std::cin >> final;
    std::cin.ignore();

    if (internal < 0 || internal > 100 || final < 0 || final > 100) {
        std::cout << "Error: Marks must be between 0 and 100.\n";
        return;
    }

    PrimaryGrade* grade = new PrimaryGrade();
    grade->setInternalMark(internal);
    grade->setFinalMark(final);
    enrolment.addGrade(grade);

    std::cout << "Grades submitted successfully!\n";
}