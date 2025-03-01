// Beginner/12_sum_of_array.cpp
#include <iostream>

using namespace std;

int main() {
    int numbers[] = {10, 20, 30, 40, 50};
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sum += numbers[i];
    }

    cout << "Sum of array elements: " << sum << endl;
    return 0;
}