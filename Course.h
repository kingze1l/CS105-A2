
#pragma once
#include <string>


class Course {
private:
    std::string courseName;
    std::string courseCode;
    std::string description;
    std::string teacher;
    std::string teacherUsername;  // Add this new member

public:
    // Modify constructor declaration:
    Course(const std::string& cName, const std::string& cCode,
           const std::string& desc, const std::string& teacherUsername);
    
    std::string getCourseName() const;
    std::string getCourseCode() const;
    std::string getDescription() const;
   // std::string getTeacher() const;
    std::string getTeacherUsername() const;  // Add this new getter

    void display() const;
};