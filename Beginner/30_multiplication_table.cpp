#include <iostream>
using namespace std;

int main() {
    int number, range;
    
    cout << "Enter a number for multiplication table: ";
    cin >> number;
    cout << "Enter the range (up to which number): ";
    cin >> range;
    
    cout << "\nMultiplication Table for " << number << ":" << endl;
    cout << "================================" << endl;
    
    for (int i = 1; i <= range; i++) {
        cout << number << " x " << i << " = " << (number * i) << endl;
    }
    
    cout << "\nBonus: Square and Cube values:" << endl;
    cout << "Square of " << number << " = " << (number * number) << endl;
    cout << "Cube of " << number << " = " << (number * number * number) << endl;
    
    return 0;
}