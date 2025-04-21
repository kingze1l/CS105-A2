#include "Teacher.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "Enrolment.h"
#include "Course.h"
#include "Student.h"
#include <fstream>
#include <ctime>

Teacher::Teacher(const std::string& uname, const std::string& pwd, const std::string& name)
    : User(uname, pwd, "teacher"), fullName(name) {
}

std::string Teacher::getTeacherName() const { return fullName; }

void Teacher::setTeacherName(const std::string& name) { fullName = name; }

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


void Teacher::viewStudentsAndGenerateReports(const std::vector<Course>& courses, const std::vector<Enrolment*>& enrollments) {
    std::cout << "\n------ Course Reports ------\n";

    // Open reports.txt in append mode
    std::ofstream outFile("reports.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cout << "Error: Could not open reports.txt for writing.\n";
        return;
    }

    // Get teacher's courses
    std::vector<Course> teacherCourses;
    for (const auto& course : courses) {
        if (course.getTeacherUsername() == getUsername()) {
            teacherCourses.push_back(course);
        }
    }

    if (teacherCourses.empty()) {
        std::cout << "No courses assigned to you.\n";
        outFile << "Teacher: " << fullName << " - No courses assigned.\n\n";
        outFile.close();
        return;
    }

    // Display courses
    std::cout << "Your Courses:\n";
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(20) << "Course Name"
        << " | " << std::setw(15) << "Course Code" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < teacherCourses.size(); ++i) {
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(20) << teacherCourses[i].getCourseName()
            << " | " << std::setw(15) << teacherCourses[i].getCourseCode() << " |\n";
    }
    std::cout << "-------------------------------------------------\n";

    int courseChoice;
    std::cout << "Select a course to generate report (1-" << teacherCourses.size() << ", 0 for all courses): ";
    if (!(std::cin >> courseChoice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        outFile.close();
        return;
    }
    std::cin.ignore();

    if (courseChoice < 0 || courseChoice > static_cast<int>(teacherCourses.size())) {
        std::cout << "Error: Invalid selection.\n";
        outFile.close();
        return;
    }

    // Get current timestamp
    std::time_t now = std::time(nullptr);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now);
    std::string timestamp(buffer);
    timestamp.pop_back();

    // Process selected course(s)
    std::vector<Course> coursesToReport;
    if (courseChoice == 0) {
        coursesToReport = teacherCourses;
    }
    else {
        coursesToReport.push_back(teacherCourses[courseChoice - 1]);
    }

    for (const auto& course : coursesToReport) {
        std::cout << "\n=== Course Report: " << course.getCourseName() << " ===\n";
        outFile << "=== Course Report: " << course.getCourseName() << " ===\n";
        std::cout << "Generated: " << timestamp << "\n";
        outFile << "Generated: " << timestamp << "\n";
        std::cout << "Course Code: " << course.getCourseCode() << "\n";
        outFile << "Course Code: " << course.getCourseCode() << "\n";
        std::cout << "Teacher: " << fullName << "\n";
        outFile << "Teacher: " << fullName << "\n";

        // Find enrolled students
        std::vector<Enrolment*> courseEnrollments;
        for (const auto* enrollment : enrollments) {
            if (enrollment->getCourse().getCourseCode() == course.getCourseCode()) {
                courseEnrollments.push_back(const_cast<Enrolment*>(enrollment));
            }
        }

        if (courseEnrollments.empty()) {
            std::cout << "No students enrolled in this course.\n";
            outFile << "No students enrolled in this course.\n";
            std::cout << "===================\n";
            outFile << "===================\n\n";
            continue;
        }

        // Display students and grades
        std::cout << "-------------------------------------------------\n";
        std::cout << "| " << std::left << std::setw(5) << "No."
            << " | " << std::setw(20) << "Student Name"
            << " | " << std::setw(10) << "Roll No"
            << " | " << std::setw(12) << "Total Grade" << " |\n";
        std::cout << "-------------------------------------------------\n";
        outFile << "-------------------------------------------------\n";
        outFile << "| " << std::left << std::setw(5) << "No."
            << " | " << std::setw(20) << "Student Name"
            << " | " << std::setw(10) << "Roll No"
            << " | " << std::setw(12) << "Total Grade" << " |\n";
        outFile << "-------------------------------------------------\n";

        float totalGrade = 0.0f;
        int gradedStudents = 0;
        for (size_t i = 0; i < courseEnrollments.size(); ++i) {
            float grade = courseEnrollments[i]->getGrade() ? courseEnrollments[i]->getGrade()->calculateGrade() : 0.0f;
            if (courseEnrollments[i]->getGrade()) {
                totalGrade += grade;
                gradedStudents++;
            }
            std::cout << "| " << std::left << std::setw(5) << (i + 1)
                << " | " << std::setw(20) << courseEnrollments[i]->getStudent().getName()
                << " | " << std::setw(10) << courseEnrollments[i]->getStudent().getRollno()
                << " | " << std::setw(12) << (courseEnrollments[i]->getGrade() ? std::to_string(grade) : "N/A") << " |\n";
            outFile << "| " << std::left << std::setw(5) << (i + 1)
                << " | " << std::setw(20) << courseEnrollments[i]->getStudent().getName()
                << " | " << std::setw(10) << courseEnrollments[i]->getStudent().getRollno()
                << " | " << std::setw(12) << (courseEnrollments[i]->getGrade() ? std::to_string(grade) : "N/A") << " |\n";
        }
        std::cout << "-------------------------------------------------\n";
        outFile << "-------------------------------------------------\n";

        // Course statistics
        std::cout << "Total Students: " << courseEnrollments.size() << "\n";
        outFile << "Total Students: " << courseEnrollments.size() << "\n";
        if (gradedStudents > 0) {
            float averageGrade = totalGrade / gradedStudents;
            std::cout << "Average Grade: " << averageGrade << "\n";
            outFile << "Average Grade: " << averageGrade << "\n";
        }
        else {
            std::cout << "Average Grade: N/A (no grades assigned)\n";
            outFile << "Average Grade: N/A (no grades assigned)\n";
        }
        std::cout << "===================\n";
        outFile << "===================\n\n";

        // Option to generate detailed student report
        std::cout << "Generate detailed report for a student? (1-" << courseEnrollments.size() << ", 0 to skip): ";
        int studentChoice;
        if (!(std::cin >> studentChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: Invalid input.\n";
            continue;
        }
        std::cin.ignore();

        if (studentChoice >= 1 && studentChoice <= static_cast<int>(courseEnrollments.size())) {
            courseEnrollments[studentChoice - 1]->generateReport(outFile);
        }
    }

    outFile.close();
    std::cout << "Report saved to reports.txt\n";
}

void Teacher::showMenu() {
    std::cout << "\n=== TEACHER MENU ===\n"
        << "1. View Assigned Courses\n"
        << "2. Input Grades\n"
        << "3. Edit Grades\n"          // New option
        << "4. Delete Grades\n"        // New option
        << "5. View Students and Generate Reports\n"
        << "6. Exit\n"
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
void Teacher::editGrade(Enrolment& enrolment) {
    std::cout << "\n------ Edit Grade ------\n";
    std::cout << "Grading for: " << enrolment.getStudent().getName()
        << " in " << enrolment.getCourse().getCourseName() << "\n";

    Grade* existingGrade = enrolment.getGrade();
    if (existingGrade) {
        std::cout << "Current Grades: Internal = " << existingGrade->getInternalMark()
            << ", Final = " << existingGrade->getFinalMark()
            << ", Total = " << existingGrade->calculateGrade() << "\n";
    }
    else {
        std::cout << "No grades assigned yet. Use 'Input Grades' to add grades.\n";
        return;
    }

    float internal, final;
    std::cout << "Enter new internal marks (0-100, -1 to keep " << existingGrade->getInternalMark() << "): ";
    if (!(std::cin >> internal)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return;
    }
    std::cout << "Enter new final marks (0-100, -1 to keep " << existingGrade->getFinalMark() << "): ";
    if (!(std::cin >> final)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return;
    }
    std::cin.ignore();

    // Validate marks (allow -1 to skip updating)
    if (internal != -1 && (internal < 0 || internal > 100)) {
        std::cout << "Error: Internal marks must be between 0 and 100.\n";
        return;
    }
    if (final != -1 && (final < 0 || final > 100)) {
        std::cout << "Error: Final marks must be between 0 and 100.\n";
        return;
    }

    // Update grades if provided
    if (internal != -1) existingGrade->setInternalMark(internal);
    if (final != -1) existingGrade->setFinalMark(final);

    std::cout << "Success: Grades updated successfully!\n";
}
void Teacher::deleteGrade(Enrolment& enrolment) {
    std::cout << "\n------ Delete Grade ------\n";
    std::cout << "Student: " << enrolment.getStudent().getName()
        << ", Course: " << enrolment.getCourse().getCourseName() << "\n";

    Grade* existingGrade = enrolment.getGrade();
    if (!existingGrade) {
        std::cout << "Error: No grades assigned to this enrollment.\n";
        return;
    }

    std::cout << "Current Grades: Internal = " << existingGrade->getInternalMark()
        << ", Final = " << existingGrade->getFinalMark()
        << ", Total = " << existingGrade->calculateGrade() << "\n";
    std::cout << "Are you sure you want to delete these grades? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Delete cancelled.\n";
        return;
    }

    enrolment.addGrade(nullptr); // Deletes existing grade and sets to nullptr
    std::cout << "Success: Grades deleted successfully!\n";
}