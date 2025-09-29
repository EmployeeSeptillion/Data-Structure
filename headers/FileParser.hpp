#pragma once
#include "DataModels.hpp"
#include "ArrayContainer.hpp"
#include <string>
#include <vector>
#include <unordered_set>

class FileParser {
public:
    static bool parseResumeCSV(const std::string& filename, ArrayContainer<Resume>& resumes);
    static bool parseJobDescriptionCSV(const std::string& filename, ArrayContainer<JobDescription>& jobDescriptions);
    
    static std::unordered_set<std::string> extractSkills(const std::string& description); // <-- public

    // other public helpers if needed
private:
    static std::vector<std::string> splitCSVLine(const std::string& line);
    static std::string trim(const std::string& str);
    static bool isSkillWord(const std::string& word);
    static std::string toLower(const std::string& str);
};
