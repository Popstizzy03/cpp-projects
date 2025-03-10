// Intermediate/10_maps.cpp
#include <iostream>
#include <map>

using namespace std;

int main() {
    map<string, int> ages;
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    cout << "Alices's age: " << ages["Alice"] << endl;
    return 0;
}