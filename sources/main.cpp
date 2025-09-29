#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "../headers/DataModels.hpp"
#include "../headers/ArrayContainer.hpp"
#include "../headers/FileParser.hpp"
#include "../headers/MatchingAlgorithm.hpp"

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
    // Initialize containers
    ArrayContainer<Resume> resumes;
    ArrayContainer<JobDescription> jobDescriptions;
    
    printHeader("RESUME AND JOB MATCHING SYSTEM");
    
    // Parse CSV files
    std::cout << "Loading resume data..." << std::endl;
    if (!FileParser::parseResumeCSV("C:\\Users\\leeli\\OneDrive\\Documents\\dstr_assignment\\data\\resume.csv", resumes)) {
        std::cerr << "Failed to parse resume.csv" << std::endl;
        return 1;
    }
    std::cout << "Loaded " << resumes.size() << " resumes." << std::endl;
    
    std::cout << "Loading job description data..." << std::endl;
    if (!FileParser::parseJobDescriptionCSV("C:\\Users\\leeli\\OneDrive\\Documents\\dstr_assignment\\data\\job_description.csv", jobDescriptions)) {
        std::cerr << "Failed to parse job_description.csv" << std::endl;
        return 1;
    }
    std::cout << "Loaded " << jobDescriptions.size() << " job descriptions." << std::endl;
    
    // Display loaded data
    printResumeDetails(resumes);
    printJobDetails(jobDescriptions);
    
    // Perform matching analysis
    printHeader("BEST CANDIDATES FOR EACH JOB");
    for (const auto& jobPtr : jobDescriptions.getAll()) {
        const JobDescription& job = *jobPtr;
        std::cout << "\nBest candidates for " << job.id << ":" << std::endl;
        
        // This is the memory-safe way to find matches
        auto candidates = MatchingAlgorithm::findBestCandidates(job, resumes, 3);
        printMatchResults(candidates, 3);
    }
    
    // This section of code is memory-safe
    printHeader("BEST MATCHES FOR EACH RESUME");
    for (const auto& resumePtr : resumes.getAll()) {
        const Resume& resume = *resumePtr;
        std::cout << "\nBest matches for " << resume.id << ":" << std::endl;
        
        auto matches = MatchingAlgorithm::matchResume(resume, jobDescriptions);
        printMatchResults(matches, 3);
    }

    std::cout << "\nAnalysis Complete. Program will now exit." << std::endl;
    return 0;
}