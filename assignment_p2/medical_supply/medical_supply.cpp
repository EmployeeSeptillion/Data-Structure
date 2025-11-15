#include <iostream>
#include "medical_supply.hpp"
#include <limits>
using namespace std;

void medical_supply() {
    SupplyStack supplyStack;
    supplyStack.loadFromFile(); // Load existing supplies

    int choice;
    do {
        cout << "\n===== Medical Supply Management Menu =====\n";
        cout << "1. Add Supply Stock\n";
        cout << "2. Use Last Added Supply\n";
        cout << "3. View Current Supplies\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string batchCode;

            // Clear input buffer before taking batch input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Batch Code: ";
            getline(cin, batchCode); // read full batch line correctly

            // Make sure batch is not empty
            if (batchCode.empty()) {
                cout << "Batch code cannot be empty!\n";
                continue;
            }

            char addMore = 'y';
            while (tolower(addMore) == 'y') {
                Supply s;
                s.batch = batchCode; // use same batch for all supplies

                cout << "Enter Supply Type: ";
                getline(cin, s.type);

                // Validate type input
                if (s.type.empty()) {
                    cout << "Supply type cannot be empty!\n";
                    continue;
                }

                cout << "Enter Quantity: ";
                while (!(cin >> s.quantity)) {  // ensures valid integer input
                    cout << "Please enter a valid number for quantity: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear after number input

                supplyStack.push(s);

                cout << "Add another supply for the same batch? (y/n): ";
                cin >> addMore;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean up newline
            }
        }


        else if (choice == 2) {
            supplyStack.pop();
        } 
        else if (choice == 3) {
            supplyStack.show();
        } 
        else if (choice == 0) {
            cout << "\nSaving and exiting program...\n";
            supplyStack.saveToFile();
        } 
        else {
            cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}
