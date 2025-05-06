// 24_simple_character_counter.cpp
#include <iostream>
#include <string>

int main() {
    std::string text;
    std::cout << "Enter a string: ";
    std::getline(std::cin, text);
    std::cout << "Length: " << text.length() << std::endl;
    return 0;
}