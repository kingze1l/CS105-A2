#pragma once
#include <string>

class Course {
private:
    std::string courseName;
    std::string courseCode;
    std::string description;
    std::string teacher;

public:
    Course(const std::string& cName, const std::string& cCode,
        const std::string& desc, const std::string& t);

    std::string getCourseName() const;
    std::string getCourseCode() const;
    std::string getDescription() const;
    std::string getTeacher() const;

    void display() const;
};