// Beginner/23_simple_average_calculator.cpp
#include <iostream>

int main() {
    int num1, num2, num3;
    std::cout << "Enter three numbers: ";
    std::cin >> num1 >> num2 >> num3;
    std::cout << "Average: " << (num1 + num2 + num3) / 3.0 << std::endl;
    return 0;
}
