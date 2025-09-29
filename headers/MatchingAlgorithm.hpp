#pragma once
#include "DataModels.hpp"
#include "ArrayContainer.hpp"
#include <vector>

class MatchingAlgorithm {
public:
    // Match a single resume against all job descriptions
    static std::vector<MatchResult> matchResume(
        const Resume& resume, 
        const ArrayContainer<JobDescription>& jobDescriptions
    );
    
    // Match all resumes against all job descriptions
    static std::vector<MatchResult> matchAll(
        const ArrayContainer<Resume>& resumes,
        const ArrayContainer<JobDescription>& jobDescriptions
    );
    
    // Find best matches for a specific job
    static std::vector<MatchResult> findBestCandidates(
        const JobDescription& job,
        const ArrayContainer<Resume>& resumes,
        size_t maxResults = 5
    );
    
private:
    // Calculate match score between resume and job description
    static double calculateMatchScore(const Resume& resume, const JobDescription& job);
    
    // Get intersection of two skill sets
    static std::vector<std::string> getMatchedSkills(
        const std::unordered_set<std::string>& resumeSkills,
        const std::unordered_set<std::string>& jobSkills
    );
    
    // Get skills that are missing from resume
    static std::vector<std::string> getMissingSkills(
        const std::unordered_set<std::string>& resumeSkills,
        const std::unordered_set<std::string>& jobSkills
    );
};