//To compile and run (from project root):
//g++ -o jobmatch.exe main.cpp sources/FileParser.cpp sources/MatchingAlgorithm.cpp sources/ArrayContainer.cpp -std=c++17 -lpsapi
//./jobmatch.exe

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include "headers/ArrayContainer.hpp"
#include "headers/DataModels.hpp"
#include "headers/FileParser.hpp"
#include "headers/MatchingAlgorithm.hpp"
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")

// Helper struct to replace std::pair for statistics gathering
struct SkillCountPair {
    std::string skill;
    int count;

    // Comparator for sorting (descending order by count)
    bool operator>(const SkillCountPair& other) const {
        return count > other.count;
    }
};

// Helper function for Quicksort
template<typename T>
size_t partition(DynamicArray<T>& arr, size_t low, size_t high) {
    // We use the last element as the pivot
    T pivot = arr[high]; 
    size_t i = low; 

    // Re-arrange the array so elements > pivot are on the left
    for (size_t j = low; j < high; j++) {
        // Use operator> for descending sort (arr[j] > pivot)
        // Note: arr[j] and arr[i] elements must be swappable.
        if (arr[j] > pivot) { 
            // Swap arr[i] and arr[j]
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
        }
    }
    // Swap the pivot (arr[high]) with the element at the partition index (arr[i])
    T temp = arr[i];
    arr[i] = arr[high];
    arr[high] = temp;
    return i;
}

// Recursive Quicksort function
template<typename T>
void quickSort(DynamicArray<T>& arr, size_t low, size_t high) {
    if (low < high) {
        size_t pi = partition(arr, low, high);

        // Recursively sort elements before and after partition
        if (pi > 0) {
            quickSort(arr, low, pi - 1);
        }
        quickSort(arr, pi + 1, high);
    }
}

// Wrapper to simplify the function call
template<typename T>
void quickSortWrapper(DynamicArray<T>& arr) {
    if (arr.size() > 1) {
        quickSort(arr, 0, arr.size() - 1);
    }
}

// Function to get current memory usage in bytes
size_t getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
                             (PROCESS_MEMORY_COUNTERS*)&pmc,
                             sizeof(pmc))) {
        return static_cast<size_t>(pmc.WorkingSetSize);
    }
    return 0;
}

void printSeparator(char c = '=', int length = 80) {
    std::cout << std::string(length, c) << std::endl;
}

void printHeader(const std::string& title) {
    printSeparator();
    std::cout << std::setw(40 + title.length()/2) << title << std::endl;
    printSeparator();
}

void printMatchResults(const DynamicArray<MatchResult>& results, int maxResults = 10) {
    if (results.empty()) {
        std::cout << "No matches found." << std::endl;
        return;
    }
    
    // Removed the redundant block from the end of the original file
    
    std::cout << std::left << std::setw(12) << "Resume ID" 
              << std::setw(10) << "Job ID"
              << std::setw(8) << "Score"
              << std::setw(25) << "Matched Skills"
              << "Missing Skills" << std::endl;
    
    printSeparator('-', 80);
    
    // Using simple check instead of std::min
    int limit = (maxResults < (int)results.size()) ? maxResults : (int)results.size();
    for (int i = 0; i < limit; ++i) {
        const auto& result = results[i];
        
        std::cout << std::left << std::setw(12) << result.resumeId
                  << std::setw(10) << result.jobId
                  << std::setw(8) << std::fixed << std::setprecision(2) << result.matchScore;
        
        // Print matched skills
        std::string matchedStr;
        size_t matchedLimit = (result.matchedSkills.size() < 3) ? result.matchedSkills.size() : 3;
        for (size_t j = 0; j < matchedLimit; ++j) {
            if (j > 0) matchedStr += ", ";
            matchedStr += result.matchedSkills[j];
        }
        if (result.matchedSkills.size() > 3) {
            matchedStr += "...";
        }
        std::cout << std::setw(25) << matchedStr;
        
        // Print missing skills
        std::string missingStr;
        size_t missingLimit = (result.missingSkills.size() < 3) ? result.missingSkills.size() : 3;
        for (size_t j = 0; j < missingLimit; ++j) {
            if (j > 0) missingStr += ", ";
            missingStr += result.missingSkills[j];
        }
        if (result.missingSkills.size() > 3) {
            missingStr += "...";
        }
        std::cout << missingStr << std::endl;
    }
}

void printResumeDetails(const ArrayContainer<Resume>& resumes) {
    printHeader("RESUME DETAILS");
    
    // Using ArrayContainer's custom iterator
    for (auto it = resumes.begin(); it != resumes.end(); ++it) {
        const Resume& resume = *it;
        std::cout << "ID: " << resume.id << std::endl;
        std::cout << "Skills (" << resume.skills.size() << "): ";
        
        // Note: StringSet doesn't have an iterator, must use toArray() or direct access
        DynamicArray<std::string> skillArray = resume.skills.toArray();
        
        int count = 0;
        for (size_t i = 0; i < skillArray.size(); ++i) {
            if (count > 0) std::cout << ", ";
            std::cout << skillArray[i];
            if (++count >= 10) {
                std::cout << "...";
                break;
            }
        }
        std::cout << std::endl;
        // Check length before substr to avoid exceptions
        size_t descLength = resume.description.length();
        std::cout << "Description: " << resume.description.substr(0, (descLength > 100 ? 100 : descLength)) 
                  << (descLength > 100 ? "..." : "") << std::endl;
        printSeparator('-', 40);
    }
}

void printJobDetails(const ArrayContainer<JobDescription>& jobs) {
    printHeader("JOB DESCRIPTION DETAILS");
    
    // Using ArrayContainer's custom iterator
    for (auto it = jobs.begin(); it != jobs.end(); ++it) {
        const JobDescription& job = *it;
        std::cout << "ID: " << job.id << std::endl;
        std::cout << "Required Skills (" << job.requiredSkills.size() << "): ";
        
        // Note: StringSet doesn't have an iterator, must use toArray() or direct access
        DynamicArray<std::string> skillArray = job.requiredSkills.toArray();
        
        int count = 0;
        for (size_t i = 0; i < skillArray.size(); ++i) {
            if (count > 0) std::cout << ", ";
            std::cout << skillArray[i];
            if (++count >= 10) {
                std::cout << "...";
                break;
            }
        }
        std::cout << std::endl;
        size_t descLength = job.description.length();
        std::cout << "Description: " << job.description.substr(0, (descLength > 100 ? 100 : descLength)) 
                  << (descLength > 100 ? "..." : "") << std::endl;
        printSeparator('-', 40);
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    size_t memBefore = getMemoryUsage();

    // Initialize containers
    ArrayContainer<Resume> resumes;
    ArrayContainer<JobDescription> jobDescriptions;
    
    printHeader("RESUME AND JOB MATCHING SYSTEM");
    
    // Parse CSV files
    std::cout << "Loading resume data..." << std::endl;
    if (!FileParser::parseResumeCSV("data/resume.csv", resumes)) {
        std::cerr << "Failed to parse resume.csv" << std::endl;
        return 1;
    }
    std::cout << "Loaded " << resumes.size() << " resumes." << std::endl;
    
    std::cout << "Loading job description data..." << std::endl;
    if (!FileParser::parseJobDescriptionCSV("data/job_description.csv", jobDescriptions)) {
        std::cerr << "Failed to parse job_description.csv" << std::endl;
        return 1;
    }
    std::cout << "Loaded " << jobDescriptions.size() << " job descriptions." << std::endl;

    // ====================================================================
    // 1. BEST CANDIDATES FOR EACH JOB (FAST - Streams Output)
    // ====================================================================
    printHeader("BEST CANDIDATES FOR EACH JOB");
    int jobLimit = 1000; // Limit to 1000 jobs
    int jobCount = 0;
    
    std::cout << "Calculating matches for " << jobLimit << " jobs... Please wait." << std::endl;

    for (auto it = jobDescriptions.begin(); it != jobDescriptions.end(); ++it) {
        if (jobCount++ >= jobLimit) break;

        const JobDescription& job = *it;
        
        // PROGRESS INDICATOR
        if (jobCount % 100 == 0) {
            std::cout << "\rProcessing Jobs... " << jobCount << "/" << jobLimit << std::flush;
        }

        // Output the results for the current job immediately
        std::cout << "\nBest candidates for " << job.id << ":" << std::endl;
        auto candidates = MatchingAlgorithm::findBestCandidates(job, resumes, 3);
        printMatchResults(candidates, 3);
    }
    std::cout << "\rProcessing Jobs... " << jobLimit << "/" << jobLimit << " Done!" << std::endl;
    printSeparator();

    // ====================================================================
    // 2. BEST MATCHES FOR EACH RESUME (FAST - Streams Output)
    // ====================================================================
    printHeader("BEST MATCHES FOR EACH RESUME");
    int resumeLimit = 1000; // Limit to 1000 resumes
    int resumeCount = 0;
    
    std::cout << "Calculating matches for " << resumeLimit << " resumes... Please wait." << std::endl;

    for (auto it = resumes.begin(); it != resumes.end(); ++it) {
        if (resumeCount++ >= resumeLimit) break;

        const Resume& resume = *it;
        
        // PROGRESS INDICATOR
        if (resumeCount % 100 == 0) {
            std::cout << "\rProcessing Resumes... " << resumeCount << "/" << resumeLimit << std::flush;
        }

        // Output the results for the current resume immediately
        std::cout << "\nBest matches for " << resume.id << ":" << std::endl;
        auto matches = MatchingAlgorithm::matchResume(resume, jobDescriptions);
        printMatchResults(matches, 3);
    }
    std::cout << "\rProcessing Resumes... " << resumeLimit << "/" << resumeLimit << " Done!" << std::endl;
    printSeparator();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = end - start;

    size_t memAfter = getMemoryUsage();

    std::cout << "\n============================================\n";
    std::cout << "Performance Summary\n";
    std::cout << "============================================\n";
    std::cout << "Total Execution Time: " << total.count() << " ms" << std::endl;
    std::cout << "Memory Used: "
          << (memAfter - memBefore) / (1024.0 * 1024.0)
          << " MB" << std::endl;
    std::cout << "============================================\n";
    
    return 0;
}