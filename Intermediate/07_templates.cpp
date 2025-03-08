// Intermediate/07_templates.cpp
#include <iostream>

using namespace std;

template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    cout << add(5, 3) << endl;
    cout << add(5.5, 2.5) << endl;
    return 0;
}