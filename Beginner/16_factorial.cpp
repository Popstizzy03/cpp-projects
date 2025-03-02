// Beginner/16_factorial.cpp
#include <iostream>

using namespace std;

int main() {
    int number;
    cout << "Enter a positive integer: ";
    cin >> number;

    if (number < 0) {
        cout << "Factorial is not defined for negative numbers." << endl;
        return 1;
    }

    long long factorial = 1; // Use long long for larger factorials
    for (int i = 1; i <= number; ++i) {
        factorial *= i;
    }

    cout << "Factorial of " << number << " is: " << factorial << endl;
    return 0;
}