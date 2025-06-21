// Beginner/25_simple_vowel_counter.cpp
#include <iostream>
#include <string>
#include <cctype>

int main() {
    std::string text;
    std::cout << "Enter a string: ";
    std::getline(std::cin, text);
    int vowelCount = 0;
    for (char c : text) {
        c = std::tolower(c);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowelCount++;
        }
    }
    std::cout << "Vowel count: " << vowelCount << std::endl;
    return 0;
}
