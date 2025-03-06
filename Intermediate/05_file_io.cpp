// Intermediate/05_file_io.cpp
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ofstream outFile("output.txt");
    outFile << "Hello, File I/O!" << endl;
    outFile.close();

    ifstream inFile("output.txt");
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
    return 0;
}