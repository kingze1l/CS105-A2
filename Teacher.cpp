#include "Teacher.h"
#include <iostream>
#include <vector>
#include "Enrolment.h"

Teacher::Teacher(const std::string& uname, const std::string& pwd,
    const std::string& name)
    : User(uname, pwd, "teacher"), fullName(name) {
}

void Teacher::showAssignedCourses(const std::vector<Course>& courses) const {
    std::cout << "\nYour Courses:\n";
    bool hasCourses = false;
    for (const auto& course : courses) {
        if (course.getTeacherUsername() == getUsername()) {
            course.display();
            hasCourses = true;
        }
    }
    if (!hasCourses) {
        std::cout << "No courses assigned to you yet.\n";
    }
}

void Teacher::showMenu() {
    int choice;
    do {
        std::cout << "\n=== TEACHER MENU ===\n"
            << "1. View Assigned Courses\n"
            << "2. Input grades\n"
            << "3. Exit\n"
            << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            // Will need to pass courses vector from main
            // For now, we'll just show a message
            std::cout << "Feature coming soon!\n";
            break;
        case 2: {
            // Temporary solution - in real implementation, you'd:
            // 1. Show teacher's courses
            // 2. Let them select a course
            // 3. Show enrollments in that course
            // 4. Let them select an enrollment to grade
            std::cout << "Grade input feature coming soon!\n";
            break;
        }
        case 3:
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);
}

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
}