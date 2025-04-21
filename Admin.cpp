#include "Admin.h"
#include "Student.h"
#include "Course.h"
#include "StudentUser.h"
#include "Teacher.h"
#include "PasswordDisplay.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <conio.h> // For _getch()

Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd, "admin") {
}

void Admin::printAdminHeader() const {
    std::cout << "\n------ Admin Menu ------\n";
    std::cout << "Welcome, " << getUsername() << "!\n";
}

void Admin::showMenu() {
    printAdminHeader();
    std::cout << "1. Create Student Account\n"
        << "2. Create Teacher Account\n"
        << "3. Create Course\n"
        << "4. View All Users\n"
        << "5. View All Students\n"
        << "6. Enroll Student in Course\n"
        << "7. Edit Student\n"
        << "8. Delete Student\n"
        << "9. Edit Course\n"
        << "10. Delete Course\n"
        << "11. Edit Enrollment\n"
        << "12. Delete Enrollment\n"
        << "13. Edit User\n"
        << "14. Delete User\n"
        << "15. Generate System Report\n" // New option
        << "16. Exit\n"
        << "Enter your choice (1-16): ";
}
void Admin::createStudent(AuthSystem& authSystem, std::vector<Student>& students) {
    std::string username, password, name, address, contact;
    int rollno;

    std::cout << "\n------ Create Student Account ------\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    if (username.empty()) {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == username) {
            std::cout << "Error: Username already taken!\n";
            return;
        }
    }
    std::cout << "Password: "; password = getPasswordWithMask();
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }
    std::cout << "Enter Roll No: ";
    if (!(std::cin >> rollno)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid roll number.\n";
        return;
    }
    std::cin.ignore();
    if (rollno <= 0) {
        std::cout << "Error: Roll number must be positive.\n";
        return;
    }
    for (const auto& student : students) {
        if (student.getRollno() == rollno) {
            std::cout << "Error: Roll number already exists!\n";
            return;
        }
    }
    std::cout << "Enter Name: "; std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "Error: Name cannot be empty.\n";
        return;
    }
    std::cout << "Enter Address: "; std::getline(std::cin, address);
    std::cout << "Enter Contact: "; std::getline(std::cin, contact);

    students.emplace_back(rollno, name, address, contact);
    authSystem.addUser(std::make_unique<StudentUser>(username, password, students.back()));
    std::cout << "Success: Student account created successfully!\n";
}

void Admin::createTeacher(AuthSystem& authSystem) {
    std::string username, password, name;

    std::cout << "\n------ Create Teacher Account ------\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    if (username.empty()) {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }
    for (const auto& user : authSystem.getUsers()) {
        if (user->getUsername() == username) {
            std::cout << "Error: Username already taken!\n";
            return;
        }
    }
    std::cout << "Password: "; password = getPasswordWithMask();
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }
    std::cout << "Teacher's Full Name: "; std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "Error: Name cannot be empty.\n";
        return;
    }

    authSystem.addUser(std::make_unique<Teacher>(username, password, name));
    std::cout << "Success: Teacher account created successfully!\n";
}

void Admin::createCourse(std::vector<Course>& courses, AuthSystem& authSystem) {
    std::string name, code, description;

    std::cout << "\n------ Create New Course ------\n";
    std::cout << "Course Name: "; std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "Error: Course name cannot be empty.\n";
        return;
    }
    std::cout << "Course Code: "; std::getline(std::cin, code);
    if (code.empty()) {
        std::cout << "Error: Course code cannot be empty.\n";
        return;
    }
    for (const auto& course : courses) {
        if (course.getCourseCode() == code) {
            std::cout << "Error: Course code already exists!\n";
            return;
        }
    }
    std::cout << "Description: "; std::getline(std::cin, description);

    std::cout << "Available Teachers:\n";
    int i = 1;
    std::vector<std::string> teacherUsernames;
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "teacher") {
            Teacher* teacher = dynamic_cast<Teacher*>(user.get());
            std::cout << i++ << ". " << (teacher ? teacher->getTeacherName() : user->getUsername()) << "\n";
            teacherUsernames.push_back(user->getUsername());
        }
    }
    if (teacherUsernames.empty()) {
        std::cout << "Error: No teachers available. Create a teacher first.\n";
        return;
    }

    int choice;
    std::cout << "Select teacher (1-" << teacherUsernames.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(teacherUsernames.size())) {
        std::cout << "Error: Invalid teacher selection.\n";
        return;
    }

    courses.emplace_back(name, code, description, teacherUsernames[choice - 1]);
    std::cout << "Success: Course created successfully!\n";
}

void Admin::ViewAllUsers(const std::vector<std::unique_ptr<User>>& users) const {
    std::cout << "\n------ System Users ------\n";
    if (users.empty()) {
        std::cout << "No users in the system.\n";
        return;
    }
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(12) << "Username"
        << " | " << std::setw(13) << "Password"
        << " | " << std::setw(6) << "Role"
        << " | " << std::setw(11) << "Roll Number" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (const auto& user : users) {
        std::cout << "| " << std::left << std::setw(12) << user->getUsername()
            << " | " << std::setw(13) << PasswordDisplay::maskPassword(user->getPassword())
            << " | " << std::setw(6) << user->getRole();
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser) {
                int rollno = studentUser->getStudent().getRollno();
                std::cout << " | " << std::setw(11) << (rollno != 0 ? std::to_string(rollno) : "-");
            }
            else {
                std::cout << " | -";
            }
        }
        else {
            std::cout << " | -";
        }
        std::cout << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
}

void Admin::viewAllStudents(const std::vector<Student>& students) const {
    std::cout << "\n------ All Students ------\n";
    if (students.empty()) {
        std::cout << "No students in the system.\n";
        return;
    }
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(20) << "Name"
        << " | " << std::setw(10) << "Roll No"
        << " | " << std::setw(15) << "Contact" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < students.size(); ++i) {
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(20) << students[i].getName()
            << " | " << std::setw(10) << students[i].getRollno()
            << " | " << std::setw(15) << students[i].getContact() << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
}

void Admin::viewAllCourses(const std::vector<Course>& courses) const {
    std::cout << "\n------ All Courses ------\n";
    if (courses.empty()) {
        std::cout << "No courses in the system.\n";
        return;
    }
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(15) << "Course Code"
        << " | " << std::setw(20) << "Course Name"
        << " | " << std::setw(12) << "Teacher" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(15) << courses[i].getCourseCode()
            << " | " << std::setw(20) << courses[i].getCourseName()
            << " | " << std::setw(12) << courses[i].getTeacherUsername() << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
}

Student* Admin::selectStudent(const std::vector<Student>& students) const {
    viewAllStudents(students);
    if (students.empty()) return nullptr;
    int choice;
    std::cout << "Enter student number (1-" << students.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return nullptr;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(students.size())) {
        std::cout << "Error: Invalid selection.\n";
        return nullptr;
    }
    return const_cast<Student*>(&students[choice - 1]);
}

Course* Admin::selectCourse(const std::vector<Course>& courses) const {
    viewAllCourses(courses);
    if (courses.empty()) return nullptr;
    int choice;
    std::cout << "Enter course number (1-" << courses.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return nullptr;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(courses.size())) {
        std::cout << "Error: Invalid selection.\n";
        return nullptr;
    }
    return const_cast<Course*>(&courses[choice - 1]);
}

void Admin::enrollStudent(std::vector<Enrolment*>& enrollments,
    const std::vector<Student>& students,
    const std::vector<Course>& courses,
    const AuthSystem& authSystem) {
    std::cout << "\n------ Enroll Student in Course ------\n";
    Student* student = selectStudent(students);
    if (!student) {
        std::cout << "Enrollment cancelled: No student selected.\n";
        return;
    }

    Course* course = selectCourse(courses);
    if (!course) {
        std::cout << "Enrollment cancelled: No course selected.\n";
        return;
    }

    for (const auto* enrollment : enrollments) {
        if (enrollment->getStudent().getRollno() == student->getRollno() &&
            enrollment->getCourse().getCourseCode() == course->getCourseCode()) {
            std::cout << "Error: Student already enrolled in this course!\n";
            return;
        }
    }

    Enrolment* newEnrolment = new Enrolment(*student, *course);
    enrollments.push_back(newEnrolment);

    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser && studentUser->getStudent().getRollno() == student->getRollno()) {
                studentUser->addEnrolment(newEnrolment);
                break;
            }
        }
    }

    std::cout << "Success: Student enrolled successfully!\n";
}
void Admin::editStudent(std::vector<Student>& students, AuthSystem& authSystem) {
    std::cout << "\n------ Edit Student ------\n";
    Student* student = selectStudent(students);
    if (!student) {
        std::cout << "Edit cancelled: No student selected.\n";
        return;
    }

    std::cout << "\nCurrent Details:\n";
    student->display();

    std::string name, address, contact;
    std::cout << "Enter new Name (leave blank to keep '" << student->getName() << "'): ";
    std::getline(std::cin, name);
    std::cout << "Enter new Address (leave blank to keep '" << student->getAddress() << "'): ";
    std::getline(std::cin, address);
    std::cout << "Enter new Contact (leave blank to keep '" << student->getContact() << "'): ";
    std::getline(std::cin, contact);

    // Update only non-empty fields
    if (!name.empty()) student->setName(name); // Note: Need to add setName to Student class
    if (!address.empty()) student->setAddress(address); // Note: Need to add setAddress
    if (!contact.empty()) student->setContact(contact); // Note: Need to add setContact

    // Update associated StudentUser
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser && studentUser->getStudent().getRollno() == student->getRollno()) {
                // Update StudentUser's student object (may need copy or reference update)
                std::cout << "StudentUser updated.\n";
                break;
            }
        }
    }

    std::cout << "Success: Student details updated successfully!\n";
}

void Admin::deleteStudent(std::vector<Student>& students, AuthSystem& authSystem, std::vector<Enrolment*>& enrollments) {
    std::cout << "\n------ Delete Student ------\n";
    Student* student = selectStudent(students);
    if (!student) {
        std::cout << "Delete cancelled: No student selected.\n";
        return;
    }

    int rollno = student->getRollno();
    std::cout << "Are you sure you want to delete " << student->getName() << " (Roll No: " << rollno << ")? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Delete cancelled.\n";
        return;
    }

    // Remove associated enrollments
    auto it = enrollments.begin();
    while (it != enrollments.end()) {
        if ((*it)->getStudent().getRollno() == rollno) {
            delete* it; // Free memory
            it = enrollments.erase(it);
        }
        else {
            ++it;
        }
    }

    // Find and remove associated StudentUser
    std::string userToDelete;
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser && studentUser->getStudent().getRollno() == rollno) {
                userToDelete = user->getUsername();
                break;
            }
        }
    }

    if (!userToDelete.empty()) {
        authSystem.removeUser(userToDelete);
    }
    else {
        std::cout << "Warning: No associated StudentUser found for Roll No: " << rollno << "\n";
    }

    // Remove student from students vector
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getRollno() == rollno) {
            students.erase(it);
            break;
        }
    }

    std::cout << "Success: Student deleted successfully!\n";
}
void Admin::editCourse(std::vector<Course>& courses, AuthSystem& authSystem) {
    std::cout << "\n------ Edit Course ------\n";
    Course* course = selectCourse(courses);
    if (!course) {
        std::cout << "Edit cancelled: No course selected.\n";
        return;
    }

    std::cout << "\nCurrent Details:\n";
    course->display();

    std::string name, code, description, teacherUsername;
    std::cout << "Enter new Course Name (leave blank to keep '" << course->getCourseName() << "'): ";
    std::getline(std::cin, name);
    std::cout << "Enter new Course Code (leave blank to keep '" << course->getCourseCode() << "'): ";
    std::getline(std::cin, code);
    std::cout << "Enter new Description (leave blank to keep '" << course->getDescription() << "'): ";
    std::getline(std::cin, description);

    // Teacher selection
    std::cout << "\nAvailable Teachers:\n";
    int i = 1;
    std::vector<std::string> teacherUsernames;
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "teacher") {
            Teacher* teacher = dynamic_cast<Teacher*>(user.get());
            std::cout << i++ << ". " << (teacher ? teacher->getTeacherName() : user->getUsername()) << "\n";
            teacherUsernames.push_back(user->getUsername());
        }
    }
    if (teacherUsernames.empty()) {
        std::cout << "Error: No teachers available. Cannot change teacher.\n";
    }
    else {
        std::cout << "Select new teacher (1-" << teacherUsernames.size() << ", 0 to keep '"
            << course->getTeacherUsername() << "'): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: Invalid input.\n";
            return;
        }
        std::cin.ignore();
        if (choice >= 1 && choice <= static_cast<int>(teacherUsernames.size())) {
            teacherUsername = teacherUsernames[choice - 1];
        }
    }

    // Validate new course code (if provided)
    if (!code.empty()) {
        for (const auto& c : courses) {
            if (c.getCourseCode() == code && c.getCourseCode() != course->getCourseCode()) {
                std::cout << "Error: Course code already exists!\n";
                return;
            }
        }
    }

    // Update non-empty fields
    if (!name.empty()) course->setCourseName(name);
    if (!code.empty()) course->setCourseCode(code);
    if (!description.empty()) course->setDescription(description);
    if (!teacherUsername.empty()) course->setTeacherUsername(teacherUsername);

    std::cout << "Success: Course updated successfully!\n";
}
void Admin::deleteCourse(std::vector<Course>& courses, std::vector<Enrolment*>& enrollments, AuthSystem& authSystem) {
    std::cout << "\n------ Delete Course ------\n";
    Course* course = selectCourse(courses);
    if (!course) {
        std::cout << "Delete cancelled: No course selected.\n";
        return;
    }

    std::string courseCode = course->getCourseCode();
    std::cout << "Are you sure you want to delete " << course->getCourseName()
        << " (Code: " << courseCode << ")? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Delete cancelled.\n";
        return;
    }

    // Remove associated enrollments
    auto it = enrollments.begin();
    while (it != enrollments.end()) {
        if ((*it)->getCourse().getCourseCode() == courseCode) {
            delete* it; // Free memory
            it = enrollments.erase(it);
        }
        else {
            ++it;
        }
    }

    // Update StudentUser enrollments
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser) {
                studentUser->loadEnrollments(enrollments); // Refresh enrollments
            }
        }
    }

    // Remove course from courses vector
    for (auto it = courses.begin(); it != courses.end(); ++it) {
        if (it->getCourseCode() == courseCode) {
            courses.erase(it);
            break;
        }
    }

    std::cout << "Success: Course deleted successfully!\n";
}
Enrolment* Admin::selectEnrolment(const std::vector<Enrolment*>& enrollments) const {
    std::cout << "\n------ All Enrollments ------\n";
    if (enrollments.empty()) {
        std::cout << "No enrollments in the system.\n";
        return nullptr;
    }
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(20) << "Student Name"
        << " | " << std::setw(10) << "Roll No"
        << " | " << std::setw(15) << "Course" << " |\n";
    std::cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < enrollments.size(); ++i) {
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(20) << enrollments[i]->getStudent().getName()
            << " | " << std::setw(10) << enrollments[i]->getStudent().getRollno()
            << " | " << std::setw(15) << enrollments[i]->getCourse().getCourseName() << " |\n";
    }
    std::cout << "-------------------------------------------------\n";

    int choice;
    std::cout << "Enter enrollment number (1-" << enrollments.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return nullptr;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(enrollments.size())) {
        std::cout << "Error: Invalid selection.\n";
        return nullptr;
    }
    return enrollments[choice - 1];
}

void Admin::editEnrolment(std::vector<Enrolment*>& enrollments, const std::vector<Student>& students,
    const std::vector<Course>& courses, AuthSystem& authSystem) {
    std::cout << "\n------ Edit Enrollment ------\n";
    Enrolment* enrolment = selectEnrolment(enrollments);
    if (!enrolment) {
        std::cout << "Edit cancelled: No enrollment selected.\n";
        return;
    }

    std::cout << "\nCurrent Enrollment:\n";
    std::cout << "Student: " << enrolment->getStudent().getName()
        << " (Roll No: " << enrolment->getStudent().getRollno() << ")\n";
    std::cout << "Course: " << enrolment->getCourse().getCourseName()
        << " (" << enrolment->getCourse().getCourseCode() << ")\n";

    std::cout << "\nSelect new student or course (or keep existing):\n";
    Student* newStudent = selectStudent(students);
    Course* newCourse = selectCourse(courses);

    if (!newStudent && !newCourse) {
        std::cout << "Edit cancelled: No changes made.\n";
        return;
    }

    // Use existing values if not changed
    const Student& selectedStudent = newStudent ? *newStudent : enrolment->getStudent();
    const Course& selectedCourse = newCourse ? *newCourse : enrolment->getCourse();

    // Check for duplicate enrollment
    for (const auto* e : enrollments) {
        if (e != enrolment &&
            e->getStudent().getRollno() == selectedStudent.getRollno() &&
            e->getCourse().getCourseCode() == selectedCourse.getCourseCode()) {
            std::cout << "Error: This student is already enrolled in this course!\n";
            return;
        }
    }

    // If course changes, check if existing grade should be cleared
    Grade* existingGrade = enrolment->getGrade();
    if (newCourse && existingGrade) {
        std::cout << "Warning: Changing the course will clear existing grades. Proceed? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();
        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "Edit cancelled.\n";
            return;
        }
    }

    // Update enrollment by creating a new one and replacing the old
    Enrolment* newEnrolment = new Enrolment(selectedStudent, selectedCourse);
    if (existingGrade && !newCourse) {
        newEnrolment->addGrade(existingGrade); // Preserve grade if course unchanged
    }
    for (size_t i = 0; i < enrollments.size(); ++i) {
        if (enrollments[i] == enrolment) {
            delete enrollments[i]; // Free old enrollment
            enrollments[i] = newEnrolment;
            break;
        }
    }

    // Update StudentUser enrollments
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser &&
                (studentUser->getStudent().getRollno() == enrolment->getStudent().getRollno() ||
                    studentUser->getStudent().getRollno() == selectedStudent.getRollno())) {
                studentUser->loadEnrollments(enrollments); // Refresh enrollments
            }
        }
    }

    std::cout << "Success: Enrollment updated successfully!\n";
}
void Admin::deleteEnrolment(std::vector<Enrolment*>& enrollments, AuthSystem& authSystem) {
    std::cout << "\n------ Delete Enrollment ------\n";
    Enrolment* enrolment = selectEnrolment(enrollments);
    if (!enrolment) {
        std::cout << "Delete cancelled: No enrollment selected.\n";
        return;
    }

    std::cout << "\nEnrollment Details:\n";
    std::cout << "Student: " << enrolment->getStudent().getName()
        << " (Roll No: " << enrolment->getStudent().getRollno() << ")\n";
    std::cout << "Course: " << enrolment->getCourse().getCourseName()
        << " (" << enrolment->getCourse().getCourseCode() << ")\n";
    std::cout << "Are you sure you want to delete this enrollment? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Delete cancelled.\n";
        return;
    }

    // Remove enrollment from vector and free memory
    for (auto it = enrollments.begin(); it != enrollments.end(); ++it) {
        if (*it == enrolment) {
            delete* it; // Free memory (including grade)
            enrollments.erase(it);
            break;
        }
    }

    // Update StudentUser enrollments
    for (const auto& user : authSystem.getUsers()) {
        if (user->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(user.get());
            if (studentUser &&
                studentUser->getStudent().getRollno() == enrolment->getStudent().getRollno()) {
                studentUser->loadEnrollments(enrollments); // Refresh enrollments
            }
        }
    }

    std::cout << "Success: Enrollment deleted successfully!\n";
}
User* Admin::selectUser(const std::vector<std::unique_ptr<User>>& users) const {
    ViewAllUsers(users);
    if (users.empty()) return nullptr;
    int choice;
    std::cout << "Enter user number (1-" << users.size() << "): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        return nullptr;
    }
    std::cin.ignore();
    if (choice < 1 || choice > static_cast<int>(users.size())) {
        std::cout << "Error: Invalid selection.\n";
        return nullptr;
    }
    return users[choice - 1].get();
}

void Admin::editUser(AuthSystem& authSystem, std::vector<Student>& students, std::vector<Course>& courses) {
    std::cout << "\n------ Edit User ------\n";
    User* user = selectUser(authSystem.getUsers());
    if (!user) {
        std::cout << "Edit cancelled: No user selected.\n";
        return;
    }

    std::cout << "\nCurrent Details:\n";
    std::cout << "Username: " << user->getUsername() << "\n";
    std::cout << "Role: " << user->getRole() << "\n";

    std::string newUsername, newPassword;
    std::cout << "Enter new username (leave blank to keep '" << user->getUsername() << "'): ";
    std::getline(std::cin, newUsername);
    std::cout << "Enter new password (leave blank to keep current): ";
    newPassword = getPasswordWithMask();

    // Validate new username
    if (!newUsername.empty()) {
        for (const auto& u : authSystem.getUsers()) {
            if (u->getUsername() == newUsername && u.get() != user) {
                std::cout << "Error: Username already taken!\n";
                return;
            }
        }
    }

    // Update common fields
    if (!newUsername.empty()) user->setUsername(newUsername);
    if (!newPassword.empty()) user->setPassword(newPassword);

    // Role-specific edits
    if (user->getRole() == "teacher") {
        Teacher* teacher = dynamic_cast<Teacher*>(user);
        if (teacher) {
            std::string newName;
            std::cout << "Enter new teacher name (leave blank to keep '" << teacher->getTeacherName() << "'): ";
            std::getline(std::cin, newName);
            if (!newName.empty()) teacher->setTeacherName(newName);
        }
    }
    else if (user->getRole() == "student") {
        StudentUser* studentUser = dynamic_cast<StudentUser*>(user);
        if (studentUser) {
            Student* student = nullptr;
            for (auto& s : students) {
                if (s.getRollno() == studentUser->getStudent().getRollno()) {
                    student = &s;
                    break;
                }
            }
            if (student) {
                std::string name, address, contact;
                int newRollno;
                std::cout << "\nCurrent Student Details:\n";
                student->display();
                std::cout << "Enter new Roll No (0 to keep " << student->getRollno() << "): ";
                if (!(std::cin >> newRollno)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Error: Invalid input.\n";
                    return;
                }
                std::cin.ignore();
                std::cout << "Enter new Name (leave blank to keep '" << student->getName() << "'): ";
                std::getline(std::cin, name);
                std::cout << "Enter new Address (leave blank to keep '" << student->getAddress() << "'): ";
                std::getline(std::cin, address);
                std::cout << "Enter new Contact (leave blank to keep '" << student->getContact() << "'): ";
                std::getline(std::cin, contact);

                // Validate new roll number
                if (newRollno != 0 && newRollno != student->getRollno()) {
                    for (const auto& s : students) {
                        if (s.getRollno() == newRollno) {
                            std::cout << "Error: Roll number already exists!\n";
                            return;
                        }
                    }
                }

                // Update student details
                if (newRollno != 0) student->setRollno(newRollno);
                if (!name.empty()) student->setName(name);
                if (!address.empty()) student->setAddress(address);
                if (!contact.empty()) student->setContact(contact);
            }
        }
    }

    // Update courses if username changed (for teachers)
    if (!newUsername.empty() && user->getRole() == "teacher") {
        for (auto& course : courses) {
            if (course.getTeacherUsername() == user->getUsername()) {
                course.setTeacherUsername(newUsername);
            }
        }
    }

    std::cout << "Success: User updated successfully!\n";
}
void Admin::deleteUser(AuthSystem& authSystem, std::vector<Student>& students, std::vector<Course>& courses, std::vector<Enrolment*>& enrollments) {
    std::cout << "\n------ Delete User ------\n";
    User* user = selectUser(authSystem.getUsers());
    if (!user) {
        std::cout << "Delete cancelled: No user selected.\n";
        return;
    }

    // Prevent deletion of last admin
    if (user->getRole() == "admin") {
        int adminCount = 0;
        for (const auto& u : authSystem.getUsers()) {
            if (u->getRole() == "admin") adminCount++;
        }
        if (adminCount <= 1) {
            std::cout << "Error: Cannot delete the last admin account!\n";
            return;
        }
    }

    std::cout << "\nUser Details:\n";
    std::cout << "Username: " << user->getUsername() << "\n";
    std::cout << "Role: " << user->getRole() << "\n";
    std::cout << "Are you sure you want to delete this user? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Delete cancelled.\n";
        return;
    }

    // Handle role-specific dependencies
    if (user->getRole() == "student") {
        StudentUser* studentUser = dynamic_cast<StudentUser*>(user);
        if (studentUser) {
            int rollno = studentUser->getStudent().getRollno();
            // Remove enrollments
            auto it = enrollments.begin();
            while (it != enrollments.end()) {
                if ((*it)->getStudent().getRollno() == rollno) {
                    delete* it;
                    it = enrollments.erase(it);
                }
                else {
                    ++it;
                }
            }
            // Remove student
            for (auto it = students.begin(); it != students.end(); ++it) {
                if (it->getRollno() == rollno) {
                    students.erase(it);
                    break;
                }
            }
        }
    }
    else if (user->getRole() == "teacher") {
        // Check for assigned courses
        bool hasCourses = false;
        for (const auto& course : courses) {
            if (course.getTeacherUsername() == user->getUsername()) {
                hasCourses = true;
                break;
            }
        }
        if (hasCourses) {
            std::cout << "This teacher is assigned to courses. Reassign courses before deletion.\n";
            std::cout << "Available Teachers:\n";
            int i = 1;
            std::vector<std::string> teacherUsernames;
            for (const auto& u : authSystem.getUsers()) {
                if (u->getRole() == "teacher" && u.get() != user) {
                    Teacher* t = dynamic_cast<Teacher*>(u.get());
                    std::cout << i++ << ". " << (t ? t->getTeacherName() : u->getUsername()) << "\n";
                    teacherUsernames.push_back(u->getUsername());
                }
            }
            if (teacherUsernames.empty()) {
                std::cout << "Error: No other teachers available. Cannot delete without reassigning courses.\n";
                return;
            }
            int choice;
            std::cout << "Select new teacher for reassignment (1-" << teacherUsernames.size() << "): ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Error: Invalid input.\n";
                return;
            }
            std::cin.ignore();
            if (choice < 1 || choice > static_cast<int>(teacherUsernames.size())) {
                std::cout << "Error: Invalid teacher selection.\n";
                return;
            }
            // Reassign courses
            for (auto& course : courses) {
                if (course.getTeacherUsername() == user->getUsername()) {
                    course.setTeacherUsername(teacherUsernames[choice - 1]);
                }
            }
        }
    }

    // Delete user
    authSystem.removeUser(user->getUsername());

    // Update StudentUser enrollments (for consistency)
    for (const auto& u : authSystem.getUsers()) {
        if (u->getRole() == "student") {
            StudentUser* studentUser = dynamic_cast<StudentUser*>(u.get());
            if (studentUser) {
                studentUser->loadEnrollments(enrollments);
            }
        }
    }

    std::cout << "Success: User deleted successfully!\n";
}
void Admin::generateSystemReport(const std::vector<Student>& students, const std::vector<Course>& courses, const std::vector<Enrolment*>& enrollments) {
    std::cout << "\n------ System-Wide Report ------\n";

    // Open reports.txt in append mode
    std::ofstream outFile("reports.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cout << "Error: Could not open reports.txt for writing.\n";
        return;
    }

    // Get current timestamp
    std::time_t now = std::time(nullptr);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now);
    std::string timestamp(buffer);
    timestamp.pop_back();

    // Console and file output
    std::cout << "=== System-Wide Report ===\n";
    std::cout << "Generated: " << timestamp << "\n";
    outFile << "=== System-Wide Report ===\n";
    outFile << "Generated: " << timestamp << "\n";

    // System summary
    std::cout << "Total Students: " << students.size() << "\n";
    std::cout << "Total Courses: " << courses.size() << "\n";
    std::cout << "Total Enrollments: " << enrollments.size() << "\n";
    outFile << "Total Students: " << students.size() << "\n";
    outFile << "Total Courses: " << courses.size() << "\n";
    outFile << "Total Enrollments: " << enrollments.size() << "\n";

    if (enrollments.empty()) {
        std::cout << "No enrollments in the system.\n";
        outFile << "No enrollments in the system.\n";
        std::cout << "===================\n";
        outFile << "===================\n\n";
        outFile.close();
        return;
    }

    // Detailed enrollment report
    std::cout << "-------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(20) << "Student Name"
        << " | " << std::setw(10) << "Roll No"
        << " | " << std::setw(20) << "Course"
        << " | " << std::setw(12) << "Total Grade" << " |\n";
    std::cout << "-------------------------------------------------\n";
    outFile << "-------------------------------------------------\n";
    outFile << "| " << std::left << std::setw(5) << "No."
        << " | " << std::setw(20) << "Student Name"
        << " | " << std::setw(10) << "Roll No"
        << " | " << std::setw(20) << "Course"
        << " | " << std::setw(12) << "Total Grade" << " |\n";
    outFile << "-------------------------------------------------\n";

    float totalGrade = 0.0f;
    int gradedEnrollments = 0;
    for (size_t i = 0; i < enrollments.size(); ++i) {
        float grade = enrollments[i]->getGrade() ? enrollments[i]->getGrade()->calculateGrade() : 0.0f;
        if (enrollments[i]->getGrade()) {
            totalGrade += grade;
            gradedEnrollments++;
        }
        std::cout << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(20) << enrollments[i]->getStudent().getName()
            << " | " << std::setw(10) << enrollments[i]->getStudent().getRollno()
            << " | " << std::setw(20) << enrollments[i]->getCourse().getCourseName()
            << " | " << std::setw(12) << (enrollments[i]->getGrade() ? std::to_string(grade) : "N/A") << " |\n";
        outFile << "| " << std::left << std::setw(5) << (i + 1)
            << " | " << std::setw(20) << enrollments[i]->getStudent().getName()
            << " | " << std::setw(10) << enrollments[i]->getStudent().getRollno()
            << " | " << std::setw(20) << enrollments[i]->getCourse().getCourseName()
            << " | " << std::setw(12) << (enrollments[i]->getGrade() ? std::to_string(grade) : "N/A") << " |\n";
    }
    std::cout << "-------------------------------------------------\n";
    outFile << "-------------------------------------------------\n";

    // System statistics
    if (gradedEnrollments > 0) {
        float averageGrade = totalGrade / gradedEnrollments;
        std::cout << "Average Grade (All Enrollments): " << averageGrade << "\n";
        outFile << "Average Grade (All Enrollments): " << averageGrade << "\n";
    }
    else {
        std::cout << "Average Grade: N/A (no grades assigned)\n";
        outFile << "Average Grade: N/A (no grades assigned)\n";
    }
    std::cout << "===================\n";
    outFile << "===================\n\n";

    // Option to generate detailed student report
    std::cout << "Generate detailed report for a student? (1-" << enrollments.size() << ", 0 to skip): ";
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input.\n";
        outFile.close();
        return;
    }
    std::cin.ignore();

    if (choice >= 1 && choice <= static_cast<int>(enrollments.size())) {
        enrollments[choice - 1]->generateReport(outFile);
    }

    outFile.close();
    std::cout << "Report saved to reports.txt\n";
}