// Intermediate/02_pointers.cpp
#include <iostream>

int main() {
    int number = 10;
    int* ptr = &number;

    std::cout << "Value: " << *ptr << std::endl;
    *ptr = 20;
    std::cout << "New Value: " << number << std::endl;
    return 0;
}