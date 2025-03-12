// Intermediate/16_binary_search.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> numbers = {1, 3, 4, 5, 7, 8, 9};
    if (binary_search(numbers.begin(), numbers.end(), 5)) {
        cout << "Found! 5" << endl;
    }
    return 0;
}