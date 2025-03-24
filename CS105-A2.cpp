// CS105-A2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<string>
using namespace std;

//student class  creation and definition
class Student {
private:
	int rollno; //private attribute to encupsulate the data
	string name;
	string address;
	string contact;

public:
	Student(int roll, string n, string addr, string cnt) : rollno(roll), name(n), address(addr), contact(cnt) {} //constructor to initialize the data;

	//getter and setter methods to access the private data 
	int getRollno() {return rollno;}
	string getName() { return name; }
	string getAddress() { return address; }
	string getContact() { return contact; }

	//display student details method
	void display() {
		cout << "Roll No: " << rollno << endl;
		cout << "Name: " << name << endl;
		cout << "Address: " << address << endl;
		cout << "Contact: " << contact << endl;
	}
};

class Course {
private:
	string courseName;
	string courseCode;
	string description;
	string teacher;

public:
	Course(string cName, string cCode, string desc, string t) : courseName(cName), courseCode(cCode), description(desc), teacher(t) {} //constructor to initialize the data;
	//getter and setter methods to access the private data 
	string getCourseName() { return courseName; }
	string getCourseCode() { return courseCode; }
	string getDescription() { return description; }
	string getTeacher() { return teacher; }
	//display course details method
	void display() {
		cout << "Course Name: " << courseName << endl;
		cout << "Course Code: " << courseCode << endl;
		cout << "Description: " << description << endl;
		cout << "Teacher: " << teacher << endl;
	}
};



int main()
{

	Student student1(101, "Sami jan", "Kabul", "0781234567"); //creating object of student class
	student1.display();
	//test course 
	Course math("Math", "M101", "Mathematics", "Mr. John");
	math.display();
    std::cout << "Hello World!\n";
}

