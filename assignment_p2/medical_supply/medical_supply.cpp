#include <iostream>
#include <limits>
#include "medical_supply.hpp"

using namespace std;

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void medical_supply() {
    int choice;
    do {
        cout << "\n===== Medical Supply Manager Menu =====\n";
        cout << "1. Add Supply Stock\n";
        cout << "2. Use 'Last Added' Supply\n";
        cout << "3. View Current Supplies\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            clearInput();
            continue;
        }
        clearInput();

        if (choice == 0) {
            cout << "Returning to main menu...\n";
        } else {
            cout << "[Placeholder] This feature will be implemented by "
                 << "the Medical Supply Manager team member.\n";
        }

    } while (choice != 0);
}
