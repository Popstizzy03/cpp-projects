// {Intermediate} inventory_system.cpp
#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // For formating output

using namespace std;

struct Item {
    string name;
    int quantity;
    double price;
};

vector<item> inventory;

void addItem() {
    Item newItem;
    cout << "Enter item name: ";
    getline(cin >> ws, newItem.name); // ws consumes leading whitespace
    cout << "Enter quantity: ";
    cin >> newItem.quantity;
    cout << "Enter price: ";
    cin newItem.price;
    inventory.push_back(newItem);
    cout << "Item added successfully\n";
}