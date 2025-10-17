#ifndef JOB_MATCHING_ARRAY_H
#define JOB_MATCHING_ARRAY_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
#include <iomanip>  
#include <stdexcept> 

class DynamicArray {
private:
    std::string* data;
    int capacity;
    int size;

    void resize() {
        capacity *= 2;
        std::string* newData = new std::string[capacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    // Copy constructor
    DynamicArray(const DynamicArray& other) : capacity(other.capacity), size(other.size) {
        data = new std::string[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        // Self-assignment check
        if (this != &other) {
            // 1. Clean up old memory
            delete[] data; 

            // 2. Copy members
            capacity = other.capacity;
            size = other.size;

            // 3. Allocate new memory and copy elements
            data = new std::string[capacity];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Default constructor
    DynamicArray() : capacity(10), size(0) {
        data = new std::string[capacity];
    }

    // Destructor
    ~DynamicArray() {
        delete[] data;
    }

    void add(const std::string& item) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = item;
    }

    std::string& get(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const std::string& get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    int getSize() const {
        return size;
    }

    void clear() {
        delete[] data;
        capacity = 10;
        size = 0;
        data = new std::string[capacity];
    }
};

class JobMatchingSystem {
private:
    DynamicArray jobDescriptions;
    DynamicArray resumeDescriptions;
    
    // Common skills dictionary
    struct SkillCategory {
        std::string skill;
        std::string category;
        double weight;
    };
    
    SkillCategory skillCategories[100] = {
        // Data Analyst Skills
        {"sql", "data", 1.5},
        {"excel", "data", 1.3},
        {"power bi", "data", 1.4},
        {"tableau", "data", 1.4},
        {"data cleaning", "data", 1.2},
        {"data analysis", "data", 1.3},
        {"reporting", "data", 1.1},
        {"statistics", "data", 1.2},
        {"data visualization", "data", 1.3},
        {"dashboards", "data", 1.1},
        
        // Data Scientist Skills
        {"python", "data_science", 1.6},
        {"machine learning", "data_science", 2.0},
        {"deep learning", "data_science", 2.0},
        {"nlp", "data_science", 1.8},
        {"pandas", "data_science", 1.5},
        {"numpy", "data_science", 1.4},
        {"tensorflow", "data_science", 1.8},
        {"pytorch", "data_science", 1.8},
        {"keras", "data_science", 1.7},
        {"scikit-learn", "data_science", 1.6},
        
        // Software Engineering Skills
        {"java", "engineering", 1.5},
        {"spring boot", "engineering", 1.6},
        {"docker", "engineering", 1.4},
        {"kubernetes", "engineering", 1.5},
        {"rest apis", "engineering", 1.3},
        {"microservices", "engineering", 1.4},
        {"system design", "engineering", 1.5},
        {"git", "engineering", 1.1},
        {"agile", "engineering", 1.1},
        {"ci/cd", "engineering", 1.3},
        
        // Machine Learning Engineer Skills
        {"mlops", "ml_engineering", 1.6},
        {"computer vision", "ml_engineering", 1.8},
        {"cloud", "ml_engineering", 1.5},
        {"model deployment", "ml_engineering", 1.4},
        {"tensorflow serving", "ml_engineering", 1.5},
        {"pytorch lightning", "ml_engineering", 1.5},
        {"data pipelines", "ml_engineering", 1.3},

        // Product Manager Skills
        {"agile", "product_management", 1.3},
        {"scrum", "product_management", 1.3},
        {"stakeholder management", "product_management", 1.4},
        {"user stories", "product_management", 1.3},
        {"product roadmap", "product_management", 1.5},
        {"requirements gathering", "product_management", 1.2},
        {"ui/ux", "product_management", 1.1},
        {"communication", "product_management", 1.0},
        {"team leadership", "product_management", 1.0},

        // Soft Skills
        {"communication", "general", 1.0},
        {"time management", "general", 1.1},
        {"adaptability", "general", 1.0},
        {"project management", "general", 1.1},
        {"presentation", "general", 1.0},
        {"leadership", "general", 1.0},
        {"collaboration", "general", 1.0},
        {"problem solving", "general", 1.2}
    };

    struct SkillSynonym {
        std::string canonical;
        std::string variants[3]; // up to 3 synonyms
    };

    SkillSynonym skillSynonyms[10] = {
        {"rest apis", {"rest api", "restful apis", "restful api"}},
        {"mlops", {"machine learning ops", "ml operations"}},
        {"power bi", {"powerbi", "ms power bi"}},
        {"git", {"version control"}},
        {"excel", {"microsoft excel", "spreadsheets"}},
        {"ci/cd", {"continuous integration", "continuous delivery"}}
    };

    double calculateSkillWeight(const std::string& skill) {
        for (int i = 0; i < 100; i++) {
            if (skillCategories[i].skill == skill) {
                return skillCategories[i].weight;
            }
        }
        return 1.0; // Default weight for unmatched skills
    }

    DynamicArray extractSkills(const std::string& text) {
        DynamicArray skills;
        std::string lowercaseText = toLowercase(text);

        // Regular skill detection
        for (int i = 0; i < 100; i++) {
            std::string skill = skillCategories[i].skill;
            if (lowercaseText.find(skill) != std::string::npos && isValidSkillMatch(lowercaseText, skill)) {
                skills.add(skill);
            }
        }

        // Synonym-based detection
        for (int i = 0; i < 10; i++) {
            for (const std::string& variant : skillSynonyms[i].variants) {
                if (variant.empty()) break;
                if (lowercaseText.find(variant) != std::string::npos && isValidSkillMatch(lowercaseText, variant)) {
                    skills.add(skillSynonyms[i].canonical);
                    break;
                }
            }
        }

        return skills;
    }

    bool isValidSkillMatch(const std::string& text, const std::string& skill) {
        // Prevent partial matches like "ai" in "wait", "hair", "again"
        std::size_t pos = text.find(skill);
        
        if (pos == std::string::npos) return false;
        
        // Check character before the match
        if (pos > 0 && std::isalpha(text[pos - 1])) {
            return false; // Skill is part of a larger word
        }
        
        // Check character after the match
        std::size_t after = pos + skill.length();
        if (after < text.length() && std::isalpha(text[after])) {
            return false; // Skill is part of a larger word
        }
        
        return true;
    }

    double calculateWeightedScore(const DynamicArray& jobSkills, const DynamicArray& resumeSkills) {
        if (jobSkills.getSize() == 0) {
            return 0.0;
        }

        double totalMatchWeight = 0.0;
        double maxPossibleWeight = 0.0;
        int matchesFound = 0;

        // Calculate maximum possible weight based on job requirements
        for (int i = 0; i < jobSkills.getSize(); i++) {
            maxPossibleWeight += calculateSkillWeight(jobSkills.get(i));
        }

        if (maxPossibleWeight == 0) {
            return 0.0;
        }

        // Calculate matched weight
        for (int i = 0; i < jobSkills.getSize(); i++) {
            const std::string& jobSkill = jobSkills.get(i);
            bool found = false;
            
            for (int j = 0; j < resumeSkills.getSize(); j++) {
                if (jobSkill == resumeSkills.get(j)) {
                    totalMatchWeight += calculateSkillWeight(jobSkill);
                    matchesFound++;
                    found = true;
                    break;
                }
            }
            
            // Apply penalty for missing important skills
            if (!found) {
                double skillWeight = calculateSkillWeight(jobSkill);
                // Higher penalty for more important missing skills
                totalMatchWeight -= skillWeight * 0.3; // 30% penalty for missing skills
            }
        }

        // Calculate base score
        double baseScore = (totalMatchWeight / maxPossibleWeight) * 100.0;
        
        // Ensure score is within bounds
        baseScore = std::max(0.0, std::min(100.0, baseScore));
        
        // Apply bonus for having most required skills (but never reach 100% unless perfect match)
        double matchRatio = static_cast<double>(matchesFound) / jobSkills.getSize();
        if (matchRatio >= 0.8) {
            // Good match bonus, but cap at 95% for non-perfect matches
            baseScore = std::min(baseScore * 1.1, 95.0);
        }
        
        // Only give 100% for perfect matches (all skills present)
        if (matchesFound == jobSkills.getSize()) {
            return 100.0;
        }
        
        return baseScore;
    }

    std::string toLowercase(const std::string& text) {
        std::string result = text;
        for (char& c : result) {
            c = std::tolower(c);
        }
        return result;
    }

public:
    bool loadJobsFromCSV(const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cout << "Error: Cannot open file " << filename << std::endl;
                return false;
            }

            std::string line;
            // Skip header
            std::getline(file, line);
            
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    // Remove quotes if present
                    if (line.front() == '"' && line.back() == '"') {
                        line = line.substr(1, line.length() - 2);
                    }
                    jobDescriptions.add(line);
                }
            }
            
            file.close();
            std::cout << "Loaded " << jobDescriptions.getSize() << " job descriptions" << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cout << "Error loading jobs: " << e.what() << std::endl;
            return false;
        }
    }

    bool loadResumesFromCSV(const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cout << "Error: Cannot open file " << filename << std::endl;
                return false;
            }

            std::string line;
            // Skip header
            std::getline(file, line);
            
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    // Remove quotes if present
                    if (line.front() == '"' && line.back() == '"') {
                        line = line.substr(1, line.length() - 2);
                    }
                    resumeDescriptions.add(line);
                }
            }
            
            file.close();
            std::cout << "Loaded " << resumeDescriptions.getSize() << " resume descriptions" << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cout << "Error loading resumes: " << e.what() << std::endl;
            return false;
        }
    }

    void findBestMatches(int jobIndex, int topN = 5) {
        if (jobIndex < 0 || jobIndex >= jobDescriptions.getSize()) {
            std::cout << "Error: Invalid job index" << std::endl;
            return;
        }

        std::cout << "\n=== Finding Best Matches for Job " << jobIndex + 1 << " ===" << std::endl;
        std::cout << "Job Description: " << jobDescriptions.get(jobIndex) << std::endl;
        
        DynamicArray jobSkills = extractSkills(jobDescriptions.get(jobIndex));
        std::cout << "Required Skills: ";
        for (int i = 0; i < jobSkills.getSize(); i++) {
            std::cout << jobSkills.get(i);
            if (i < jobSkills.getSize() - 1) std::cout << ", ";
        }
        std::cout << std::endl;

        // Array to store matches
        struct Match {
            int resumeIndex;
            double score;
        };
        
        Match* matches = new Match[resumeDescriptions.getSize()];
        int matchCount = 0;

        clock_t startTime = clock();
        
        // Calculate scores for all resumes
        for (int i = 0; i < resumeDescriptions.getSize(); i++) {
            DynamicArray resumeSkills = extractSkills(resumeDescriptions.get(i));
            double score = calculateWeightedScore(jobSkills, resumeSkills);
            
            if (score > 0) {
                matches[matchCount].resumeIndex = i;
                matches[matchCount].score = score;
                matchCount++;
            }
        }

        // Sort matches using bubble sort (simple array sorting)
        for (int i = 0; i < matchCount - 1; i++) {
            for (int j = 0; j < matchCount - i - 1; j++) {
                if (matches[j].score < matches[j + 1].score) {
                    Match temp = matches[j];
                    matches[j] = matches[j + 1];
                    matches[j + 1] = temp;
                }
            }
        }

        clock_t endTime = clock();
        double duration = double(endTime - startTime) / CLOCKS_PER_SEC;

        // Display top N matches
        std::cout << "\nTop " << topN << " Matches:" << std::endl;
        std::cout << "==================" << std::endl;
        
        int displayCount = std::min(topN, matchCount);
        for (int i = 0; i < displayCount; i++) {
            std::cout << i + 1 << ". Resume " << matches[i].resumeIndex + 1 
                      << " - Score: " << std::fixed << std::setprecision(2) 
                      << matches[i].score << "%" << std::endl;
            std::cout << "   Skills: ";
            DynamicArray resumeSkills = extractSkills(resumeDescriptions.get(matches[i].resumeIndex));
            for (int j = 0; j < resumeSkills.getSize(); j++) {
                std::cout << resumeSkills.get(j);
                if (j < resumeSkills.getSize() - 1) std::cout << ", ";
            }
            std::cout << "\n   Preview: " 
                      << resumeDescriptions.get(matches[i].resumeIndex).substr(0, 100) 
                      << "..." << std::endl << std::endl;
        }

        std::cout << "Matching completed in " << duration << " seconds" << std::endl;
        std::cout << "Total resumes processed: " << resumeDescriptions.getSize() << std::endl;
        std::cout << "Total matches found: " << matchCount << std::endl;

        delete[] matches;
    }

    void performanceTest() {
        std::cout << "\n=== Performance Test ===" << std::endl;
        
        if (jobDescriptions.getSize() == 0 || resumeDescriptions.getSize() == 0) {
            std::cout << "Error: No data loaded for performance test" << std::endl;
            return;
        }

        // Test with first 10 jobs
        int testJobs = std::min(10, jobDescriptions.getSize());
        double totalTime = 0.0;

        for (int i = 0; i < testJobs; i++) {
            clock_t startTime = clock();
            
            DynamicArray jobSkills = extractSkills(jobDescriptions.get(i));
            int matchCount = 0;
            
            for (int j = 0; j < resumeDescriptions.getSize(); j++) {
                DynamicArray resumeSkills = extractSkills(resumeDescriptions.get(j));
                double score = calculateWeightedScore(jobSkills, resumeSkills);
                if (score > 50.0) { // Count matches above 50%
                    matchCount++;
                }
            }
            
            clock_t endTime = clock();
            double duration = double(endTime - startTime) / CLOCKS_PER_SEC;
            totalTime += duration;
            
            std::cout << "Job " << i + 1 << ": " << matchCount 
                      << " matches found in " << duration << " seconds" << std::endl;
        }

        std::cout << "\nAverage matching time per job: " << totalTime / testJobs << " seconds" << std::endl;
        std::cout << "Total memory used: ~" 
                  << (jobDescriptions.getSize() + resumeDescriptions.getSize()) * 1000 / 1024 
                  << " KB (estimated)" << std::endl;
    }

    int getJobCount() const {
        return jobDescriptions.getSize();
    }

    int getResumeCount() const {
        return resumeDescriptions.getSize();
    }
};

#endif