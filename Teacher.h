// Teacher.h
#pragma once
#include "User.h"
#include "Enrolment.h"
#include <vector>
#include "Course.h"

class Teacher : public User {
private:
    std::string fullName;
public:
    Teacher(const std::string& username, const std::string& password, const std::string& name);
    std::string getTeacherName() const;
    void showAssignedCourses(const std::vector<Course>& courses) const;
    void showMenu() override;
    void gradeAssignment(Enrolment& enrolment);
    void viewStudentsAndGenerateReports(const std::vector<Course>& courses, const std::vector<Enrolment*>& enrollments);
	void editGrade(Enrolment& enrolment);
	void deleteGrade(Enrolment& enrolment);

    void setTeacherName(const std::string& name);
};