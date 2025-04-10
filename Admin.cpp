#include "Admin.h"
#include "Student.h"
#include "Course.h"
#include <iostream>
#include "Enrolment.h"

Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd, "admin") {
}

void Admin::showMenu() {
    printAdminHeader();
    std::cout << "1. Create Student Account\n"
        << "2. Create Course\n"
        << "3. View All Users\n"
        << "4. Enroll Student in Course\n"  
        << "5. Exit\n"  
        << "Enter your choice: ";
}
void Admin::createCourse(std::vector<Course>& courses) {
    std::string name, code, description, teacher;

    std::cout << "\n--- Create New Course ---\n";
    std::cout << "Course Name: ";
    std::getline(std::cin, name);

    std::cout << "Course Code: ";
    std::getline(std::cin, code);

    std::cout << "Description: ";
    std::getline(std::cin, description);

    std::cout << "Teacher: ";
    std::getline(std::cin, teacher);

    courses.emplace_back(name, code, description, teacher);
    std::cout << "Course created successfully!\n";
}
void Admin::createStudent(std::vector<Student>& students) {
    std::string name, address, contact;
    int rollno;

    std::cout << "\n---- Create Student Account ----\n";
    std::cout << "Enter Roll No: "; std::cin >> rollno;
    std::cin.ignore();

    std::cout << "Enter Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Address: ";
    std::getline(std::cin, address);

    std::cout << "Enter Contact: ";
    std::getline(std::cin, contact);

    students.emplace_back(rollno, name, address, contact);
    std::cout << "Student account created successfully!\n";
}


void Admin::enrollStudent(std::vector<Enrolment*>& enrollments, const std::vector<Student>& students, const std::vector<Course>& courses) {
    int rollno;
    std::string courseCode;

    std::cout << "\n--- Enroll Student in Course ---\n";
    std::cout << "Enter Student Roll No: ";
    std::cin >> rollno;
    std::cin.ignore();  // Clear buffer after integer input
    std::cout << "Enter Course Code: ";
    std::getline(std::cin, courseCode);

    // Find student by roll number
    Student* foundStudent = nullptr;
    for (const auto& student : students) {
        if (student.getRollno() == rollno) {
			foundStudent = const_cast<Student*>(&student);  // Remove const for Enrolment constructor ie, const_cast is used to cast away the constness of the object temporarily so in our case we can use it in Enrolment constructor
            break;
        }
    }

    // Find course by code
    Course* foundCourse = nullptr;
    for (const auto& course : courses) {
        if (course.getCourseCode() == courseCode) {
            foundCourse = const_cast<Course*>(&course);
            break;
        }
    }

    // Create enrollment if both found
    if (foundStudent && foundCourse) {
        enrollments.push_back(new Enrolment(*foundStudent, *foundCourse));
        std::cout << "Student enrolled successfully!\n";
    }
    else {
        std::cout << "Error: Student or Course not found!\n";
    }
}
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