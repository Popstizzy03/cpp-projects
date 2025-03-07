// Intermediate/06_inheritance.cpp
#include <iostream>

using namespace std;

class Animal {
    public:
        void eat() {
            cout << "Animal Eating." << endl;
        }
};

class Dog : public Animal {
    public:
        void bark() {
            cout << "Dog Barking." << endl;
        }
};

int main() {
    Dog myDog;
    myDog.eat();
    myDog.bark();
    return 0;
}