#include "Teacher.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "Enrolment.h"
#include "Course.h"
#include "Student.h"

Teacher::Teacher(const std::string& uname, const std::string& pwd, const std::string& name)
    : User(uname, pwd, "teacher"), fullName(name) {
}

std::string Teacher::getTeacherName() const { return fullName; }

void Teacher::showAssignedCourses(const std::vector<Course>& courses) const {
    std::cout << "\n------ Your Assigned Courses ------\n";
    if (courses.empty()) {
        std::cout << "No courses available in the system.\n";
        return;
    }
    bool hasCourses = false;
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(20) << "Course Name"
        << " | " << std::setw(15) << "Course Code"
        << " | " << std::setw(12) << "Teacher" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (const auto& course : courses) {
        if (course.getTeacherUsername() == getUsername()) {
            std::cout << "| " << std::left << std::setw(20) << course.getCourseName()
                << " | " << std::setw(15) << course.getCourseCode()
                << " | " << std::setw(12) << course.getTeacherUsername() << " |\n";
            hasCourses = true;
        }
    }
    if (!hasCourses) {
        std::cout << "| " << std::left << std::setw(47) << "No courses assigned to you yet." << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
}

void Teacher::viewStudentsAndGenerateReports(const std::vector<Course>& courses, const std::vector<Enrolment*>& enrollments) const {
    std::cout << "\n------ Your Students ------\n";

    std::vector<Enrolment*> teacherEnrollments;
    for (const auto* enrollment : enrollments) {
        for (const auto& course : courses) {
            if (course.getTeacherUsername() == getUsername() &&
                enrollment->getCourse().getCourseCode() == course.getCourseCode()) {
                teacherEnrollments.push_back(const_cast<Enrolment*>(enrollment));
                break;
            }
        }
    }

    if (teacherEnrollments.empty()) {
        std::cout << "No students enrolled in your courses.\n";
        return;
    }

    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(20) << "Student Name"
        << " | " << std::setw(10) << "Roll No"
        << " | " << std::setw(15) << "Course" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < teacherEnrollments.size(); ++i) {
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(20) << teacherEnrollments[i]->getStudent().getName()
            << " | " << std::setw(10) << teacherEnrollments[i]->getStudent().getRollno()
            << " | " << std::setw(15) << teacherEnrollments[i]->getCourse().getCourseName() << " |\n";
    }
    std::cout << "-------------------------------------------------\n";

    int choice;
    std::cout << "Select a student to generate a report (1-" << teacherEnrollments.size() << ", 0 to cancel): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return;
    }
    std::cin.ignore();

    if (choice == 0) {
        std::cout << "Action cancelled.\n";
        return;
    }

    if (choice < 1 || choice > static_cast<int>(teacherEnrollments.size())) {
        std::cout << "Error: Invalid selection.\n";
        return;
    }

    std::cout << "\n------ Report for " << teacherEnrollments[choice - 1]->getStudent().getName() << " ------\n";
    teacherEnrollments[choice - 1]->generateReport();
}

void Teacher::showMenu() {
    std::cout << "\n=== TEACHER MENU ===\n"
        << "1. View Assigned Courses\n"
        << "2. Input Grades\n"
        << "3. View Students and Generate Reports\n"
        << "4. Exit\n"
        << "Choice: ";
}

void Teacher::gradeAssignment(Enrolment& enrolment) {
    float internal, final;

    std::cout << "\nGrading for: " << enrolment.getStudent().getName()
        << " in " << enrolment.getCourse().getCourseName() << "\n";

    std::cout << "Enter internal marks (0-100): ";
    if (!(std::cin >> internal)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return;
    }
    std::cout << "Enter final marks (0-100): ";
    if (!(std::cin >> final)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return;
    }
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