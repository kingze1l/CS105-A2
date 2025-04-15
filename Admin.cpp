#include "Admin.h"
#include "Student.h"
#include "Course.h"
#include <iostream>
#include "Enrolment.h"
#include <iomanip>
#include "AuthSystem.h"
#include "StudentUser.h"
#include "Teacher.h"
#include "DataManager.h"
#include "User.h"
#include <vector>


Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd, "admin") {
}




void Admin::showMenu() {
    printAdminHeader();
    std::cout << "1. Create Student Account\n"
        << "2. Create Teacher Account\n"  // New option
        << "3. Create Course\n"
        << "4. Enroll Student in Course\n"
        << "5. View All Users\n"
        << "6. Exit\n"
        << "Enter your choice: ";
}
void Admin::createCourse(std::vector<Course>& courses, const AuthSystem& authSystem) {
    std::string name, code, description;

    std::cout << "\n--- Create New Course ---\n";
    std::cout << "Course Name: "; std::getline(std::cin, name);
    std::cout << "Course Code: "; std::getline(std::cin, code);

    for (const auto& course : courses) {
        if (course.getCourseCode() == code) {
            std::cout << "Error: Course code already exists!\n";
            return;
        }
    }

    std::cout << "Description: "; std::getline(std::cin, description);

    // NEW: Teacher selection
    std::cout << "Available Teachers:\n";
    int i = 1;
    std::vector<std::string> teacherUsernames;
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "teacher") {
            std::cout << i++ << ". " << user->getUsername() << "\n";
            teacherUsernames.push_back(user->getUsername());
        }
    }

    if (teacherUsernames.empty()) {
        std::cout << "No teachers available. Create a teacher first.\n";
        return;
    }

    int choice;
    std::cout << "Select teacher (1-" << teacherUsernames.size() << "): ";
    std::cin >> choice;
    std::cin.ignore();

    courses.emplace_back(name, code, description, teacherUsernames[choice - 1]);
    std::cout << "Course created successfully!\n";
}
//void Admin::createStudent(std::vector<Student>& students) {
//    std::string name, address, contact;
//    int rollno;
//
//    std::cout << "\n---- Create Student Account ----\n";
//    std::cout << "Enter Roll No: "; std::cin >> rollno;
//    std::cin.ignore();
//
//    std::cout << "Enter Name: ";
//    std::getline(std::cin, name);
//
//    std::cout << "Enter Address: ";
//    std::getline(std::cin, address);
//
//    std::cout << "Enter Contact: ";
//    std::getline(std::cin, contact);
//
//    students.emplace_back(rollno, name, address, contact);
//    std::cout << "Student account created successfully!\n";
//}
//void Admin::createTeacher(AuthSystem& authSystem) {
//    std::string username, password, name;
//
//    std::cout << "\n--- Create Teacher Account ---\n";
//    std::cout << "Username: "; std::getline(std::cin, username);
//    std::cout << "Password: "; std::getline(std::cin, password);
//    std::cout << "Teacher's Full Name: "; std::getline(std::cin, name);
//
//    authSystem.addUser(std::make_unique<Teacher>(username, password, name));
//    std::cout << "Teacher account created successfully!\n";
//}
void Admin::createStudent(AuthSystem& authSystem, std::vector<Student>& students) {
    std::string username, password, name, address, contact;
    int rollno;

    std::cout << "\n---- Create Student Account ----\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    std::cout << "Password: "; std::getline(std::cin, password);
    std::cout << "Enter Roll No: "; std::cin >> rollno;
    std::cin.ignore();

    std::cout << "Enter Name: "; std::getline(std::cin, name);
    std::cout << "Enter Address: "; std::getline(std::cin, address);
    std::cout << "Enter Contact: "; std::getline(std::cin, contact);

    students.emplace_back(rollno, name, address, contact);
    authSystem.addUser(std::make_unique<StudentUser>(username, password, students.back()));
    std::cout << "Student account created successfully!\n";
}
void Admin::createTeacher(AuthSystem& authSystem) {
    std::string username, password, name;

    std::cout << "\n--- Create Teacher Account ---\n";
    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    std::cout << "Teacher's Full Name: ";
    std::getline(std::cin, name);

    authSystem.addUser(std::make_unique<Teacher>(username, password, name));
    std::cout << "Teacher account created successfully!\n";
}


//void Admin::enrollStudent(std::vector<Enrolment*>& enrollments, const std::vector<Student>& students, const std::vector<Course>& courses) {
//    int rollno;
//    std::string courseCode;
//
//    std::cout << "\n--- Enroll Student in Course ---\n";
//    std::cout << "Enter Student Roll No: ";
//    std::cin >> rollno;
//    std::cin.ignore();  // Clear buffer after integer input
//    std::cout << "Enter Course Code: ";
//    std::getline(std::cin, courseCode);
//
//    // Find student by roll number
//    Student* foundStudent = nullptr;
//    for (const auto& student : students) {
//        if (student.getRollno() == rollno) {
//			foundStudent = const_cast<Student*>(&student);  // Remove const for Enrolment constructor ie, const_cast is used to cast away the constness of the object temporarily so in our case we can use it in Enrolment constructor
//            break;
//        }
//    }
//
//    // Find course by code
//    Course* foundCourse = nullptr;
//    for (const auto& course : courses) {
//        if (course.getCourseCode() == courseCode) {
//            foundCourse = const_cast<Course*>(&course);
//            break;
//        }
//    }
//
//    // Create enrollment if both found
//    if (foundStudent && foundCourse) {
//        enrollments.push_back(new Enrolment(*foundStudent, *foundCourse));
//        std::cout << "Student enrolled successfully!\n";
//    }
//    else {
//        std::cout << "Error: Student or Course not found!\n";
//    }
//}
void Admin::ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const {
    printAdminHeader();
    std::cout << "---- System Users ----\n";

    for (const auto& user : users) {
        std::cout << "- " << user->getUsername()
            << " (" << user->getRole() << ")\n";
    }
}

void Admin::printAdminHeader() const {
    std::cout << "\n---- Admin Menu ----\n";
    std::cout << "Welcome, " << getUsername() << "!\n";
}
void Admin::viewAllStudents(const std::vector<Student>& students) const {
    std::cout << "\n--- All Students ---\n";
    if (students.empty()) {
        std::cout << "No students in the system.\n";
        return;
    }
    for (size_t i = 0; i < students.size(); ++i) {
        std::cout << i + 1 << ". " << students[i].getName() << " (Roll No: " << students[i].getRollno() << ")\n";
    }
}

void Admin::viewAllCourses(const std::vector<Course>& courses) const {
    std::cout << "\n--- All Courses ---\n";
    if (courses.empty()) {
        std::cout << "No courses in the system.\n";
        return;
    }
    for (size_t i = 0; i < courses.size(); ++i) {
        std::cout << i + 1 << ". " << courses[i].getCourseCode() << " - " << courses[i].getCourseName() << "\n";
    }
}

Student* Admin::selectStudent(const std::vector<Student>& students) const {
    viewAllStudents(students);
    if (students.empty()) return nullptr;
    int choice;
    std::cout << "Enter student number (1-" << students.size() << "): ";
    std::cin >> choice;
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(students.size())) {
        std::cout << "Invalid selection.\n";
        return nullptr;
    }
    return const_cast<Student*>(&students[choice - 1]);  // -1 because list starts at 1
}

Course* Admin::selectCourse(const std::vector<Course>& courses) const {
    viewAllCourses(courses);
    if (courses.empty()) return nullptr;
    int choice;
    std::cout << "Enter course number (1-" << courses.size() << "): ";
    std::cin >> choice;
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(courses.size())) {
        std::cout << "Invalid selection.\n";
        return nullptr;
    }
    return const_cast<Course*>(&courses[choice - 1]);
}

void Admin::enrollStudent(std::vector<Enrolment*>& enrollments,
    const std::vector<Student>& students,
    const std::vector<Course>& courses,
    const AuthSystem& authSystem) {
    std::cout << "\n--- Enroll Student in Course ---\n";
    Student* student = selectStudent(students);
    if (!student) {
        std::cout << "Enrollment cancelled: No student selected.\n";
        return;
    }

    // Show available courses with teachers
    std::cout << "\nAvailable Courses:\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        std::cout << i + 1 << ". " << courses[i].getCourseCode()
            << " - " << courses[i].getCourseName()
            << " (Teacher: " << courses[i].getTeacherUsername() << ")\n";
    }

    if (courses.empty()) {
        std::cout << "No courses available to enroll in.\n";
        return;
    }

    int courseChoice;
    std::cout << "Select course (1-" << courses.size() << "): ";
    std::cin >> courseChoice;
    std::cin.ignore();

    if (courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        std::cout << "Invalid course selection.\n";
        return;
    }

    // Check for existing enrollment
    for (const auto* enrollment : enrollments) {
        if (enrollment->getStudent().getRollno() == student->getRollno() &&
            enrollment->getCourse().getCourseCode() == courses[courseChoice - 1].getCourseCode()) {
            std::cout << "Error: Student already enrolled in this course!\n";
            return;
        }
    }

    enrollments.push_back(new Enrolment(*student, courses[courseChoice - 1]));
    std::cout << "Student enrolled successfully!\n";
}
