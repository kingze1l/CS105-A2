#pragma once
#include "User.h"
#include "Student.h"
#include <vector>
#include "Enrolment.h"

class Enrolment; // forward declaration for Enrolment class

class StudentUser : public User {
    const Student& student;
    std::vector<Enrolment*> enrolments; // Track student's courses
public:
    StudentUser(const std::string& username,
        const std::string& password,
        const Student& student);
    void loadEnrollments(const std::vector<Enrolment*>& allEnrollments) {
        enrolments.clear();

        for (auto* enrollment : allEnrollments) {
            if (enrollment->getStudent().getRollno() == student.getRollno()) {
                enrolments.push_back(enrollment);
            }
        }
    }
    void displayEnrolledCourses() const {
        std::cout << "\n==== Enrolled Courses ====\n";
        if (enrolments.empty()) {
            std::cout << "Not enrolled in any courses.\n";
            return;
        }

        for (size_t i = 0; i < enrolments.size(); i++) {
            std::cout << (i + 1) << ". " << enrolments[i]->getCourse().getCourseName()
                << " (" << enrolments[i]->getCourse().getCourseCode() << ")\n";
        }
    }

    void addEnrolment(Enrolment* enrolment);
    void showMenu() override;

    // ADDED: Accessor method for student
    const Student& getStudent() const { return student; }
};