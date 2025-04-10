#pragma once
#include "User.h"
#include <vector>
#include <memory> 
#include "Enrolment.h"

class Student;
class Course;

class Admin : public User {
public:
    Admin(const std::string& uname, const std::string& pwd);
    void showMenu() override;

    void createStudent(std::vector<Student>& students);
    void createCourse(std::vector<Course>& courses);
    void ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const;
    void viewAllStudents(const std::vector<Student>& students) const;  // New: List all students
    void viewAllCourses(const std::vector<Course>& courses) const;    // New: List all courses
    Student* selectStudent(const std::vector<Student>& students) const;  // New: Select a student
    Course* selectCourse(const std::vector<Course>& courses) const;     // New: Select a course
    void enrollStudent(std::vector<Enrolment*>& enrollments, const std::vector<Student>& students, const std::vector<Course>& courses);  // New method to enroll students by admin

private:
    void printAdminHeader() const;
};