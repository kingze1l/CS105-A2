#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <conio.h> // For _getch()
#include "Student.h"
#include "Course.h"
#include "Enrolment.h"
#include "AuthSystem.h"
#include "Admin.h" // Updated to include getPasswordWithMask declaration
#include "Teacher.h"
#include "StudentUser.h"
#include "DataManager.h"

using namespace std;

vector<Student> students;
vector<Course> courses;
vector<Enrolment*> enrollments;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string getPasswordWithMask() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' is Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Erase last asterisk
            }
        }
        else if (ch >= 32 && ch <= 126) { // Printable ASCII characters
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

void displayHeader(const string& title) {
    clearScreen();
    cout << "\n=======================================\n";
    cout << "  " << title << "\n";
    cout << "=======================================\n\n";
}

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
        authSystem.clearUsers();
        for (auto& user : loadedUsers) {
            authSystem.addUser(std::move(user));
        }
    }

    bool student1Exists = false;
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == "student1") {
            student1Exists = true;
            break;
        }
    }
    if (!student1Exists) {
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
        displayHeader("Pokeno South Primary School - Student Management System");
        cout << "Welcome! Please select an option:\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter your choice (1-2): ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            displayHeader("Login");
            cout << "Available Roles: Admin, Teacher, Student\n";
            cout << "Default Credentials:\n";
            cout << "- Admin: admin/admin123\n";
            cout << "- Teacher: teacher/teacher123\n";
            cout << "- Student: student1/student123\n\n";
            cout << "Username: "; getline(cin, username);
            cout << "Password: "; password = getPasswordWithMask();

            User* currentUser = authSystem.login(username, password);

            if (currentUser) {
                string role = currentUser->getRole();
                if (role == "admin") {
                    Admin* admin = dynamic_cast<Admin*>(currentUser);
                    if (admin) {
                        bool adminRunning = true;
                        while (adminRunning) {
                            admin->showMenu();
                            int adminChoice;
                            if (!(std::cin >> adminChoice)) {
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                std::cout << "Invalid input. Please enter a number.\n";
                                std::cout << "Press Enter to continue...";
                                std::cin.ignore();
                                continue;
                            }
                            std::cin.ignore();
                            switch (adminChoice) {
                            case 1: admin->createStudent(authSystem, students); break;
                            case 2: admin->createTeacher(authSystem); break;
                            case 3: admin->createCourse(courses, authSystem); break;
                            case 4: admin->ViewAllUsers(authSystem.getUsers()); break;
                            case 5: admin->viewAllStudents(students); break;
                            case 6: admin->enrollStudent(enrollments, students, courses, authSystem); break;
                            case 7: admin->editStudent(students, authSystem); break;
                            case 8: admin->deleteStudent(students, authSystem, enrollments); break;
                            case 9: admin->editCourse(courses, authSystem); break;
                            case 10: admin->deleteCourse(courses, enrollments, authSystem); break;
                            case 11: admin->editEnrolment(enrollments, students, courses, authSystem); break;
                            case 12: admin->deleteEnrolment(enrollments, authSystem); break;
                            case 13: admin->editUser(authSystem, students, courses); break;
                            case 14: admin->deleteUser(authSystem, students, courses, enrollments); break;
                            case 15: admin->generateSystemReport(students, courses, enrollments); break; // New case
                            case 16: adminRunning = false; break;
                            default: std::cout << "Invalid choice. Please try again.\n";
                            }
                            if (adminChoice != 16) {
                                std::cout << "\nPress Enter to continue...";
                                std::cin.ignore();
                            }
                        }
                    }
                }
                else if (role == "teacher") {
                    Teacher* teacher = dynamic_cast<Teacher*>(currentUser);
                    if (teacher) {
                        bool teacherRunning = true;
                        while (teacherRunning) {
                            displayHeader("Teacher Menu - Welcome, " + teacher->getTeacherName());
                            teacher->showMenu();
                            int teacherChoice;
                            if (!(cin >> teacherChoice)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid input. Please enter a number.\n";
                                cout << "Press Enter to continue...";
                                cin.ignore();
                                continue;
                            }
                            cin.ignore();
                            switch (teacherChoice) {
                            case 1:
                                teacher->showAssignedCourses(courses);
                                break;
                            case 2: {
                                displayHeader("Grade Assignment");
                                cout << "--- Select Course to Grade ---\n";
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
                                if (!(cin >> courseChoice)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid input. Please enter a number.\n";
                                    break;
                                }
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
                                if (!(cin >> studentChoice)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid input. Please enter a number.\n";
                                    break;
                                }
                                cin.ignore();
                                if (studentChoice < 1 || studentChoice > static_cast<int>(courseEnrollments.size())) {
                                    cout << "Invalid student selection.\n";
                                    break;
                                }
                                teacher->gradeAssignment(*courseEnrollments[studentChoice - 1]);
                                break;
                            }
                            case 3: {
                                displayHeader("Edit Grades");
                                cout << "--- Select Course to Edit Grades ---\n";
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
                                if (!(cin >> courseChoice)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid input. Please enter a number.\n";
                                    break;
                                }
                                cin.ignore();
                                if (courseChoice < 1 || courseChoice > static_cast<int>(teacherCourses.size())) {
                                    cout << "Invalid course selection.\n";
                                    break;
                                }
                                cout << "\n--- Select Student to Edit Grades ---\n";
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
                                if (!(cin >> studentChoice)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid input. Please enter a number.\n";
                                    break;
                                }
                                cin.ignore();
                                if (studentChoice < 1 || studentChoice > static_cast<int>(courseEnrollments.size())) {
                                    cout << "Invalid student selection.\n";
                                    break;
                                }
                                teacher->editGrade(*courseEnrollments[studentChoice - 1]);
                                break;
                            }
                            case 4: {
                                displayHeader("Delete Grades");
                                cout << "--- Select Course to Delete Grades ---\n";
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
                                if (!(cin >> courseChoice)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid input. Please enter a number.\n";
                                    break;
                                }
                                cin.ignore();
                                if (courseChoice < 1 || courseChoice > static_cast<int>(teacherCourses.size())) {
                                    cout << "Invalid course selection.\n";
                                    break;
                                }
                                cout << "\n--- Select Student to Delete Grades ---\n";
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
                                if (!(cin >> studentChoice)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid input. Please enter a number.\n";
                                    break;
                                }
                                cin.ignore();
                                if (studentChoice < 1 || studentChoice > static_cast<int>(courseEnrollments.size())) {
                                    cout << "Invalid student selection.\n";
                                    break;
                                }
                                teacher->deleteGrade(*courseEnrollments[studentChoice - 1]);
                                break;
                            }
                            case 5:
                                teacher->viewStudentsAndGenerateReports(courses, enrollments);
                                break;
                            case 6:
                                teacherRunning = false;
                                break;
                            default:
                                cout << "Invalid choice. Please try again.\n";
                            }
                            if (teacherChoice != 6) {
                                cout << "\nPress Enter to continue...";
                                cin.ignore();
                            }
                        }
                    }
                }
                else if (role == "student") {
                    StudentUser* student = dynamic_cast<StudentUser*>(currentUser);
                    if (student) {
                        displayHeader("Student Portal - Welcome, " + student->getStudent().getName());
                        student->showMenu();
                        currentUser = nullptr; // Reset to fix looping issue
                    }
                }
            }
            else {
                cout << "\nLogin failed. Invalid username or password.\n";
                cout << "Press Enter to continue...";
                cin.ignore();
            }
        }
        else if (choice == 2) {
            running = false;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
            cout << "Press Enter to continue...";
            cin.ignore();
        }
    }

    dataManager.saveStudents(students);
    dataManager.saveCourses(courses);
    dataManager.saveEnrollments(enrollments);
    dataManager.saveUsers(authSystem.getUsers());

    cleanupEnrollments();

    displayHeader("Goodbye");
    cout << "Thank you for using Pokeno South Primary School Student Management System.\n";
    return 0;
}