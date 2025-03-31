#include "Student.h"

Student::Student(int roll, const std::string& n,
    const std::string& addr, const std::string& cnt)
    : rollno(roll), name(n), address(addr), contact(cnt) {
}

int Student::getRollno() const { return rollno; }
std::string Student::getName() const { return name; }
std::string Student::getAddress() const { return address; }
std::string Student::getContact() const { return contact; }

void Student::display() const {
    std::cout << "Roll No: " << rollno << "\n"
        << "Name: " << name << "\n"
        << "Address: " << address << "\n"
        << "Contact: " << contact << "\n";
}