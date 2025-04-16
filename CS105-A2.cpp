#include <iostream>
#include <vector>
#include "Student.h"
#include "Course.h"
#include "Enrolment.h"
#include "AuthSystem.h"
#include "Admin.h"
#include "Teacher.h"
#include "StudentUser.h"
#include "DataManager.h"

using namespace std;

vector<Student> students;
vector<Course> courses;
vector<Enrolment*> enrollments;

void initializeTestData() {
    if (students.empty()) {
        students.emplace_back(101, "Ali Khan", "123 Main St", "0700112233");
        students.emplace_back(102, "Sara Ahmed", "456 Oak Ave", "0722334455");
    }
    if (courses.empty()) {
        courses.emplace_back("Mathematics", "MATH101", "Basic Calculus", "teacher");
        courses.emplace_back("English", "ENG101", "Grammar and Composition", "teacher");
    }
    if (enrollments.empty()) {
        enrollments.push_back(new Enrolment(students[0], courses[0]));
        enrollments.push_back(new Enrolment(students[0], courses[1]));
        PrimaryGrade* grade1 = new PrimaryGrade();
        grade1->setInternalMark(75);
        grade1->setFinalMark(85);
        enrollments[0]->addGrade(grade1);
    }
}

void cleanupEnrollments() {
    for (auto* enrollment : enrollments) {
        delete enrollment;
    }
    enrollments.clear();
}

int main() {
    DataManager dataManager;

    bool studentsLoaded = dataManager.loadStudents(students);
    bool coursesLoaded = dataManager.loadCourses(courses);
    bool enrollmentsLoaded = dataManager.loadEnrollments(enrollments, students, courses);

    if (!studentsLoaded || !coursesLoaded || !enrollmentsLoaded) {
        initializeTestData();
    }

    AuthSystem authSystem;
    vector<std::unique_ptr<User>> loadedUsers;
    bool usersLoaded = dataManager.loadUsers(loadedUsers, students);
    if (usersLoaded) {
        // Clear existing users to avoid duplicates
        authSystem.clearUsers();
        for (auto& user : loadedUsers) {
            authSystem.addUser(std::move(user));
        }
    }

    // Add default student user if not already present
    bool student1Exists = false;
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == "student1") {
            student1Exists = true;
            break;
        }
    }
    if (!student1Exists) {
        // Find or create student with roll number 1
        Student* student1 = nullptr;
        for (auto& student : students) {
            if (student.getRollno() == 1) {
                student1 = &student;
                break;
            }
        }
        if (!student1) {
            students.emplace_back(1, "John Doe", "123 Main St", "555-1234");
            student1 = &students.back();
        }
        authSystem.addUser(std::make_unique<StudentUser>("student1", "student123", *student1));
    }

    // Load enrollments for all StudentUsers
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser) {
                studentUser->loadEnrollments(enrollments);
            }
        }
    }

    bool running = true;
    string username, password;

    while (running) {
        cout << "\n===== Pokeno South Primary School - Student Management System =====\n";
        cout << "Login\n";
        cout << "Username: "; getline(cin, username);
        cout << "Password: "; getline(cin, password);

        User* currentUser = authSystem.login(username, password);

        if (currentUser) {
            string role = currentUser->getRole();
            if (role == "admin") {
                Admin* admin = dynamic_cast<Admin*>(currentUser);
                if (admin) {
                    bool adminRunning = true;
                    while (adminRunning) {
                        int choice;
                        admin->showMenu();
                        cin >> choice;
                        cin.ignore();
                        switch (choice) {
                        case 1: admin->createStudent(authSystem, students); break;
                        case 2: admin->createTeacher(authSystem); break;
                        case 3: admin->createCourse(courses, authSystem); break;
                        case 4: admin->ViewAllUsers(authSystem.getUsers()); break;
                        case 5: admin->viewAllStudents(students); break;
                        case 6: admin->enrollStudent(enrollments, students, courses, authSystem); break;
                        case 7: adminRunning = false; break;
                        default: cout << "Invalid choice. Please try again.\n";
                        }
                    }
                }
            }
            else if (role == "teacher") {
                Teacher* teacher = dynamic_cast<Teacher*>(currentUser);
                if (teacher) {
                    bool teacherRunning = true;
                    while (teacherRunning) {
                        int choice;
                        teacher->showMenu();
                        cin >> choice;
                        cin.ignore();
                        switch (choice) {
                        case 1: teacher->showAssignedCourses(courses); break;
                        case 2: {
                            cout << "\n--- Select Course to Grade ---\n";
                            vector<Course> teacherCourses;
                            for (const auto& course : courses) {
                                if (course.getTeacherUsername() == teacher->getUsername()) {
                                    teacherCourses.push_back(course);
                                }
                            }
                            if (teacherCourses.empty()) {
                                cout << "No courses assigned.\n";
                                break;
                            }
                            for (size_t i = 0; i < teacherCourses.size(); ++i) {
                                cout << i + 1 << ". " << teacherCourses[i].getCourseName()
                                    << " (" << teacherCourses[i].getCourseCode() << ")\n";
                            }
                            int courseChoice;
                            cout << "Select course (1-" << teacherCourses.size() << "): ";
                            cin >> courseChoice;
                            cin.ignore();
                            if (courseChoice < 1 || courseChoice > static_cast<int>(teacherCourses.size())) {
                                cout << "Invalid course selection.\n";
                                break;
                            }
                            cout << "\n--- Select Student to Grade ---\n";
                            vector<Enrolment*> courseEnrollments;
                            for (auto* enrollment : enrollments) {
                                if (enrollment->getCourse().getCourseCode() == teacherCourses[courseChoice - 1].getCourseCode()) {
                                    courseEnrollments.push_back(enrollment);
                                }
                            }
                            if (courseEnrollments.empty()) {
                                cout << "No students enrolled in this course.\n";
                                break;
                            }
                            for (size_t i = 0; i < courseEnrollments.size(); ++i) {
                                cout << i + 1 << ". " << courseEnrollments[i]->getStudent().getName()
                                    << " (Roll No: " << courseEnrollments[i]->getStudent().getRollno() << ")\n";
                            }
                            int studentChoice;
                            cout << "Select student (1-" << courseEnrollments.size() << "): ";
                            cin >> studentChoice;
                            cin.ignore();
                            if (studentChoice < 1 || studentChoice > static_cast<int>(courseEnrollments.size())) {
                                cout << "Invalid student selection.\n";
                                break;
                            }
                            teacher->gradeAssignment(*courseEnrollments[studentChoice - 1]);
                            break;
                        }
                        case 3: teacherRunning = false; break;
                        default: cout << "Invalid choice. Please try again.\n";
                        }
                    }
                }
            }
            else if (role == "student") {
                StudentUser* student = dynamic_cast<StudentUser*>(currentUser);
                if (student) {
                    student->showMenu();
                }
            }
        }
        else {
            cout << "Login failed. Invalid username or password.\n";
        }

        cout << "\nDo you want to exit the program? (y/n): ";
        char exitChoice;
        cin >> exitChoice;
        cin.ignore();
        if (exitChoice == 'y' || exitChoice == 'Y') {
            running = false;
        }
    }

    dataManager.saveStudents(students);
    dataManager.saveCourses(courses);
    dataManager.saveEnrollments(enrollments);
    dataManager.saveUsers(authSystem.getUsers());

    cleanupEnrollments();

    cout << "\nThank you for using Pokeno South Primary School Student Management System.\n";
    return 0;
}