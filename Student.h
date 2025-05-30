#pragma once
#include <string>
#include <iostream>

class Student {
private:
    int rollno;
    std::string name;
    std::string address;
    std::string contact;

public:
    Student(int roll, const std::string& n,
        const std::string& addr, const std::string& cnt);

    // Getters
    int getRollno() const;
    std::string getName() const;
    std::string getAddress() const;
    std::string getContact() const;

    void setRollno(int newRollno);
    void setName(const std::string& n);
    void setAddress(const std::string& addr);
    void setContact(const std::string& cnt);

    void display() const;
};