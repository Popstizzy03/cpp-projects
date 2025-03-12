// 17_sorting_vectors.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> numbers = {6, 7, 2, 9, 10, 1, 100};
    sort(numbers.begin(), numbers.end());
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;
    return 0;
}