//#include "Course.h"
//#include <iostream>
//
////Course::Course(const std::string& cName, const std::string& cCode,
////    const std::string& desc, const std::string& t)
////    : courseName(cName), courseCode(cCode), description(desc), teacher(t) {
////}
//// Update constructor:
//Course::Course(const std::string& cName, const std::string& cCode,
//               const std::string& desc, const std::string& teacher)
//    : courseName(cName), courseCode(cCode), 
//      description(desc), teacherUsername(teacher) {}  // Changed to teacherUsername
//
//// Add getter:
//////std::string Course::getTeacherUsername() const {
////    return teacherUsername;
////}
//
//std::string Course::getCourseName() const { return courseName; }
//std::string Course::getCourseCode() const { return courseCode; }
//std::string Course::getDescription() const { return description; }
//std::string Course::getTeacher() const { return teacher; }
//std::string Course::getTeacherUsername() const { return teacherUsername; }
//
//void Course::display() const {
//    std::cout << "Course Name: " << courseName << "\n"
//        << "Course Code: " << courseCode << "\n"
//        << "Description: " << description << "\n"
//        << "Teacher: " << teacher << "\n";
//}

#include "Course.h"
#include <iostream>

// CHANGED: Parameter renamed
Course::Course(const std::string& cName, const std::string& cCode,
    const std::string& desc, const std::string& teacher)
    : courseName(cName), courseCode(cCode),
    description(desc), teacherUsername(teacher) {
}

std::string Course::getCourseName() const { return courseName; }
std::string Course::getCourseCode() const { return courseCode; }
std::string Course::getDescription() const { return description; }

// CHANGED: Renamed
std::string Course::getTeacherUsername() const {
    return teacherUsername;
}

void Course::display() const {
    std::cout << "Course Name: " << courseName << "\n"
        << "Course Code: " << courseCode << "\n"
        << "Description: " << description << "\n"
        << "Teacher: " << teacherUsername << "\n"; // CHANGED: Now shows username
}