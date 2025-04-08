// Teacher.h
#pragma once
#include "User.h"
#include "Enrolment.h"

class Teacher : public User {
public:
    Teacher(const std::string& username, const std::string& password);
    void showMenu() override;
	void gradeAssignment(Enrolment& enrolment);
};