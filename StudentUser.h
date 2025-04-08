#pragma once
#include "User.h"
#include "Student.h"
#include <vector>

class Enrolment; // forward declaration for Enrolment class

class StudentUser : public User {
    const Student& student;
    std::vector<Enrolment*> enrolments; // Track student's courses
public:
    StudentUser(const std::string& username,
        const std::string& password,
        const Student& student);

    void addEnrolment(Enrolment* enrolment);
    void showMenu() override;

    // ADDED: Accessor method for student
    const Student& getStudent() const { return student; }
};