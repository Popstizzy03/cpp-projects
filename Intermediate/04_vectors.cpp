// Intermediate/04_vectors.cpp
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5};
    numbers.push_back(6);
    for (int num : numbers) {
        cout << num << " ";
    }

    cout << endl;
    return 0;
}