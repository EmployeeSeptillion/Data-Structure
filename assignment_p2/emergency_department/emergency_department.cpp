#include <iostream>
#include <limits>
#include "HospitalEmergency.hpp"
#include "emergency_department.hpp"

void displayMenu() {
    std::cout << "\n=== EMERGENCY DEPARTMENT MANAGEMENT SYSTEM ===" << std::endl;
    std::cout << "1. Log Emergency Case" << std::endl;
    std::cout << "2. Process Most Critical Case" << std::endl;
    std::cout << "3. View Pending Emergency Cases" << std::endl;
    std::cout << "4. Department Status" << std::endl;
    std::cout << "5. Save Data" << std::endl;
    std::cout << "6. Load Data" << std::endl;
    std::cout << "7. Add Sample Data (10 Cases)" << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Enter your choice (1-8): ";
}

void addSampleData(EmergencyDepartment& dept) {
    std::cout << "\n=== ADDING 10 SAMPLE EMERGENCY CASES ===" << std::endl;

    dept.logEmergencyCase("John Smith", "Heart Attack", 1);
    dept.logEmergencyCase("Maria Garcia", "Stroke", 1);
    dept.logEmergencyCase("Robert Johnson", "Severe Bleeding", 2);
    dept.logEmergencyCase("Sarah Wilson", "Difficulty Breathing", 2);
    dept.logEmergencyCase("Michael Brown", "Broken Leg", 3);
    dept.logEmergencyCase("Emily Davis", "High Fever", 3);
    dept.logEmergencyCase("David Miller", "Deep Cut", 3);
    dept.logEmergencyCase("Lisa Anderson", "Sprained Ankle", 4);
    dept.logEmergencyCase("James Taylor", "Minor Burn", 4);
    dept.logEmergencyCase("Jennifer Lee", "Rash", 5);

    std::cout << "10 sample cases added successfully!" << std::endl;
}

void emergency_department() {

    EmergencyDepartment dept("emergency_department/emergency_data.txt");

    std::cout << "=== WELCOME TO HOSPITAL EMERGENCY DEPARTMENT ===" << std::endl;
    std::cout << "Loading emergency department data..." << std::endl;
    dept.loadFromFile();

    int choice;
    std::string name, type;
    int priority;

    do {
        displayMenu();
        std::cin >> choice;

        // Validate menu input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number between 1-8." << std::endl;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {

            case 1:  // Log case
                std::cout << "Enter patient name: ";
                std::getline(std::cin, name);
                std::cout << "Enter emergency type: ";
                std::getline(std::cin, type);
                std::cout << "Enter priority level (1-Critical to 5-Minor): ";
                std::cin >> priority;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cout << "Invalid priority level!" << std::endl;
                } else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    dept.logEmergencyCase(name, type, priority);
                }
                break;

            case 2:
                dept.processMostCriticalCase();
                break;

            case 3:
                dept.viewPendingEmergencyCases();
                break;

            case 4:
                dept.displayDepartmentStatus();
                break;

            case 5:
                dept.saveToFile();
                break;

            case 6:
                dept.loadFromFile();
                break;

            case 7:
                addSampleData(dept);
                break;

            case 8:
                dept.saveToFile();
                std::cout << "Thank you for using Emergency Department System!" << std::endl;
                std::cout << "Data saved successfully. Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice! Please enter a number between 1-8." << std::endl;
        }

    } while (choice != 8);
}
