
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

	//setters
    // Add setters
    void setCourseName(const std::string& cName);
    void setCourseCode(const std::string& cCode);
    void setDescription(const std::string& desc);
    void setTeacherUsername(const std::string& teacherUsername);


    void display() const;
};