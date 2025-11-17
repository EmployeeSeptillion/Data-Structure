// include library files
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "ambulance_dispatcher.hpp"

using namespace std;

// constant values and file name for ambulance queue
const int MAX_AMBULANCES = 20;
const char* AMBULANCE_DATA_FILE = "ambulance_dispatcher/ambulance_data.txt";

// data for one ambulance
struct Ambulance {
    int    id;
    string plate;
    string driverName;
};

// queue class for ambulance rotation
class AmbulanceQueue {
private:
    // storage and control values for circular queue
    Ambulance data[MAX_AMBULANCES];
    int front;
    int rear;
    int count;
    int nextId;   // next auto generate id

    // helper to make plate text from id
    static string generatePlate(int id) {
        ostringstream oss;
        oss << "AMB-" << setw(3) << setfill('0') << id;
        return oss.str();
    }

public:
    // set default values for queue
    AmbulanceQueue() {
        front = 0;
        rear  = -1;
        count = 0;
        nextId = 1;
    }

    // check if queue has no ambulance
    bool isEmpty() const {
        return count == 0;
    }

    // check if queue is full
    bool isFull() const {
        return count == MAX_AMBULANCES;
    }

    // add new ambulance with auto id and plate
    bool registerAmbulance(const string& driverName, Ambulance& created) {
        if (isFull()) {
            return false;
        }

        Ambulance a;
        a.id = nextId++;
        a.plate = generatePlate(a.id);
        a.driverName = driverName;

        rear = (rear + 1) % MAX_AMBULANCES;
        data[rear] = a;
        count++;

        created = a;
        return true;
    }

    // move first ambulance to back
    bool rotateShift() {
        if (count <= 1) {
            return false;
        }

        Ambulance first = data[front];
        front = (front + 1) % MAX_AMBULANCES;
        rear  = (rear  + 1) % MAX_AMBULANCES;
        data[rear] = first;

        return true;
    }

    // show all ambulances in order
    void displaySchedule() const {
        if (isEmpty()) {
            cout << "\nNo ambulances registered.\n";
            return;
        }

        cout << "\nCurrent Ambulance Rotation (next on duty is first):\n";
        int index = front;
        for (int i = 0; i < count; ++i) {
            cout << (i + 1) << ". ID: " << data[index].id
                 << " | Plate: " << data[index].plate
                 << " | Driver: " << data[index].driverName << '\n';
            index = (index + 1) % MAX_AMBULANCES;
        }
        cout << endl;
    }

    // save ambulance data into file
    void saveToFile(const string& filename) const {
        ofstream out(filename.c_str());
        if (!out) {
            return;
        }

        out << count << '\n';
        int index = front;
        for (int i = 0; i < count; ++i) {
            out << data[index].id << ','
                << data[index].plate << ','
                << data[index].driverName << '\n';
            index = (index + 1) % MAX_AMBULANCES;
        }
    }

    // load ambulance data from file
    void loadFromFile(const string& filename) {
        ifstream in(filename.c_str());
        if (!in) {
            return;
        }

        front = 0;
        rear  = -1;
        count = 0;
        nextId = 1;

        int n;
        if (!(in >> n)) {
            return;
        }
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        int maxId = 0;

        for (int i = 0; i < n && i < MAX_AMBULANCES; ++i) {
            string line;
            if (!getline(in, line)) break;
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, plateStr, nameStr;

            if (!getline(ss, idStr, ',')) continue;
            if (!getline(ss, plateStr, ',')) continue;
            if (!getline(ss, nameStr)) continue;

            Ambulance a;
            a.id = stoi(idStr);
            a.plate = plateStr;
            a.driverName = nameStr;

            if (a.id > maxId) {
                maxId = a.id;
            }

            rear = (rear + 1) % MAX_AMBULANCES;
            data[rear] = a;
            ++count;
        }

        if (maxId >= 1) {
            nextId = maxId + 1;
        } else {
            nextId = 1;
        }
    }

    // remove ambulance by id
    bool removeById(int id) {
        if (isEmpty()) {
            return false;
        }

        Ambulance temp[MAX_AMBULANCES];
        int newCount = 0;
        bool removed = false;

        int idx = front;
        for (int i = 0; i < count; ++i) {
            if (!removed && data[idx].id == id) {
                removed = true;
            } else {
                temp[newCount++] = data[idx];
            }
            idx = (idx + 1) % MAX_AMBULANCES;
        }

        if (!removed) {
            return false;
        }

        front = 0;
        count = newCount;

        if (newCount == 0) {
            rear = -1;
        } else {
            for (int i = 0; i < newCount; ++i) {
                data[i] = temp[i];
            }
            rear = newCount - 1;
        }

        return true;
    }
};

// shared queue object for this file
static AmbulanceQueue ambulanceQueue;
static bool ambulanceDataLoaded = false;

// helper to clear user input
static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// menu for ambulance dispatcher role
void ambulance_dispatcher() {
    if (!ambulanceDataLoaded) {
        ambulanceQueue.loadFromFile(AMBULANCE_DATA_FILE);
        ambulanceDataLoaded = true;
    }

    int choice;

    do {
        cout << "\n===== Ambulance Dispatcher Menu =====\n";
        cout << "1. Register Ambulance\n";
        cout << "2. Rotate Ambulance Shift\n";
        cout << "3. Display Ambulance Schedule\n";
        cout << "4. Delete Ambulance by ID\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            clearInput();
            continue;
        }
        clearInput();

        // handle user choice to register ambulance
        if (choice == 1) {
            if (ambulanceQueue.isFull()) {
                cout << "Queue is full. Cannot register more ambulances.\n";
                continue;
            }

            string driverName;
            cout << "Enter Driver Name: ";
            getline(cin, driverName);
            if (driverName.empty()) {
                cout << "Driver name cannot be empty.\n";
                continue;
            }

            Ambulance created;
            if (ambulanceQueue.registerAmbulance(driverName, created)) {
                cout << "Ambulance registered successfully.\n";
                cout << "Assigned ID: " << created.id << '\n';
                cout << "Assigned Plate: " << created.plate << '\n';
            } else {
                cout << "Queue is full. Cannot register more ambulances.\n";
            }

        // handle user choice to rotate shift
        } else if (choice == 2) {
            if (ambulanceQueue.rotateShift()) {
                cout << "Ambulance shift rotated.\n";
            } else {
                cout << "Not enough ambulances to rotate.\n";
            }

        // handle user choice to see schedule
        } else if (choice == 3) {
            ambulanceQueue.displaySchedule();

        // handle user choice to delete ambulance
        } else if (choice == 4) {
            if (ambulanceQueue.isEmpty()) {
                cout << "No ambulances to delete.\n";
                continue;
            }

            int deleteId;
            cout << "Enter Ambulance ID to delete: ";
            if (!(cin >> deleteId)) {
                cout << "Invalid ID.\n";
                clearInput();
                continue;
            }
            clearInput();

            if (ambulanceQueue.removeById(deleteId)) {
                cout << "Ambulance with ID " << deleteId << " deleted.\n";
            } else {
                cout << "No ambulance found with that ID.\n";
            }

        // handle user choice to leave menu
        } else if (choice == 0) {
            ambulanceQueue.saveToFile(AMBULANCE_DATA_FILE);
            cout << "Returning to main menu...\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);
}
