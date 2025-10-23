#include <iostream>
#include <iomanip>
#include "JobMatchingArray.h"

void displayMenu() {
    std::cout << "\n=== Job Matching System (Array Implementation) ===" << std::endl;
    std::cout << "1. Load Job Descriptions" << std::endl;
    std::cout << "2. Load Resume Descriptions" << std::endl;
    std::cout << "3. Find Best Matches for a Job" << std::endl;
    std::cout << "4. Run Performance Test" << std::endl;
    std::cout << "5. Display Statistics" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

int main() {
    JobMatchingSystem system;
    int choice;
    bool jobsLoaded = false;
    bool resumesLoaded = false;

    std::cout << "Job Matching System using Array Data Structure" << std::endl;
    std::cout << "Implementation: Weighted Scoring Algorithm" << std::endl;

    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                if (system.loadJobsFromCSV("data/job_description.csv")) {
                    jobsLoaded = true;
                }
                break;
                
            case 2:
                if (system.loadResumesFromCSV("data/resume.csv")) {
                    resumesLoaded = true;
                }
                break;
                
            case 3:
                if (!jobsLoaded || !resumesLoaded) {
                    std::cout << "Error: Please load both jobs and resumes first" << std::endl;
                    break;
                }
                int jobIndex;
                std::cout << "Enter job index (1 to " << system.getJobCount() << "): ";
                std::cin >> jobIndex;
                system.findBestMatches(jobIndex - 1);
                break;
                
            case 4:
                if (!jobsLoaded || !resumesLoaded) {
                    std::cout << "Error: Please load both jobs and resumes first" << std::endl;
                    break;
                }
                system.performanceTest();
                break;
                
            case 5:
                std::cout << "\n=== System Statistics ===" << std::endl;
                std::cout << "Jobs loaded: " << system.getJobCount() << std::endl;
                std::cout << "Resumes loaded: " << system.getResumeCount() << std::endl;
                break;
                
            case 6:
                std::cout << "Exiting system. Goodbye!" << std::endl;
                break;
                
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 6);

    return 0;
}