// DataManager.h
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Student.h"
#include "Course.h"
#include "Enrolment.h"
#include "User.h"
#include "Admin.h"
#include "Teacher.h"
#include "StudentUser.h"

class DataManager {
private:
    // File paths
    const std::string STUDENTS_FILE = "students.txt";
    const std::string COURSES_FILE = "courses.txt";
    const std::string ENROLLMENTS_FILE = "enrollments.txt";
    const std::string USERS_FILE = "users.txt";

public:
    // Save data to files
    bool saveStudents(const std::vector<Student>& students);
    bool saveCourses(const std::vector<Course>& courses);
    bool saveEnrollments(const std::vector<Enrolment*>& enrollments);
    bool saveUsers(const std::vector<std::unique_ptr<User>>& users);

    // Load data from files
    bool loadStudents(std::vector<Student>& students);
    bool loadCourses(std::vector<Course>& courses);
    bool loadEnrollments(std::vector<Enrolment*>& enrollments,
        const std::vector<Student>& students,
        const std::vector<Course>& courses);
    bool loadUsers(std::vector<std::unique_ptr<User>>& users,
        const std::vector<Student>& students);

private:
    // Helper methods
    Student findStudentByRollNo(int rollNo, const std::vector<Student>& students);
    Course findCourseByCode(const std::string& code, const std::vector<Course>& courses);
};