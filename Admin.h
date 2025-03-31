#pragma once
#include "User.h"
#include <vector>
#include <memory> 

class Student;
class Course;

class Admin : public User {
public:
    Admin(const std::string& uname, const std::string& pwd);
    void showMenu() override;

    void createStudent(std::vector<Student>& students);
    void createCourse(std::vector<Course>& courses);
    void ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const;

private:
    void printAdminHeader() const;
};