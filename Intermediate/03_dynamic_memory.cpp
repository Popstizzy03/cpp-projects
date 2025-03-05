// Intermediate/03_dynamic_memory.cpp
#include <iostream>

using namespace std;

int main() {
    int* array = new int[5];
    for (int i = 0; i < 5; ++i) {
        array[i] = i * 2;
    }
    for (int i = 0; i < 5; ++i) {
        cout << array[i] << " ";
    }

    cout << endl;
    delete[] array;

    return 0;
}