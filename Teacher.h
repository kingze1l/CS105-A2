// Teacher.h
#pragma once
#include "User.h"
#include "Enrolment.h"
#include <vector>
#include "Course.h" // NEW: Added include

class Teacher : public User {
private:
    std::string fullName; // NEW: Added to store teacher's name
public:
    // CHANGED: Added name parameter
    Teacher(const std::string& username, const std::string& password,
        const std::string& name);
	std::string getTeacherName() const; // NEW: Getter for teacher's name
    // NEW: Added method to show assigned courses
    void showAssignedCourses(const std::vector<Course>& courses) const;

    void showMenu() override;
    void gradeAssignment(Enrolment& enrolment);
};