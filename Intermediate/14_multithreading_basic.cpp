// Intermediate/14_multithreading_basic.cpp
#include <iostream>
#include <thread>

using namespace std;

void printMessage() {
    cout << "Hello, From Threads!" << endl;
}

int main() {
    thread t(printMessage);
    t.join();
    return 0;
}