// Intermediate/15_lambda_expressions.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5};
    for_each(numbers.begin(), numbers.end(), [](int num) {
        cout << num * 2 << " ";
    });
    cout << endl;
    return 0;
}