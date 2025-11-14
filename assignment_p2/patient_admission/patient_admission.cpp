#include <iostream>
#include <limits>
#include "patient_admission.hpp"

using namespace std;

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void patient_admission() {
    int choice;
    do {
        cout << "\n===== Patient Admission Clerk Menu =====\n";
        cout << "1. Admit Patient\n";
        cout << "2. Discharge Patient\n";
        cout << "3. View Patient Queue\n";
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
                 << "the Patient Admission Clerk team member.\n";
        }

    } while (choice != 0);
}
