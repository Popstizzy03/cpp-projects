// Intermediate/11_sets.cpp
#include <iostream>
#include <set>

using namespace std;

int main() {
    set<int> numbers = {1, 2, 3, 4, 5};
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;
    return 0;
}