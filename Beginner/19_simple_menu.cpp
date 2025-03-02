// Beginner/19_simple_menu.cpp
#include <iostream>

using namespace std;

int main() {
    int choice;

    do {
        cout << "Menu:\n";
        cout << "1. Option 1\n";
        cout << "2. Option 2\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: cout << "Option 1 selected.\n"; break;
            case 2: cout << "Option 2 selected.\n"; break;
            case 3: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);

    return 0;
}