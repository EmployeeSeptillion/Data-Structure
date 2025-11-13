#ifndef HOSPITAL_EMERGENCY_HPP
#define HOSPITAL_EMERGENCY_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <limits>

class EmergencyCase {
private:
    std::string patientName;
    std::string emergencyType;
    int priorityLevel;  // 1 = Highest (Critical), 5 = Lowest (Minor)
    std::string arrivalTime;
    int caseID;

    std::string getCurrentTime() {
        std::time_t now = std::time(0);
        std::string timeStr = std::ctime(&now);
        if (!timeStr.empty() && timeStr[timeStr.length()-1] == '\n') {
            timeStr.erase(timeStr.length()-1);
        }
        return timeStr;
    }

public:
    EmergencyCase() : patientName(""), emergencyType(""), priorityLevel(5), caseID(0) {
        arrivalTime = getCurrentTime();
    }
    
    EmergencyCase(const std::string& name, const std::string& type, int priority, int id) 
        : patientName(name), emergencyType(type), priorityLevel(priority), caseID(id) {
        arrivalTime = getCurrentTime();
    }
    
    std::string getPatientName() const { return patientName; }
    std::string getEmergencyType() const { return emergencyType; }
    int getPriorityLevel() const { return priorityLevel; }
    std::string getArrivalTime() const { return arrivalTime; }
    int getCaseID() const { return caseID; }
    
    void setPatientName(const std::string& name) { patientName = name; }
    void setEmergencyType(const std::string& type) { emergencyType = type; }
    void setPriorityLevel(int priority) { priorityLevel = priority; }
    void setCaseID(int id) { caseID = id; }
    
    bool operator<(const EmergencyCase& other) const {
        return this->priorityLevel > other.priorityLevel;
    }
    
    bool operator>(const EmergencyCase& other) const {
        return this->priorityLevel < other.priorityLevel;
    }
    
    void displayCase() const {
        std::cout << "ID: " << caseID << " | Patient: " << patientName 
                  << " | Type: " << emergencyType 
                  << " | Priority: " << priorityLevel 
                  << " | Arrived: " << arrivalTime << std::endl;
    }
    
    std::string toString() const {
        return std::to_string(caseID) + "," + patientName + "," + 
               emergencyType + "," + std::to_string(priorityLevel) + "," + arrivalTime;
    }
    
    void fromString(const std::string& data) {
        size_t pos1 = data.find(',');
        size_t pos2 = data.find(',', pos1 + 1);
        size_t pos3 = data.find(',', pos2 + 1);
        size_t pos4 = data.find(',', pos3 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos && 
            pos3 != std::string::npos && pos4 != std::string::npos) {
            caseID = std::stoi(data.substr(0, pos1));
            patientName = data.substr(pos1 + 1, pos2 - pos1 - 1);
            emergencyType = data.substr(pos2 + 1, pos3 - pos2 - 1);
            priorityLevel = std::stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
            arrivalTime = data.substr(pos4 + 1);
        }
    }
};

class PriorityQueue {
private:
    EmergencyCase* heapArray;
    int capacity;
    int currentSize;
    
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heapArray[index] > heapArray[parent]) {
                swapCases(index, parent);
                index = parent;
            } else {
                break;
            }
        }
    }
    
    void heapifyDown(int index) {
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int largest = index;
            
            if (leftChild < currentSize && heapArray[leftChild] > heapArray[largest]) {
                largest = leftChild;
            }
            
            if (rightChild < currentSize && heapArray[rightChild] > heapArray[largest]) {
                largest = rightChild;
            }
            
            if (largest != index) {
                swapCases(index, largest);
                index = largest;
            } else {
                break;
            }
        }
    }
    
    void swapCases(int index1, int index2) {
        EmergencyCase temp = heapArray[index1];
        heapArray[index1] = heapArray[index2];
        heapArray[index2] = temp;
    }
    
    void resizeHeap() {
        int newCapacity = capacity * 2;
        EmergencyCase* newHeap = new EmergencyCase[newCapacity];
        
        for (int i = 0; i < currentSize; i++) {
            newHeap[i] = heapArray[i];
        }
        
        delete[] heapArray;
        heapArray = newHeap;
        capacity = newCapacity;
    }
    
public:
    PriorityQueue() : capacity(10), currentSize(0) {
        heapArray = new EmergencyCase[capacity];
    }
    
    ~PriorityQueue() {
        delete[] heapArray;
    }
    
    PriorityQueue(const PriorityQueue& other) : capacity(other.capacity), currentSize(other.currentSize) {
        heapArray = new EmergencyCase[capacity];
        for (int i = 0; i < currentSize; i++) {
            heapArray[i] = other.heapArray[i];
        }
    }
    
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            delete[] heapArray;
            capacity = other.capacity;
            currentSize = other.currentSize;
            heapArray = new EmergencyCase[capacity];
            for (int i = 0; i < currentSize; i++) {
                heapArray[i] = other.heapArray[i];
            }
        }
        return *this;
    }
    
    void enqueue(const EmergencyCase& newCase) {
        if (currentSize >= capacity) {
            resizeHeap();
        }
        
        heapArray[currentSize] = newCase;
        heapifyUp(currentSize);
        currentSize++;
    }
    
    EmergencyCase dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Cannot dequeue from empty priority queue");
        }
        
        EmergencyCase highestPriority = heapArray[0];
        heapArray[0] = heapArray[currentSize - 1];
        currentSize--;
        
        if (currentSize > 0) {
            heapifyDown(0);
        }
        
        return highestPriority;
    }
    
    EmergencyCase peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Cannot peek empty priority queue");
        }
        return heapArray[0];
    }
    
    bool isEmpty() const {
        return currentSize == 0;
    }
    
    int getSize() const {
        return currentSize;
    }
    
    void displayAll() const {
        if (isEmpty()) {
            std::cout << "No cases in the priority queue." << std::endl;
            return;
        }
        
        std::cout << "\n=== ALL CASES IN PRIORITY QUEUE ===" << std::endl;
        for (int i = 0; i < currentSize; i++) {
            std::cout << (i + 1) << ". ";
            heapArray[i].displayCase();
        }
        std::cout << "===================================" << std::endl;
    }
    
    void displayPriorityOrder() const {
        if (isEmpty()) {
            std::cout << "No pending emergency cases." << std::endl;
            return;
        }
        
        std::cout << "\n=== PENDING EMERGENCY CASES (Priority Order) ===" << std::endl;
        PriorityQueue tempQueue = *this;
        int count = 1;
        
        while (!tempQueue.isEmpty()) {
            std::cout << count << ". ";
            tempQueue.dequeue().displayCase();
            count++;
        }
        std::cout << "================================================" << std::endl;
    }
};

class EmergencyDepartment {
private:
    PriorityQueue emergencyQueue;
    int nextCaseID;
    std::string dataFileName;
    
    bool validatePriority(int priority) {
        return (priority >= 1 && priority <= 5);
    }
    
public:
    EmergencyDepartment() : nextCaseID(1), dataFileName("emergency_data.txt") {}
    
    EmergencyDepartment(const std::string& filename) : nextCaseID(1), dataFileName(filename) {}
    
    void logEmergencyCase(const std::string& patientName, const std::string& emergencyType, int priorityLevel) {
        if (patientName.empty()) {
            std::cout << "Error: Patient name cannot be empty." << std::endl;
            return;
        }
        
        if (emergencyType.empty()) {
            std::cout << "Error: Emergency type cannot be empty." << std::endl;
            return;
        }
        
        if (!validatePriority(priorityLevel)) {
            std::cout << "Error: Priority level must be between 1 (Critical) and 5 (Minor)." << std::endl;
            return;
        }
        
        EmergencyCase newCase(patientName, emergencyType, priorityLevel, nextCaseID);
        emergencyQueue.enqueue(newCase);
        
        std::cout << "\n=== EMERGENCY CASE LOGGED SUCCESSFULLY ===" << std::endl;
        newCase.displayCase();
        std::cout << "==========================================" << std::endl;
        
        nextCaseID++;
    }
    
    void processMostCriticalCase() {
        if (emergencyQueue.isEmpty()) {
            std::cout << "No emergency cases to process. Queue is empty." << std::endl;
            return;
        }
        
        EmergencyCase criticalCase = emergencyQueue.dequeue();
        
        std::cout << "\n=== PROCESSING MOST CRITICAL CASE ===" << std::endl;
        criticalCase.displayCase();
        std::cout << "This case is now being attended by emergency staff." << std::endl;
        std::cout << "Remaining cases in queue: " << emergencyQueue.getSize() << std::endl;
        std::cout << "=====================================" << std::endl;
    }
    
    void viewPendingEmergencyCases() {
        if (emergencyQueue.isEmpty()) {
            std::cout << "No pending emergency cases." << std::endl;
            return;
        }
        
        emergencyQueue.displayPriorityOrder();
    }
    
    bool hasPendingCases() const {
        return !emergencyQueue.isEmpty();
    }
    
    int getPendingCasesCount() const {
        return emergencyQueue.getSize();
    }
    
    void displayDepartmentStatus() const {
        std::cout << "\n=== EMERGENCY DEPARTMENT STATUS ===" << std::endl;
        std::cout << "Pending Cases: " << getPendingCasesCount() << std::endl;
        std::cout << "Next Case ID: " << nextCaseID << std::endl;
        std::cout << "Status: " << (hasPendingCases() ? "BUSY - Cases waiting" : "READY - No pending cases") << std::endl;
        
        if (hasPendingCases()) {
            std::cout << "\nNext case to process:" << std::endl;
            emergencyQueue.peek().displayCase();
        }
        std::cout << "==================================" << std::endl;
    }
    
    void saveToFile() {
        std::ofstream outFile(dataFileName);
        if (!outFile) {
            std::cout << "Error: Cannot open file for writing: " << dataFileName << std::endl;
            return;
        }
        
        outFile << nextCaseID << std::endl;
        
        PriorityQueue tempQueue = emergencyQueue;
        while (!tempQueue.isEmpty()) {
            EmergencyCase caseToSave = tempQueue.dequeue();
            outFile << caseToSave.toString() << std::endl;
        }
        
        outFile.close();
        std::cout << "Emergency department data saved to: " << dataFileName << std::endl;
        std::cout << "Next Case ID for future cases: " << nextCaseID << std::endl;
    }
    
    void loadFromFile() {
        std::ifstream inFile(dataFileName);
        if (!inFile) {
            std::cout << "No previous data found. Starting with new emergency department." << std::endl;
            return;
        }

        while (!emergencyQueue.isEmpty()) {
            emergencyQueue.dequeue();
        }

        std::string line;
        
        if (std::getline(inFile, line)) {
            try {
                nextCaseID = std::stoi(line);
            } catch (const std::exception& e) {
                std::cout << "Error reading next Case ID, resetting to 1" << std::endl;
                nextCaseID = 1;
            }
        }

        int loadedCases = 0;
        int maxID = 0;
        while (std::getline(inFile, line)) {
            if (!line.empty()) {
                EmergencyCase newCase;
                newCase.fromString(line);
                emergencyQueue.enqueue(newCase);
                loadedCases++;
                
                if (newCase.getCaseID() >= maxID) {
                    maxID = newCase.getCaseID();
                }
            }
        }

        // Ensure nextCaseID is higher than any loaded case
        if (maxID >= nextCaseID) {
            nextCaseID = maxID + 1;
        }

        inFile.close();
        std::cout << "Loaded " << loadedCases << " emergency cases. Next Case ID: " << nextCaseID << std::endl;
    }
};

#endif