// Intermediate/01_classes_objects.cpp
#include <iostream>
#include <string>

using namespace std;

class Car {
    public:
    string brand;
    string model;
    int year;

    void displayInfo() {
        cout << "Brand: " << brand << ", Model: " << model << ", Year " << year << endl;
    }
};

int main() {
    Car myCar;
    myCar.brand = "GNX";
    myCar.model = "Buick GNX";
    myCar.year = 1987;
    myCar.displayInfo();
    return 0;
}