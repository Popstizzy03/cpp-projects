// Intermediate/19_enums.cpp
#include <iostream>

enum class Color {
    Red,
    Blue,
    Yellow
};

int main() {
    Color myColor = Color::Green;
    if (myColor == Color::Green) {
        cout << "Color is Green" << endl;
    }
    return 0;
}