//

//-------------------------------------------------------------
//  Admin.cpp
#include "Admin.h"
#include "Student.h"
#include "Course.h"
#include "StudentUser.h"
#include "Teacher.h"
#include <iostream>
#include <iomanip>

Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd, "admin") {
}

void Admin::printAdminHeader() const {
    std::cout << "\n---- Admin Menu ----\n";
    std::cout << "Welcome, " << getUsername() << "!\n";
}

void Admin::showMenu() {
    printAdminHeader();
    std::cout << "1. Create Student Account\n"
        << "2. Create Teacher Account\n"
        << "3. Create Course\n"
        << "4. View All Users\n"
        << "5. View All Students\n"
        << "6. Enroll Student in Course\n"
        << "7. Exit\n"
        << "Enter your choice: ";
}

void Admin::createStudent(AuthSystem& authSystem, std::vector<Student>& students) {
    std::string username, password, name, address, contact;
    int rollno;

    std::cout << "\n---- Create Student Account ----\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == username) {
            std::cout << "Error: Username already taken!\n";
            return;
        }
    }
    std::cout << "Password: "; std::getline(std::cin, password);
    std::cout << "Enter Roll No: "; std::cin >> rollno;
    std::cin.ignore();
    for (const auto& student : students) {
        if (student.getRollno() == rollno) {
            std::cout << "Error: Roll number already exists!\n";
            return;
        }
    }
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
    std::cout << "Username: "; std::getline(std::cin, username);
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == username) {
            std::cout << "Error: Username already taken!\n";
            return;
        }
    }
    std::cout << "Password: "; std::getline(std::cin, password);
    std::cout << "Teacher's Full Name: "; std::getline(std::cin, name);

    authSystem.addUser(std::make_unique<Teacher>(username, password, name));
    std::cout << "Teacher account created successfully!\n";
}

void Admin::createCourse(std::vector<Course>& courses, AuthSystem& authSystem) {
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

    std::cout << "Available Teachers:\n";
    int i = 1;
    std::vector<std::string> teacherUsernames;
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "teacher") {
            Teacher* teacher = dynamic_cast<Teacher*>(user.get());
            std::cout << i++ << ". " << (teacher ? teacher->getTeacherName() : user->getUsername()) << "\n";
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
    if (choice < 1 || choice > static_cast<int>(teacherUsernames.size())) {
        std::cout << "Invalid teacher selection.\n";
        return;
    }

    courses.emplace_back(name, code, description, teacherUsernames[choice - 1]);
    std::cout << "Course created successfully!\n";
}

void Admin::ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const {
    printAdminHeader();
    std::cout << "---- System Users ----\n";
    for (const auto& user : users) {
        std::cout << "- " << user->getUsername() << " (" << user->getRole() << ")\n";
    }
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
        std::cout << i + 1 << ". " << courses[i].getCourseCode() << " - " << courses[i].getCourseName()
            << " (Teacher: " << courses[i].getTeacherUsername() << ")\n";
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
    return const_cast<Student*>(&students[choice - 1]);
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

    Course* course = selectCourse(courses);
    if (!course) {
        std::cout << "Enrollment cancelled: No course selected.\n";
        return;
    }

    for (const auto* enrollment : enrollments) {
        if (enrollment->getStudent().getRollno() == student->getRollno() &&
            enrollment->getCourse().getCourseCode() == course->getCourseCode()) {
            std::cout << "Error: Student already enrolled in this course!\n";
            return;
        }
    }

    Enrolment* newEnrolment = new Enrolment(*student, *course);
    enrollments.push_back(newEnrolment);

    // Update StudentUser's enrollments
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser && studentUser->getStudent().getRollno() == student->getRollno()) {
                studentUser->addEnrolment(newEnrolment);
                break;
            }
        }
    }

    std::cout << "Student enrolled successfully!\n";
}