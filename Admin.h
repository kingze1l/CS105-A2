
#pragma once
#include "User.h"
#include <vector>
#include <memory>
#include "Enrolment.h"
#include "AuthSystem.h"

class Student;
class Course;

class Admin : public User {
public:
    Admin(const std::string& uname, const std::string& pwd);
    void showMenu() override;
    void createStudent(AuthSystem& authSystem, std::vector<Student>& students);
    void createTeacher(AuthSystem& authSystem);
    void createCourse(std::vector<Course>& courses, AuthSystem& authSystem);
    void ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const;
    void viewAllStudents(const std::vector<Student>& students) const;
    void viewAllCourses(const std::vector<Course>& courses) const;
    Student* selectStudent(const std::vector<Student>& students) const;
    Course* selectCourse(const std::vector<Course>& courses) const;
    void enrollStudent(std::vector<Enrolment*>& enrollments,
        const std::vector<Student>& students,
        const std::vector<Course>& courses,
        const AuthSystem& authSystem);
    void editStudent(std::vector<Student>& students, AuthSystem& authSystem);
    void deleteStudent(std::vector<Student>& students, AuthSystem& authSystem, std::vector<Enrolment*>& enrollments);
    void editCourse(std::vector<Course>& courses, AuthSystem& authSystem);
    void deleteCourse(std::vector<Course>& courses, std::vector<Enrolment*>& enrollments, AuthSystem& authSystem);
    void editEnrolment(std::vector<Enrolment*>& enrollments, const std::vector<Student>& students, const std::vector<Course>& courses, AuthSystem& authSystem);
    Enrolment* selectEnrolment(const std::vector<Enrolment*>& enrollments) const;
    void deleteEnrolment(std::vector<Enrolment*>& enrollments, AuthSystem& authSystem);
    void editUser(AuthSystem& authSystem, std::vector<Student>& students, std::vector<Course>& courses);
    User* selectUser(const std::vector<std::unique_ptr<User>>& users) const;
    void deleteUser(AuthSystem& authSystem, std::vector<Student>& students, std::vector<Course>& courses, std::vector<Enrolment*>& enrollments);
    void generateSystemReport(const std::vector<Student>& students, const std::vector<Course>& courses, const std::vector<Enrolment*>& enrollments);
    void printAdminHeader() const;

};

// Declaration for password masking function
extern std::string getPasswordWithMask();


//#pragma once
//#include "User.h"
//#include <vector>
//#include <memory> 
//#include "Enrolment.h"
//#include "AuthSystem.h"
//#include "Student.h"
//#include "Course.h"
//#include <iostream>
//
//
//class Student;
//class Course;
//class AuthSystem;
//
//class Admin : public User {
//public:
//    Admin(const std::string& uname, const std::string& pwd);
//    void showMenu() override;
//    void createTeacher(AuthSystem& authSystem);
//    void createStudent(AuthSystem& authSystem, std::vector<Student>& students);
//    void createCourse(std::vector<Course>& courses, const AuthSystem& authSystem);
//    void ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const;
//    void viewAllStudents(const std::vector<Student>& students) const;  // New: List all students
//    void viewAllCourses(const std::vector<Course>& courses) const;    // New: List all courses
//    Student* selectStudent(const std::vector<Student>& students) const;  // New: Select a student
//    Course* selectCourse(const std::vector<Course>& courses) const;     // New: Select a course
//    void enrollStudent(std::vector<Enrolment*>& enrollments,
//        const std::vector<Student>& students,
//        const std::vector<Course>& courses,
//        const AuthSystem& authSystem);
//
//private:
//    void printAdminHeader() const;
//};