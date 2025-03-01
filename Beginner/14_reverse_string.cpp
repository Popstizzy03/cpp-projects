// Beginner/14_reverse_string.cpp
#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string text;
    std::cout << "Enter a string: ";
    std::getline(std::cin, text);

    std::reverse(text.begin(), text.end());
    std::cout << "Reversed string: " << text << std::endl;
    return 0;
}