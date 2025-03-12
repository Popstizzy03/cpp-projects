// Intermediate/20_function_pointers.cpp
#include <iostream>

using namespace std;

int add(int a, int b) {
    return a + b;
}

int main() {
    int (*funcPtr)(int, int) = add;
    cout << "Result: " << funcPtr(5, 3) << endl;
    return 0;
}