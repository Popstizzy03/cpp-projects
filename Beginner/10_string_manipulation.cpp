// Beginner/10_string_manipulation.cpp
#include <iostream>
#include <string>

using namespace std;

int main() {
    string text = "Hello, C++ World!";
    cout << "Length: " << text.length() << endl;
    cout << "Substring: " << text.substr(7, 3) << endl; // Extracts "C++"
    cout << "Final Word: " << text.find("World") << endl;
    return 0;
}