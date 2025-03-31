#include "Course.h"
#include <iostream>

Course::Course(const std::string& cName, const std::string& cCode,
    const std::string& desc, const std::string& t)
    : courseName(cName), courseCode(cCode), description(desc), teacher(t) {
}

std::string Course::getCourseName() const { return courseName; }
std::string Course::getCourseCode() const { return courseCode; }
std::string Course::getDescription() const { return description; }
std::string Course::getTeacher() const { return teacher; }

void Course::display() const {
    std::cout << "Course Name: " << courseName << "\n"
        << "Course Code: " << courseCode << "\n"
        << "Description: " << description << "\n"
        << "Teacher: " << teacher << "\n";
}