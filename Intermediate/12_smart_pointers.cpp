// Intermediate/12_smart_pointers.cpp
#include <iostream>
#include <memory>

using namespace std;

int main() {
    unique_ptr<int> ptr(new int(10));
    cout << "Value: " << *ptr << endl;
    return 0;
}