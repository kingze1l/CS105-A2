#include <iostream>
#include <vector>
#include "Student.h"
#include "Course.h"
#include "Enrolment.h"
#include "AuthSystem.h"
#include "Admin.h"
#include "Teacher.h"
#include "StudentUser.h"
#include "DataManager.h"  // ADDED: Include the DataManager header

using namespace std;

// Global data stores
vector<Student> students;
vector<Course> courses;
vector<Enrolment*> enrollments;

// ADDED: This function initializes test data if needed
void initializeTestData() {
    // Add some test data if files are empty
    if (students.empty()) {
        students.emplace_back(101, "Ali Khan", "123 Main St", "0700112233");
        students.emplace_back(102, "Sara Ahmed", "456 Oak Ave", "0722334455");
    }

    if (courses.empty()) {
        courses.emplace_back("Mathematics", "MATH101", "Basic Calculus", "Dr. Smith");
        courses.emplace_back("English", "ENG101", "Grammar and Composition", "Ms. Johnson");
    }

    if (enrollments.empty()) {
        // Create some enrollments
        enrollments.push_back(new Enrolment(students[0], courses[0]));
        enrollments.push_back(new Enrolment(students[0], courses[1]));

        // Add sample grades
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
    // ADDED: Create data manager
    DataManager dataManager;

    // ADDED: Try to load data from files
    bool studentsLoaded = dataManager.loadStudents(students);
    bool coursesLoaded = dataManager.loadCourses(courses);
    bool enrollmentsLoaded = dataManager.loadEnrollments(enrollments, students, courses);

    // ADDED: If any data couldn't be loaded, initialize with test data
    if (!studentsLoaded || !coursesLoaded || !enrollmentsLoaded) {
        initializeTestData();
    }

    // Create authentication system
    AuthSystem authSystem;

    // Main login loop
    string username, password;
    bool running = true;

    while (running) {
        cout << "\n===== Pokeno South Primary School - Student Management System =====\n";
        cout << "Login\n";
        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;

        User* currentUser = authSystem.login(username, password);

        if (currentUser) {
            // User is authenticated, show appropriate menu based on role
            string role = currentUser->getRole();

            if (role == "admin") {
                Admin* admin = dynamic_cast<Admin*>(currentUser);
                if (admin) {
                    bool adminRunning = true;
                    while (adminRunning) {
                        int choice;
                        admin->showMenu();
                        cin >> choice;

                        switch (choice) {
                        case 1:
                            admin->createStudent(students);
                            break;
                        case 2:
                            admin->createCourse(courses);
                            break;
                        case 3:
                            // View all users functionality
                            break;
                        case 4:
                            adminRunning = false;
                            break;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                        }
                    }
                }
            }
            else if (role == "teacher") {
                Teacher* teacher = dynamic_cast<Teacher*>(currentUser);
                if (teacher) {
                    teacher->showMenu();
                    // Additional teacher functionality can be added here
                }
            }
            else if (role == "student") {
                // Student functionality is already handled in StudentUser::showMenu()
            }
        }
        else {
            cout << "Login failed. Invalid username or password.\n";
        }

        cout << "\nDo you want to exit the program? (y/n): ";
        char exitChoice;
        cin >> exitChoice;
        if (exitChoice == 'y' || exitChoice == 'Y') {
            running = false;
        }
    }

    // ADDED: Save data before exiting
    dataManager.saveStudents(students);
    dataManager.saveCourses(courses);
    dataManager.saveEnrollments(enrollments);

    // Clean up dynamic memory
    cleanupEnrollments();

    cout << "\nThank you for using Pokeno South Primary School Student Management System.\n";
    return 0;
}