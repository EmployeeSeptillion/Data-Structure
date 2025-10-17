//To compile and run (from project root):
//g++ main.cpp sources\ArrayContainer.cpp sources\FileParser.cpp sources\MatchingAlgorithm.cpp -o jobmatch.exe -std=c++17 -lpsapi
//./jobmatch.exe

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <chrono>
#include <memory>
#include "headers/ArrayContainer.hpp"
#include "headers/DataModels.hpp"
#include "headers/FileParser.hpp"
#include "headers/MatchingAlgorithm.hpp"
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")

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

void printMatchResults(const std::vector<MatchResult>& results, int maxResults = 10) {
    if (results.empty()) {
        std::cout << "No matches found." << std::endl;
        return;
    }
    
    std::cout << std::left << std::setw(12) << "Resume ID" 
              << std::setw(10) << "Job ID"
              << std::setw(8) << "Score"
              << std::setw(25) << "Matched Skills"
              << "Missing Skills" << std::endl;
    
    printSeparator('-', 80);
    
    for (int i = 0; i < std::min(maxResults, static_cast<int>(results.size())); ++i) {
        const auto& result = results[i];
        
        std::cout << std::left << std::setw(12) << result.resumeId
                  << std::setw(10) << result.jobId
                  << std::setw(8) << std::fixed << std::setprecision(2) << result.matchScore;
        
        // Print matched skills
        std::string matchedStr;
        for (size_t j = 0; j < std::min(size_t(3), result.matchedSkills.size()); ++j) {
            if (j > 0) matchedStr += ", ";
            matchedStr += result.matchedSkills[j];
        }
        if (result.matchedSkills.size() > 3) {
            matchedStr += "...";
        }
        std::cout << std::setw(25) << matchedStr;
        
        // Print missing skills
        std::string missingStr;
        for (size_t j = 0; j < std::min(size_t(3), result.missingSkills.size()); ++j) {
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
    
    for (const auto& resumePtr : resumes.getAll()) {
        const Resume& resume = *resumePtr;
        std::cout << "ID: " << resume.id << std::endl;
        std::cout << "Skills (" << resume.skills.size() << "): ";
        
        int count = 0;
        for (const auto& skill : resume.skills) {
            if (count > 0) std::cout << ", ";
            std::cout << skill;
            if (++count >= 10) {
                std::cout << "...";
                break;
            }
        }
        std::cout << std::endl;
        std::cout << "Description: " << resume.description.substr(0, 100) << "..." << std::endl;
        printSeparator('-', 40);
    }
}

void printJobDetails(const ArrayContainer<JobDescription>& jobs) {
    printHeader("JOB DESCRIPTION DETAILS");
    
    for (const auto& jobPtr : jobs.getAll()) {
        const JobDescription& job = *jobPtr;
        std::cout << "ID: " << job.id << std::endl;
        std::cout << "Required Skills (" << job.requiredSkills.size() << "): ";
        
        int count = 0;
        for (const auto& skill : job.requiredSkills) {
            if (count > 0) std::cout << ", ";
            std::cout << skill;
            if (++count >= 10) {
                std::cout << "...";
                break;
            }
        }
        std::cout << std::endl;
        std::cout << "Description: " << job.description.substr(0, 100) << "..." << std::endl;
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

    printHeader("BEST CANDIDATES FOR EACH JOB");
    int jobLimit = 1000; // adjust to view how many jobs
    int jobCount = 0;
    for (const auto& jobPtr : jobDescriptions.getAll()) {
        if (jobCount++ >= jobLimit) break; // Stop after 1000 jobs

        const JobDescription& job = *jobPtr;
        std::cout << "\nBest candidates for " << job.id << ":" << std::endl;

        auto candidates = MatchingAlgorithm::findBestCandidates(job, resumes, 3);
        printMatchResults(candidates, 3);
    }

    // Second analysis
    printHeader("BEST MATCHES FOR EACH RESUME");
    int resumeLimit = 1000; // adjust to view how many resumes
    int resumeCount = 0;
    for (const auto& resumePtr : resumes.getAll()) {
        if (resumeCount++ >= resumeLimit) break; // Stop after 1000 resumes

        const Resume& resume = *resumePtr;
        std::cout << "\nBest matches for " << resume.id << ":" << std::endl;

        auto matches = MatchingAlgorithm::matchResume(resume, jobDescriptions);
        printMatchResults(matches, 3);
    }

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


}
