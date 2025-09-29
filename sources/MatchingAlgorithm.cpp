#include "../headers/MatchingAlgorithm.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

std::vector<MatchResult> MatchingAlgorithm::matchResume(
    const Resume& resume, 
    const ArrayContainer<JobDescription>& jobDescriptions) {
    
    std::vector<MatchResult> results;
    
    for (const auto& jobPtr : jobDescriptions.getAll()) {
        const JobDescription& job = *jobPtr;
        double score = calculateMatchScore(resume, job);
        
        auto matchedSkills = getMatchedSkills(resume.skills, job.requiredSkills);
        auto missingSkills = getMissingSkills(resume.skills, job.requiredSkills);
        
        results.emplace_back(resume.id, job.id, score, matchedSkills, missingSkills);
    }
    
    // Sort by match score (descending)
    std::sort(results.begin(), results.end(), std::greater<MatchResult>());
    
    return results;
}

std::vector<MatchResult> MatchingAlgorithm::matchAll(
    const ArrayContainer<Resume>& resumes,
    const ArrayContainer<JobDescription>& jobDescriptions) {
    
    std::vector<MatchResult> allResults;
    
    for (const auto& resumePtr : resumes.getAll()) {
        const Resume& resume = *resumePtr;
        auto resumeMatches = matchResume(resume, jobDescriptions);
        
        allResults.insert(allResults.end(), resumeMatches.begin(), resumeMatches.end());
    }
    
    // Sort all results by match score
    std::sort(allResults.begin(), allResults.end(), std::greater<MatchResult>());
    
    return allResults;
}

std::vector<MatchResult> MatchingAlgorithm::findBestCandidates(
    const JobDescription& job,
    const ArrayContainer<Resume>& resumes,
    size_t maxResults) {
    
    std::vector<MatchResult> candidates;
    
    for (const auto& resumePtr : resumes.getAll()) {
        const Resume& resume = *resumePtr;
        double score = calculateMatchScore(resume, job);
        
        auto matchedSkills = getMatchedSkills(resume.skills, job.requiredSkills);
        auto missingSkills = getMissingSkills(resume.skills, job.requiredSkills);
        
        candidates.emplace_back(resume.id, job.id, score, matchedSkills, missingSkills);
    }
    
    // Sort by match score (descending)
    std::sort(candidates.begin(), candidates.end(), std::greater<MatchResult>());
    
    // Return only top results
    if (candidates.size() > maxResults) {
        candidates.resize(maxResults);
    }
    
    return candidates;
}

double MatchingAlgorithm::calculateMatchScore(const Resume& resume, const JobDescription& job) {
    if (job.requiredSkills.empty()) {
        return 0.0;
    }
    
    // Count matched skills
    size_t matchedCount = 0;
    for (const auto& skill : job.requiredSkills) {
        if (resume.skills.count(skill) > 0) {
            matchedCount++;
        }
    }
    
    // Calculate basic match percentage
    double basicScore = static_cast<double>(matchedCount) / job.requiredSkills.size();
    
    // Bonus for having more skills than required
    double skillBonus = 0.0;
    if (resume.skills.size() > job.requiredSkills.size()) {
        skillBonus = std::min(0.2, (resume.skills.size() - job.requiredSkills.size()) * 0.02);
    }
    
    // Penalty for having too few total skills
    double skillPenalty = 0.0;
    if (resume.skills.size() < job.requiredSkills.size() * 0.5) {
        skillPenalty = 0.1;
    }
    
    double finalScore = basicScore + skillBonus - skillPenalty;
    
    // Ensure score is between 0 and 1
    return std::max(0.0, std::min(1.0, finalScore));
}

std::vector<std::string> MatchingAlgorithm::getMatchedSkills(
    const std::unordered_set<std::string>& resumeSkills,
    const std::unordered_set<std::string>& jobSkills) {
    
    std::vector<std::string> matched;
    
    for (const auto& skill : jobSkills) {
        if (resumeSkills.count(skill) > 0) {
            matched.push_back(skill);
        }
    }
    
    std::sort(matched.begin(), matched.end());
    return matched;
}

std::vector<std::string> MatchingAlgorithm::getMissingSkills(
    const std::unordered_set<std::string>& resumeSkills,
    const std::unordered_set<std::string>& jobSkills) {
    
    std::vector<std::string> missing;
    
    for (const auto& skill : jobSkills) {
        if (resumeSkills.count(skill) == 0) {
            missing.push_back(skill);
        }
    }
    
    std::sort(missing.begin(), missing.end());
    return missing;
}