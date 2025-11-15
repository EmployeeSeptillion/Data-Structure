#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define SIZE 100

// Define a struct to represent each supply item
struct Supply {
    string type;
    int quantity;
    string batch;
};

// Stack class for medical supplies
class SupplyStack {
private:
    Supply stack[SIZE];
    int top = -1;
    int n = SIZE;
    string filename = "supplies.csv";

public:
    bool isEmpty() { return (top <= -1); }
    bool isFull() { return (top >= n - 1); }

    // Add new supply to stack (push)
    void push(Supply s) {
        // Check for duplicate (same type + batch)
        for (int i = 0; i <= top; i++) {
            if (stack[i].type == s.type && stack[i].batch == s.batch) {
                cout << "ERROR: Duplicate entry detected!\n";
                cout << "Type: " << s.type << " | Batch: " << s.batch
                     << " already exists. Entry not added.\n";
                return;
            }
        }

        if (isFull()) {
            cout << "Stack memory is full!\n";
            return;
        }

        // Add new supply to stack
        top++;
        stack[top] = s;

        // Save to file only when something is added
        saveToFile();

        cout << "Supply added: " << s.type
            << " | Quantity: " << s.quantity
            << " | Batch: " << s.batch << endl;
    }



    // Use last added supply (pop)
    Supply pop() {
        Supply s;
        if (isEmpty())
            cout << "No supplies available to use!\n";
        else {
            s = stack[top];
            cout << "Using last added supply: ";
            cout << "Type:" << s.type << " (Quantity: " << s.quantity << "| Batch:" << s.batch << ")\n" << endl;
            top--;
            saveToFile();
        }
        return s;
    }

    // Show all supplies
    void show() {
        if (isEmpty())
            cout << "No supplies available.\n";
        else {
            cout << "\nCurrent Supplies [Top to Bottom]\n";
            cout << "---------------------------------\n";
            for (int i = top; i >= 0; i--) {
                cout << "Type: " << stack[i].type
                     << " | Quantity: " << stack[i].quantity
                     << " | Batch: " << stack[i].batch << endl;
            }
        }
    }

    // Save to CSV file
    void saveToFile() {
        ofstream file(filename);
        if (!file) {
            cout << "Error saving file!\n";
            return;
        }
        file << "Type,Quantity,Batch\n";
        for (int i = 0; i <= top; i++) {
            file << stack[i].type << ","
                 << stack[i].quantity << ","
                 << stack[i].batch << "\n";
        }
        file.close();
    }

    // Load from CSV file
    void loadFromFile() {
        ifstream file(filename);
        if (!file) {
            cout << "(No existing supplies file found — starting fresh.)\n";
            return;
        }
        string line;
        getline(file, line); // Skip header

        while (getline(file, line)) {
            size_t pos1 = line.find(",");
            size_t pos2 = line.rfind(",");
            if (pos1 == string::npos || pos2 == string::npos)
                continue;
            Supply s;
            s.type = line.substr(0, pos1);
            s.quantity = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            s.batch = line.substr(pos2 + 1);
            if (!isFull()) {
                top++;
                stack[top] = s;
            }
        }
        file.close();
    }
};
