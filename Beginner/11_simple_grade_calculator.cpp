// Beginner/11_simple_grade_calculator.cpp
#include <iostream>

using namespace std;

int main() {
    int score;
    cout << "Enter your score: ";
    cin >> score;

    char grade;
    if (score >= 90) {
        grade = 'A';
    } else if (score >= 80) {
        grade = 'B';
    } else if (score >= 70) {
        grade = 'C';
    } else if (grade >= 60) {
        grade = 'D';
    } else {
        grade = 'F';
    }

    cout << "Your grade is: " << grade << endl;
    return 0;
}