#include <iostream>
#include <limits>
using namespace std;

#include "ambulance_dispatcher/ambulance_dispatcher.hpp"
#include "patient_admission/patient_admission.hpp"
#include "medical_supply/medical_supply.hpp"
#include "emergency_department/emergency_department.hpp"

int main() {
    int choice;

    do {
        cout << "\n========================================\n";
        cout << " Hospital Patient Care Management System\n";
        cout << "========================================\n";
        cout << "1. Patient Admission Clerk\n";
        cout << "2. Medical Supply Manager\n";
        cout << "3. Emergency Department Officer\n";
        cout << "4. Ambulance Dispatcher\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            patient_admission();
            break;
        case 2:
            medical_supply();
            break;
        case 3:
            emergency_department();  // runs your friend’s full system
            break;
        case 4:
            ambulance_dispatcher();  // your circular queue system
            break;
        case 0:
            cout << "Exiting system. Goodbye.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
