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

// Global data stores (already defined globally, no redeclaration needed)
vector<Student> students;
vector<Course> courses;
vector<Enrolment*> enrollments;

// Initialize test data if needed
void initializeTestData() {
    if (students.empty()) {
        students.emplace_back(101, "Ali Khan", "123 Main St", "0700112233");
        students.emplace_back(102, "Sara Ahmed", "456 Oak Ave", "0722334455");
    }
    if (courses.empty()) {
        courses.emplace_back("Mathematics", "MATH101", "Basic Calculus", "Dr. Smith");
        courses.emplace_back("English", "ENG101", "Grammar and Composition", "Ms. Johnson");
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

// Clean up dynamically allocated enrollments
void cleanupEnrollments() {
    for (auto* enrollment : enrollments) {
        delete enrollment;
    }
    enrollments.clear();
}

int main() {
    DataManager dataManager;

    // Load data into global vectors (no local duplicates)
    bool studentsLoaded = dataManager.loadStudents(students);
    bool coursesLoaded = dataManager.loadCourses(courses);
    bool enrollmentsLoaded = dataManager.loadEnrollments(enrollments, students, courses);

    if (!studentsLoaded || !coursesLoaded || !enrollmentsLoaded) {
        initializeTestData();  // Populates global vectors if loading fails
    }

    AuthSystem authSystem;
    vector<std::unique_ptr<User>> loadedUsers;  // For future integration
    bool usersLoaded = dataManager.loadUsers(loadedUsers, students);

    bool running = true;
    string username, password;

    while (running) {
        cout << "\n===== Pokeno South Primary School - Student Management System =====\n";
        cout << "Login\n";
        cout << "Username: "; getline(cin, username);  // Improved: Allows spaces in input
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
                        case 1: admin->createStudent(authSystem ,students); break;
                        case 2: admin->createCourse(courses, authSystem); break;
                        case 3: admin->ViewAllUsers(authSystem.getUsers()); break;
                        case 4: admin->viewAllStudents(students); break;  // test case 
                        case 5: admin->enrollStudent(enrollments, students, courses, authSystem); break;
                        case 6: adminRunning = false; break;
                        default: cout << "Invalid choice. Please try again.\n";
                        }
                    }
                }
            }
            else if (role == "teacher") {
                Teacher* teacher = dynamic_cast<Teacher*>(currentUser);
                if (teacher) {
                    teacher->showMenu();
                }
            }
            else if (role == "student") {
                // Student menu handled in AuthSystem::login via StudentUser::showMenu()
            }
        }
        else {
            cout << "Login failed. Invalid username or password.\n";
        }

        cout << "\nDo you want to exit the program? (y/n): ";
        char exitChoice;
        cin >> exitChoice;
        cin.ignore();  // Clear buffer after char input
        if (exitChoice == 'y' || exitChoice == 'Y') {
            running = false;
        }
    }

    // Save data before exiting
    dataManager.saveStudents(students);
    dataManager.saveCourses(courses);
    dataManager.saveEnrollments(enrollments);

    // Clean up dynamic memory
    cleanupEnrollments();

    cout << "\nThank you for using Pokeno South Primary School Student Management System.\n";
    return 0;
}