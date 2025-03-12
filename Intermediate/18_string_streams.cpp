// Intermediate/18_string_streams.cpp
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    stringstream ss;
    ss << "Hello " << 123 << " World";
    string result = ss.str();
    cout << result << endl;
    return 0;
}