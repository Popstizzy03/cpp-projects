#include <iostream>
using namespace std;

int main() {
    int n, first = 0, second = 1, next;
    
    cout << "Enter the number of terms for Fibonacci sequence: ";
    cin >> n;
    
    cout << "Fibonacci Sequence: ";
    
    if (n >= 1) {
        cout << first << " ";
    }
    if (n >= 2) {
        cout << second << " ";
    }
    
    for (int i = 3; i <= n; i++) {
        next = first + second;
        cout << next << " ";
        first = second;
        second = next;
    }
    
    cout << endl;
    return 0;
}