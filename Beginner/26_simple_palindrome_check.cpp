// Beginner/26_simple_palindrome_check.cpp
#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string text, reversedText;
    std::cout << "Enter a string: ";
    std::getline(std::cin, text);
    reversedText = text;
    std::reverse(reversedText.begin(), reversedText.end());
    if (text == reversedText) {
        std::cout << "Palindrome" << std::endl;
    } else {
        std::cout << "Not a palindrome" << std::endl;
    }
    return 0;
}
