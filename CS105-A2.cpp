#include <iostream>
#include "Student.h"
#include "Course.h"
#include "Enrolment.h"

using namespace std;

int main() {
    // Test data
    Student student1(101, "Sami jan", "Kabul", "0781234567");
    Course math("Math", "M101", "Mathematics", "Mr. John");

    student1.display();
    math.display();

    PrimaryGrade aliceGrade;
    aliceGrade.setInternalMark(85);
    aliceGrade.setFinalMark(90);

    cout << "Alice's total grade: " << aliceGrade.calculateGrade() << endl;

    Enrolment enrolment1(student1, math);
    enrolment1.addGrade(new PrimaryGrade(aliceGrade));
    enrolment1.generateReport();

    return 0;
}