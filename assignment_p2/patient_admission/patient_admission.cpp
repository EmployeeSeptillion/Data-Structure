#include <iostream>
#include <limits>
#include <string>
#include "patient_admission.hpp"

using namespace std;

struct Patient {
    int id;
    string name;
    string condition;
    Patient* next;
};

class PatientQueue {
private:
    Patient* first;
    Patient* last;
    int nextID;  

public:
    PatientQueue() {
        first = last = nullptr;
        nextID = 1;
    }

    bool isEmpty() {
        return (first == nullptr);
    }

    void admitPatient(string name, string condition) {
        Patient* newPatient = new Patient;
        newPatient->id = nextID++;
        newPatient->name = name;
        newPatient->condition = condition;
        newPatient->next = nullptr;

        if (last == nullptr) {
            first = last = newPatient;
        } else {
            last->next = newPatient;
            last = newPatient;
        }

        cout << "Patient admitted successfully! (ID: " << newPatient->id << ")\n";
    }

    void dischargePatient() {
        if (isEmpty()) {
            cout << "No patients to discharge.\n";
            return;
        }

        Patient* temp = first;
        cout << "🩺 Discharging patient: " << temp->name << " (ID: " << temp->id << ")\n";

        first = first->next;
        if (first == nullptr) {
            last = nullptr;
        }

        delete temp;
    }

    void viewPatientQueue() {
        if (isEmpty()) {
            cout << "No patients in queue.\n";
            return;
        }

        cout << "\n===== Current Patient Queue =====\n";
        Patient* current = first;
        while (current != nullptr) {
            cout << "ID: " << current->id
                 << " | Name: " << current->name
                 << " | Condition: " << current->condition << endl;
            current = current->next;
        }
        cout << "=================================\n";
    }

    ~PatientQueue() {
        while (!isEmpty()) {
            dischargePatient();
        }
    }
};

void patient_admission() {
    PatientQueue pq;
    int choice;
    string name, condition;

    do {
        cout << "\n===== PATIENT ADMISSION MENU =====\n";
        cout << "1. Admit Patient\n";
        cout << "2. Discharge Patient\n";
        cout << "3. View Patient Queue\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        // Numeric validation
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Clear leftover newline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Range validation
        if (choice < 1 || choice > 4) {
            cout << "Invalid choice! Please enter a number between 1 and 4.\n";
            continue;
        }

        switch (choice) {
            case 1:
                cout << "Enter patient name: ";
                getline(cin, name);
                cout << "Enter condition type (e.g., Normal, Serious, Critical): ";
                getline(cin, condition);
                pq.admitPatient(name, condition);
                break;

            case 2:
                pq.dischargePatient();
                break;

            case 3:
                pq.viewPatientQueue();
                break;

            case 4:
                cout << "Exiting Patient Admission System...\n";
                break;
        }

    } while (choice != 4);

}
