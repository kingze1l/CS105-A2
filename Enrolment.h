#pragma once
#include "Student.h"
#include "Course.h"
#include "Grade.h"

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

    // ADDED: Accessor method for grade
    Grade* getGrade() const { return grade; }
};