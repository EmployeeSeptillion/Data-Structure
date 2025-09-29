#pragma once
#include <string>
#include <vector>
#include <unordered_set>

struct Resume {
    std::string id;
    std::string description;
    std::unordered_set<std::string> skills;
    
    Resume() = default;
    Resume(const std::string& id, const std::string& desc);
};

struct JobDescription {
    std::string id;
    std::string description;
    std::unordered_set<std::string> requiredSkills;
    
    JobDescription() = default;
    JobDescription(const std::string& id, const std::string& desc);
};

struct MatchResult {
    std::string resumeId;
    std::string jobId;
    double matchScore;
    std::vector<std::string> matchedSkills;
    std::vector<std::string> missingSkills;
    
    // Default constructor
    MatchResult() : matchScore(0.0) {}
    
    MatchResult(const std::string& resId, const std::string& jId, 
                double score, const std::vector<std::string>& matched,
                const std::vector<std::string>& missing);
    
    bool operator>(const MatchResult& other) const {
        return matchScore > other.matchScore;
    }
};