#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool DataManager::saveStudents(const std::vector<Student>& students) {
    std::ofstream file(STUDENTS_FILE);
    if (!file) {
        std::cerr << "Error: Could not open file " << STUDENTS_FILE << std::endl;
        return false;
    }

    for (const auto& student : students) {
        file << student.getRollno() << "|"
            << student.getName() << "|"
            << student.getAddress() << "|"
            << student.getContact() << std::endl;
    }

    return true;
}

bool DataManager::saveCourses(const std::vector<Course>& courses) {
    std::ofstream file(COURSES_FILE);
    if (!file) {
        std::cerr << "Error: Could not open file " << COURSES_FILE << std::endl;
        return false;
    }

    for (const auto& course : courses) {
        file << course.getCourseName() << "|"
            << course.getCourseCode() << "|"
            << course.getDescription() << "|"
            << course.getTeacherUsername() << std::endl;
    }

    return true;
}

bool DataManager::saveEnrollments(const std::vector<Enrolment*>& enrollments) {
    std::ofstream file(ENROLLMENTS_FILE);
    if (!file) {
        std::cerr << "Error: Could not open file " << ENROLLMENTS_FILE << std::endl;
        return false;
    }

    for (const auto& enrollment : enrollments) {
        file << enrollment->getStudent().getRollno() << "|"
            << enrollment->getCourse().getCourseCode();

        Grade* grade = enrollment->getGrade();
        if (grade) {
            file << "|" << grade->getInternalMark()
                << "|" << grade->getFinalMark();
        }
        file << std::endl;
    }

    return true;
}

bool DataManager::saveUsers(const std::vector<std::unique_ptr<User>>& users) {
    std::ofstream file(USERS_FILE);
    if (!file) {
        std::cerr << "Error: Could not open file " << USERS_FILE << std::endl;
        return false;
    }

    for (const auto& user : users) {
        file << user->getUsername() << "|"
            << user->getPassword() << "|"  // Save the actual password
            << user->getRole();

        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser) {
                file << "|" << studentUser->getStudent().getRollno();
            }
        }
        file << std::endl;
    }

    return true;
}

bool DataManager::loadStudents(std::vector<Student>& students) {
    std::ifstream file(STUDENTS_FILE);
    if (!file) {
        std::cerr << "Warning: Could not open file " << STUDENTS_FILE << std::endl;
        std::ofstream createFile(STUDENTS_FILE);
        if (!createFile) {
            std::cerr << "Error: Could not create file " << STUDENTS_FILE << std::endl;
            return false;
        }
        createFile.close();
        return false;
    }

    students.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string rollnoStr, name, address, contact;

        std::getline(ss, rollnoStr, '|');
        std::getline(ss, name, '|');
        std::getline(ss, address, '|');
        std::getline(ss, contact, '|');

        int rollno = std::stoi(rollnoStr);
        students.emplace_back(rollno, name, address, contact);
    }

    return true;
}

bool DataManager::loadCourses(std::vector<Course>& courses) {
    std::ifstream file(COURSES_FILE);
    if (!file) {
        std::cerr << "Warning: Could not open file " << COURSES_FILE << std::endl;
        std::ofstream createFile(COURSES_FILE);
        if (!createFile) {
            std::cerr << "Error: Could not create file " << COURSES_FILE << std::endl;
            return false;
        }
        createFile.close();
        return false;
    }

    courses.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, code, description, teacher;

        std::getline(ss, name, '|');
        std::getline(ss, code, '|');
        std::getline(ss, description, '|');
        std::getline(ss, teacher, '|');

        courses.emplace_back(name, code, description, teacher);
    }

    return true;
}

bool DataManager::loadUsers(std::vector<std::unique_ptr<User>>& users,
    const std::vector<Student>& students) {
    std::ifstream file(USERS_FILE);
    if (!file) {
        std::cerr << "Warning: Could not open file " << USERS_FILE << std::endl;
        std::ofstream createFile(USERS_FILE);
        if (!createFile) {
            std::cerr << "Error: Could not create file " << USERS_FILE << std::endl;
            return false;
        }
        createFile.close();
        return false;
    }

    users.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password, role, rollnoStr;

        std::getline(ss, username, '|');
        std::getline(ss, password, '|');
        std::getline(ss, role, '|');

        if (role == "admin") {
            users.push_back(std::make_unique<Admin>(username, password));
        }
        else if (role == "teacher") {
            users.push_back(std::make_unique<Teacher>(username, password, "teacher" + username));
        }
        else if (role == "student" && std::getline(ss, rollnoStr, '|')) {
            int rollno = std::stoi(rollnoStr);
            Student studentObj = findStudentByRollNo(rollno, students);
            users.push_back(std::make_unique<StudentUser>(username, password, studentObj));
        }
    }

    return true;
}

bool DataManager::loadEnrollments(std::vector<Enrolment*>& enrollments,
    const std::vector<Student>& students,
    const std::vector<Course>& courses) {
    std::ifstream file(ENROLLMENTS_FILE);
    if (!file) {
        std::cerr << "Warning: Could not open file " << ENROLLMENTS_FILE << std::endl;
        std::ofstream createFile(ENROLLMENTS_FILE);
        if (!createFile) {
            std::cerr << "Error: Could not create file " << ENROLLMENTS_FILE << std::endl;
            return false;
        }
        createFile.close();
        return false;
    }

    for (auto* enrollment : enrollments) {
        delete enrollment;
    }
    enrollments.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string rollnoStr, courseCode, internalMarkStr, finalMarkStr;

        std::getline(ss, rollnoStr, '|');
        std::getline(ss, courseCode, '|');

        int rollno = std::stoi(rollnoStr);

        Student student = findStudentByRollNo(rollno, students);
        Course course = findCourseByCode(courseCode, courses);

        Enrolment* enrollment = new Enrolment(student, course);

        if (std::getline(ss, internalMarkStr, '|') &&
            std::getline(ss, finalMarkStr, '|')) {

            float internalMark = std::stof(internalMarkStr);
            float finalMark = std::stof(finalMarkStr);

            PrimaryGrade* grade = new PrimaryGrade();
            grade->setInternalMark(internalMark);
            grade->setFinalMark(finalMark);
            enrollment->addGrade(grade);
        }

        enrollments.push_back(enrollment);
    }

    return true;
}

Student DataManager::findStudentByRollNo(int rollNo, const std::vector<Student>& students) {
    for (const auto& student : students) {
        if (student.getRollno() == rollNo) {
            return student;
        }
    }
    return Student(0, "Unknown", "Unknown", "Unknown");
}

Course DataManager::findCourseByCode(const std::string& code, const std::vector<Course>& courses) {
    for (const auto& course : courses) {
        if (course.getCourseCode() == code) {
            return course;
        }
    }
    return Course("Unknown", "Unknown", "Unknown", "Unknown");
}