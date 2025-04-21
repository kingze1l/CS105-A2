#pragma once
#include "Student.h"
#include "Course.h"
#include "Grade.h"
#include <fstream>


class Enrolment {
private:
    Student student;
    Course course;
    Grade* grade;
public:
    Enrolment(const Student& s, const Course& c);
    ~Enrolment();
    void addGrade(Grade* g);
    void generateReport() const;
    const Student& getStudent() const { return student; }
    const Course& getCourse() const { return course; }

    void generateReport(std::ofstream& outFile) const;

    // ADDED: Accessor method for grade
    Grade* getGrade() const { return grade; }
};