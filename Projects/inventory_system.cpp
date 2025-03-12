// Intermediate/InventorySystem/inventory_system.cpp
#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // For formatting output

struct Item {
    std::string name;
    int quantity;
    double price;
};

std::vector<Item> inventory;

void addItem() {
    Item newItem;
    std::cout << "Enter item name: ";
    std::getline(std::cin >> std::ws, newItem.name); // std::ws consumes leading whitespace
    std::cout << "Enter quantity: ";
    std::cin >> newItem.quantity;
    std::cout << "Enter price: ";
    std::cin >> newItem.price;
    inventory.push_back(newItem);
    std::cout << "Item added successfully!\n";
}

void viewInventory() {
    if (inventory.empty()) {
        std::cout << "Inventory is empty.\n";
        return;
    }
    std::cout << std::left << std::setw(20) << "Name" << std::setw(10) << "Quantity" << std::setw(10) << "Price" << std::endl;
    for (const auto& item : inventory) {
        std::cout << std::left << std::setw(20) << item.name << std::setw(10) << item.quantity << std::setw(10) << item.price << std::endl;
    }
}

void updateItem() {
    std::string itemName;
    std::cout << "Enter item name to update: ";
    std::getline(std::cin >> std::ws, itemName);

    for (auto& item : inventory) {
        if (item.name == itemName) {
            std::cout << "Enter new quantity: ";
            std::cin >> item.quantity;
            std::cout << "Enter new price: ";
            std::cin >> item.price;
            std::cout << "Item updated successfully!\n";
            return;
        }
    }
    std::cout << "Item not found.\n";
}

int main() {
    int choice;
    do {
        std::cout << "\nInventory Management System\n";
        std::cout << "1. Add Item\n";
        std::cout << "2. View Inventory\n";
        std::cout << "3. Update Item\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: addItem(); break;
            case 2: viewInventory(); break;
            case 3: updateItem(); break;
            case 4: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 4);
    return 0;
}