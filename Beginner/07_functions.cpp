// Beginner/07_functions.cpp
#include <iostream>

using namespace std;

int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3);
    cout << "5 + 3 = " << result << endl;
    return 0;
}