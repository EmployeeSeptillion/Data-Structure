#include "../headers/FileParser.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <iostream>

bool FileParser::parseResumeCSV(const std::string& filename, ArrayContainer<Resume>& resumes) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int resumeId = 1;
    std::getline(file, line); // Skip the header line
    
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;
        
        std::string id = "resume_" + std::to_string(resumeId++);
        resumes.add(std::make_shared<Resume>(id, line));
    }
    
    file.close();
    return true;
}

bool FileParser::parseJobDescriptionCSV(const std::string& filename, ArrayContainer<JobDescription>& jobDescriptions) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int jobId = 1;
    std::getline(file, line); // Skip the header line
    
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;
        
        std::string id = "job_" + std::to_string(jobId++);
        jobDescriptions.add(std::make_shared<JobDescription>(id, line));
    }
    
    file.close();
    return true;
}

std::vector<std::string> FileParser::splitCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    
    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    
    fields.push_back(field);
    return fields;
}

std::string FileParser::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n\"");
    if (start == std::string::npos) return "";

    size_t end = str.find_last_not_of(" \t\r\n\"");
    return str.substr(start, end - start + 1);
}

std::unordered_set<std::string> FileParser::extractSkills(const std::string& description) {
    std::unordered_set<std::string> skills;
    
    // List of known technical skills to look for
    std::vector<std::string> knownSkills = {
        "sql", "python", "java", "javascript", "c++", "c#", "r", "scala", "go", "rust",
        "power bi", "tableau", "excel", "pandas", "numpy", "tensorflow", "keras", "pytorch",
        "machine learning", "deep learning", "nlp", "computer vision", "mlops",
        "docker", "kubernetes", "aws", "azure", "gcp", "git", "jenkins", "ci/cd",
        "spring boot", "rest apis", "microservices", "system design", "agile", "scrum",
        "react", "angular", "vue", "node.js", "express", "mongodb", "postgresql", "mysql",
        "data cleaning", "reporting", "statistics", "data analysis", "data science",
        "hadoop", "spark", "kafka", "elasticsearch", "redis", "linux", "unix"
    };
    
    std::string lowerDesc = toLower(description);
    
    for (const auto& skill : knownSkills) {
        if (lowerDesc.find(toLower(skill)) != std::string::npos) {
            skills.insert(skill);
        }
    }
    
    // Also extract individual words that look like skills
    std::istringstream iss(lowerDesc);
    std::string word;
    while (iss >> word) {
        // Remove punctuation
        word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
            return std::ispunct(c);
        }), word.end());
        
        if (isSkillWord(word) && word.length() > 2) {
            skills.insert(word);
        }
    }
    
    return skills;
}

bool FileParser::isSkillWord(const std::string& word) {
    // Simple heuristic: technical words often have certain patterns
    std::string lower = toLower(word);
    
    // Skip common non-skill words
    std::unordered_set<std::string> commonWords = {
        "the", "and", "or", "but", "in", "on", "at", "to", "for", "of", "with", "by",
        "from", "up", "about", "into", "through", "during", "before", "after", "above",
        "below", "between", "among", "throughout", "within", "without", "toward", "under",
        "over", "across", "off", "away", "down", "out", "around", "back", "along", "near",
        "experienced", "professional", "skilled", "needed", "experience", "well", "try",
        "whom", "character", "truth", "store", "better", "drop", "agent", "give", "quickly",
        "system", "administration", "onto", "cold", "staff", "lawyer", "senior", "such",
        "scene", "prepare", "soon", "last", "remember", "general", "word", "what", "worry",
        "week", "responsibility", "attack", "half", "thought", "push", "campaign", "read",
        "laugh", "national", "scientist", "address", "all", "almost", "sea", "third", "rich",
        "cover", "decide", "always", "return", "democrat", "phone", "during", "necessary",
        "talk", "little", "occur", "might", "same", "bar", "job", "natural", "base", "stage",
        "manage", "economic", "say", "ball", "ready", "air", "indicate", "accept", "evidence",
        "agreement", "get", "military", "likely", "effort", "very", "make", "walk", "first",
        "consumer", "fall", "really", "away", "future", "age", "four", "expect", "class",
        "support", "blue", "tend", "inside", "development", "local", "seem", "number",
        "meeting", "whole", "grow", "fire", "property", "myself", "toward", "view",
        "commercial", "together", "animal", "son", "friend"
    };
    
    return commonWords.find(lower) == commonWords.end();
}

std::string FileParser::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Implementation of DataModels constructors
Resume::Resume(const std::string& id, const std::string& desc) 
    : id(id), description(desc) {
    skills = FileParser::extractSkills(desc);
}

JobDescription::JobDescription(const std::string& id, const std::string& desc) 
    : id(id), description(desc) {
    requiredSkills = FileParser::extractSkills(desc);
}

MatchResult::MatchResult(const std::string& resId, const std::string& jId, 
                        double score, const std::vector<std::string>& matched,
                        const std::vector<std::string>& missing)
    : resumeId(resId), jobId(jId), matchScore(score), 
      matchedSkills(matched), missingSkills(missing) {
}