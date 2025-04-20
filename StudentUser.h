#pragma once
#include "User.h"
#include "Student.h"
#include <vector>
#include "Enrolment.h"

class Enrolment;

class StudentUser : public User {
private:
    Student student;
    std::vector<Enrolment*> enrolments;
public:
    StudentUser(const std::string& username, const std::string& password, const Student& student);
    void loadEnrollments(const std::vector<Enrolment*>& allEnrollments);
    void displayEnrolledCourses() const;
    void addEnrolment(Enrolment* enrolment);
    void showMenu() override;
    const Student& getStudent() const { return student; }
};