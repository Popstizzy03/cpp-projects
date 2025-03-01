// Beginner/13_find_max_in_array.cpp
#include <iostream>

using namespace std;

int main() {
    int numbers[] = {5, 6, 26, 7, 199};
    int max = numbers[0];
    for (int i = 1; i < 5; ++i) {
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }
    cout << "Maximum Element: " << max << endl;
    return 0;
}