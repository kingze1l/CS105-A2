#include "Admin.h"
#include "Student.h"
#include "Course.h"
#include "StudentUser.h"
#include "Teacher.h"
#include "PasswordDisplay.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <conio.h> // For _getch()

Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd, "admin") {
}

void Admin::printAdminHeader() const {
    std::cout << "\n------ Admin Menu ------\n";
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
        << "Enter your choice (1-7): ";
}

void Admin::createStudent(AuthSystem& authSystem, std::vector<Student>& students) {
    std::string username, password, name, address, contact;
    int rollno;

    std::cout << "\n------ Create Student Account ------\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    if (username.empty()) {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == username) {
            std::cout << "Error: Username already taken!\n";
            return;
        }
    }
    std::cout << "Password: "; password = getPasswordWithMask();
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }
    std::cout << "Enter Roll No: ";
    if (!(std::cin >> rollno)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid roll number.\n";
        return;
    }
    std::cin.ignore();
    if (rollno <= 0) {
        std::cout << "Error: Roll number must be positive.\n";
        return;
    }
    for (const auto& student : students) {
        if (student.getRollno() == rollno) {
            std::cout << "Error: Roll number already exists!\n";
            return;
        }
    }
    std::cout << "Enter Name: "; std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "Error: Name cannot be empty.\n";
        return;
    }
    std::cout << "Enter Address: "; std::getline(std::cin, address);
    std::cout << "Enter Contact: "; std::getline(std::cin, contact);

    students.emplace_back(rollno, name, address, contact);
    authSystem.addUser(std::make_unique<StudentUser>(username, password, students.back()));
    std::cout << "Success: Student account created successfully!\n";
}

void Admin::createTeacher(AuthSystem& authSystem) {
    std::string username, password, name;

    std::cout << "\n------ Create Teacher Account ------\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    if (username.empty()) {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == username) {
            std::cout << "Error: Username already taken!\n";
            return;
        }
    }
    std::cout << "Password: "; password = getPasswordWithMask();
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }
    std::cout << "Teacher's Full Name: "; std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "Error: Name cannot be empty.\n";
        return;
    }

    authSystem.addUser(std::make_unique<Teacher>(username, password, name));
    std::cout << "Success: Teacher account created successfully!\n";
}

void Admin::createCourse(std::vector<Course>& courses, AuthSystem& authSystem) {
    std::string name, code, description;

    std::cout << "\n------ Create New Course ------\n";
    std::cout << "Course Name: "; std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "Error: Course name cannot be empty.\n";
        return;
    }
    std::cout << "Course Code: "; std::getline(std::cin, code);
    if (code.empty()) {
        std::cout << "Error: Course code cannot be empty.\n";
        return;
    }
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
        std::cout << "Error: No teachers available. Create a teacher first.\n";
        return;
    }

    int choice;
    std::cout << "Select teacher (1-" << teacherUsernames.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(teacherUsernames.size())) {
        std::cout << "Error: Invalid teacher selection.\n";
        return;
    }

    courses.emplace_back(name, code, description, teacherUsernames[choice - 1]);
    std::cout << "Success: Course created successfully!\n";
}

void Admin::ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const {
    std::cout << "\n------ System Users ------\n";
    if (users.empty()) {
        std::cout << "No users in the system.\n";
        return;
    }
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(12) << "Username"
        << " | " << std::setw(13) << "Password"
        << " | " << std::setw(6) << "Role"
        << " | " << std::setw(11) << "Roll Number" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (const auto& user : users) {
        std::cout << "| " << std::left << std::setw(12) << user->getUsername()
            << " | " << std::setw(13) << PasswordDisplay::maskPassword(user->getPassword())
            << " | " << std::setw(6) << user->getRole();
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser) {
                int rollno = studentUser->getStudent().getRollno();
                std::cout << " | " << std::setw(11) << (rollno != 0 ? std::to_string(rollno) : "-");
            }
            else {
                std::cout << " | -";
            }
        }
        else {
            std::cout << " | -";
        }
        std::cout << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
}

void Admin::viewAllStudents(const std::vector<Student>& students) const {
    std::cout << "\n------ All Students ------\n";
    if (students.empty()) {
        std::cout << "No students in the system.\n";
        return;
    }
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(20) << "Name"
        << " | " << std::setw(10) << "Roll No"
        << " | " << std::setw(15) << "Contact" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < students.size(); ++i) {
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(20) << students[i].getName()
            << " | " << std::setw(10) << students[i].getRollno()
            << " | " << std::setw(15) << students[i].getContact() << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
}

void Admin::viewAllCourses(const std::vector<Course>& courses) const {
    std::cout << "\n------ All Courses ------\n";
    if (courses.empty()) {
        std::cout << "No courses in the system.\n";
        return;
    }
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(15) << "Course Code"
        << " | " << std::setw(20) << "Course Name"
        << " | " << std::setw(12) << "Teacher" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(15) << courses[i].getCourseCode()
            << " | " << std::setw(20) << courses[i].getCourseName()
            << " | " << std::setw(12) << courses[i].getTeacherUsername() << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
}

Student* Admin::selectStudent(const std::vector<Student>& students) const {
    viewAllStudents(students);
    if (students.empty()) return nullptr;
    int choice;
    std::cout << "Enter student number (1-" << students.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return nullptr;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(students.size())) {
        std::cout << "Error: Invalid selection.\n";
        return nullptr;
    }
    return const_cast<Student*>(&students[choice - 1]);
}

Course* Admin::selectCourse(const std::vector<Course>& courses) const {
    viewAllCourses(courses);
    if (courses.empty()) return nullptr;
    int choice;
    std::cout << "Enter course number (1-" << courses.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return nullptr;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(courses.size())) {
        std::cout << "Error: Invalid selection.\n";
        return nullptr;
    }
    return const_cast<Course*>(&courses[choice - 1]);
}

void Admin::enrollStudent(std::vector<Enrolment*>& enrollments,
    const std::vector<Student>& students,
    const std::vector<Course>& courses,
    const AuthSystem& authSystem) {
    std::cout << "\n------ Enroll Student in Course ------\n";
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

    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser && studentUser->getStudent().getRollno() == student->getRollno()) {
                studentUser->addEnrolment(newEnrolment);
                break;
            }
        }
    }

    std::cout << "Success: Student enrolled successfully!\n";
}