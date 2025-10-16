#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <ctime>
#include <iomanip>

using namespace std;

string cleanWord(string word) {
    string cleaned = "";
    for (size_t i = 0; i < word.length(); i++) {
        char c = word[i];
        if (isalpha(c)) cleaned += tolower(c);
    }
    return cleaned;
}

/*node struct*/

/*Stores individual job postings as nodes in a linked list*/
struct JobNode {
    string jobDescription;
    string jobId;

    JobNode * next;
};

/*Stores individual candidate resumes as nodes*/
struct ResumeNode {
    string resumeDescription;
    string resumeId;

    ResumeNode * next;
};

/*Breaks down descriptions into individual words for analysis*/
struct WordNode {
    string word;
    int count;

    WordNode * next;
};

/*Stores the importance weight for each skill*/
struct SkillWeightNode {
    string skill;
    int weight;

    SkillWeightNode * next;
};

/*Stores the outcome of each job-resume comparison*/
struct MatchResultNode {
    string resumeId;
    string jobId;
    double score;

    MatchResultNode * next;
};

/*linked list class*/
class JobLinkedList {
public:
    JobNode * head;
    JobNode * tail;
    int size;

    JobLinkedList(){
        this -> size = 0;
        this -> head = NULL;
        this -> tail = NULL;
    }

    int getSize(){
        return size;
    };

    void insertAtBeginning(string value){
        /*create new node on a heap*/
        JobNode * newNode = new JobNode;
        newNode -> jobDescription = value;
        /*point new node's link to current head address*/
        newNode -> next = head;
        /*point head to new node*/
        head = newNode;
        size ++;

        if (tail == NULL){
            tail = newNode;
        }
    };

    void insertAtEnd(string value){
        JobNode * newNode = new JobNode;
        newNode -> jobDescription = value;
        /*point newNode reference to NULL*/
        newNode -> next = NULL;
        /*insert into an empty list*/
        if(head == NULL){
            head = tail = newNode;
        }
        else{
            tail -> next = newNode;
            tail = newNode;
        }
        size++;
    }

    void clear(){
        JobNode * current = head;
        while(head != NULL){
            /*Move current to the next node*/
            current = current -> next;
            delete head;
            head = current;
        }
        size = 0;
    }
};

class ResumeLinkedList {
public:
    ResumeNode * head;
    ResumeNode * tail;
    int size;

    ResumeLinkedList(){
        this -> size = 0;
        this -> head = NULL;
        this -> tail = NULL;
    }

    int getSize(){
        return size;
    };

    void insertAtBeginning(string value){
        /*create new node on a heap*/
        ResumeNode * newNode = new ResumeNode;
        newNode -> resumeDescription = value;
        /*point new node's link to current head address*/
        newNode -> next = head;
        /*point head to new node*/
        head = newNode;
        size ++;

        if (tail == NULL){
            tail = newNode;
        }
    };

    void insertAtEnd(string value){
        ResumeNode * newNode = new ResumeNode;
        newNode -> resumeDescription = value;
        /*point newNode reference to NULL*/
        newNode -> next = NULL;
        /*insert into an empty list*/
        if(head == NULL){
            head = tail = newNode;
        }
        else{
            tail -> next = newNode;
            tail = newNode;
        }
        size++;
    }

    void clear(){
        ResumeNode * current = head;
        while(head != NULL){
            /*Move current to the next node*/
            current = current -> next;
            delete head;
            head = current;
        }
        size = 0;
    }
};

/*store words along with frequency*/
class WordLinkedList{
public:
    WordNode * head;
    WordNode * tail;
    int size;

    WordLinkedList() {
        head = tail = NULL;
        size = 0;
    }

    WordNode * findWord(string w) {
        WordNode * current = head;
        while (current != NULL) {
            if (current -> word == w) 
            return current;

            current = current -> next;
        }
        return NULL;
    }

    /*insert new words OR increase count of the word*/
    void insertOrIncrement(string w) {
        WordNode * node = findWord(w);

        /*increase count if the word is found*/
        if (node != NULL) {
            node -> count++;
        } else {
            /*add new word to the link list if word not found*/
            WordNode * newNode = new WordNode;
            newNode -> word = w;
            newNode -> count = 1;

            /*insert at end*/
            newNode -> next = NULL;
            if (head == NULL) head = tail = newNode;
            else {
                tail -> next = newNode;
                tail = newNode;
            }
            size++;
        }
    }

    void clear() {
        WordNode * current = head;
        while (head != NULL) {
            current = head -> next;
            delete head;
            head = current;
        }
    size = 0;
    }
};

class SkillWeightLinkedList {
public:
    SkillWeightNode * head;
    SkillWeightNode * tail;
    int size;

    SkillWeightLinkedList() {
        head = tail = NULL;
        size = 0;
    }

    void insertSkill(string skill, int weight) {
        SkillWeightNode * newNode = new SkillWeightNode;
        newNode -> skill = skill;
        newNode -> weight = weight;
        newNode -> next = NULL;
        
        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail -> next = newNode;
            tail = newNode;
        }
        size++;
    }

    int getWeight(string skill) {
        SkillWeightNode * current = head;
        while (current != NULL) {
            if (current -> skill == skill) {
                return current -> weight;
            }
            current = current -> next;
        }
        return 1; // Default weight if skill not found
    }

    void clear() {
        SkillWeightNode * current = head;
        while (head != NULL) {
            current = head -> next;
            delete head;
            head = current;
        }
        size = 0;
    }
};

class MatchResultLinkedList {
public:
    MatchResultNode * head;
    MatchResultNode * tail;
    int size;

    MatchResultLinkedList() {
        head = tail = NULL;
        size = 0;
    }

    void insertResult(string resumeId, string jobId, double score) {
        MatchResultNode * newNode = new MatchResultNode;
        newNode -> resumeId = resumeId;
        newNode -> jobId = jobId;
        newNode -> score = score;
        newNode -> next = NULL;
        
        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    /*Bubble sort for sorting results by score (descending)*/
    void sortByScore() {
        /*if the list is empty*/
        if (head == NULL || head -> next == NULL) return;
        
        /*swap nodes to sort*/
        bool swapped;
        do {
            swapped = false;
            MatchResultNode * current = head;
            MatchResultNode * prev = NULL;
            
            while (current != NULL && current -> next != NULL) {
                if (current -> score < current -> next -> score) {
                    // Swap nodes
                    MatchResultNode* nextNode = current -> next;
                    current -> next = nextNode -> next;
                    nextNode -> next = current;
                    
                    if (prev == NULL) {
                        head = nextNode;
                    } else {
                        prev -> next = nextNode;
                    }
                    
                    if (current -> next == NULL) {
                        tail = current;
                    }
                    
                    prev = nextNode;
                    swapped = true;
                } else {
                    prev = current;
                    current = current -> next;
                }
            }
        } while (swapped);
    }

    void displayTopMatches(int limit) {
        sortByScore();
        MatchResultNode * current = head;
        int count = 0;
        int nonZero = 0;
        MatchResultNode* check = head;
        while (check != nullptr) {
            if (check->score > 0.0)
                nonZero++;
            check = check->next;
        }
        cout << "DEBUG → non-zero scores: " << nonZero << endl;
        
        cout << "\n=== TOP JOB MATCHES ===" << endl;
        while (current != NULL && count < limit) {
            cout << "Resume: " << current -> resumeId 
                 << " -> Job: " << current -> jobId 
                 << " | Score: " << current -> score << endl;
            current = current -> next;
            count++;
        }
    }

    void clear() {
        MatchResultNode* current = head;
        while (head != NULL) {
            current = head -> next;
            delete head;
            head = current;
        }
        size = 0;
    }
};

/*Function to extract words from text and build word frequency list*/
WordLinkedList extractWords(string text) {
    WordLinkedList wordList;
    stringstream ss(text);
    
    string word;
    
    while (ss >> word) {
        string cleaned = cleanWord(word);
        if (!cleaned.empty()) {
            wordList.insertOrIncrement(cleaned);
        }
    }
    return wordList;
}

double calculateWeightedScore(string resumeText, string jobText, SkillWeightLinkedList& skillWeights) {
    WordLinkedList resumeWords = extractWords(resumeText);
    WordLinkedList jobWords = extractWords(jobText);
    
    double totalScore = 0.0;
    double maxPossibleScore = 0.0;
    
    // Calculate score based on job requirements
    WordNode * jobCurrent = jobWords.head;
    while (jobCurrent != NULL) {
        string jobWord = jobCurrent -> word;
        WordNode* resumeWordNode = resumeWords.findWord(jobWord);
        
        if (resumeWordNode != NULL) {
            int weight = skillWeights.getWeight(jobWord);
            totalScore += (resumeWordNode -> count * weight);
        }
        maxPossibleScore += (jobCurrent -> count * skillWeights.getWeight(jobWord));
        jobCurrent = jobCurrent->next;
    }
    
    // Avoid division by zero
    if (maxPossibleScore == 0) return 0.0;
    
    // Normalize score to 0-100 range
    double normalizedScore = (totalScore / maxPossibleScore) * 100.0;
    
    resumeWords.clear();
    jobWords.clear();
    
    return normalizedScore;
}

// Initialize skill weights
void initializeSkillWeights(SkillWeightLinkedList& skillWeights) {
    // High frequency core skills (7000-5000 range)
    skillWeights.insertSkill("data", 10);
    skillWeights.insertSkill("sql", 10);
    
    // Very high demand skills (5000-4000 range)
    skillWeights.insertSkill("agile", 9);
    skillWeights.insertSkill("learning", 9);
    skillWeights.insertSkill("product", 9);
    skillWeights.insertSkill("python", 9);
    
    // Core engineering skills (4000-3000 range)
    skillWeights.insertSkill("engineer", 8);
    skillWeights.insertSkill("management", 8);
    skillWeights.insertSkill("power", 8);
    skillWeights.insertSkill("machine", 8);
    
    // Business and methodology skills (3000-2000 range)
    skillWeights.insertSkill("stories", 7);
    skillWeights.insertSkill("user", 7);
    skillWeights.insertSkill("stakeholder", 7);
    skillWeights.insertSkill("scrum", 7);
    skillWeights.insertSkill("roadmap", 7);
    skillWeights.insertSkill("deep", 7);
    skillWeights.insertSkill("design", 7);
    skillWeights.insertSkill("bi", 7);
    skillWeights.insertSkill("computer", 7);
    skillWeights.insertSkill("system", 7);
    
    // Data processing and tools (2000-2500 range)
    skillWeights.insertSkill("cleaning", 6);
    skillWeights.insertSkill("rest", 6);
    skillWeights.insertSkill("tableau", 6);
    skillWeights.insertSkill("spring", 6);
    skillWeights.insertSkill("excel", 6);
    skillWeights.insertSkill("reporting", 6);
    
    // Advanced technical skills (2500-2000 range)
    skillWeights.insertSkill("statistics", 5);
    skillWeights.insertSkill("pandas", 5);
    skillWeights.insertSkill("nlp", 5);
    skillWeights.insertSkill("keras", 5);
    
    // Additional common tech skills
    skillWeights.insertSkill("java", 8);
    skillWeights.insertSkill("javascript", 7);
    skillWeights.insertSkill("c++", 8);
    skillWeights.insertSkill("html", 6);
    skillWeights.insertSkill("css", 6);
    skillWeights.insertSkill("api", 6);
    
    // Soft skills
    skillWeights.insertSkill("leadership", 6);
    skillWeights.insertSkill("communication", 6);
    skillWeights.insertSkill("teamwork", 5);
    skillWeights.insertSkill("problem", 7);
    skillWeights.insertSkill("analytical", 7);
};

// Performance evaluation structure
struct PerformanceMetrics {
    long long matchingTime;
    size_t memoryUsed;
    int totalMatches;
};

PerformanceMetrics performJobMatching(JobLinkedList& jobs, ResumeLinkedList& resumes, 
                                    SkillWeightLinkedList& skillWeights, 
                                    MatchResultLinkedList& results) {
    PerformanceMetrics metrics;
    metrics.matchingTime = 0;
    metrics.totalMatches = 0;
    
    // Start timing
    clock_t startTime = clock();

    int totalComparisons = jobs.size * resumes.size;
    int completedComparisons = 0;
    
    cout << "Starting job matching..." << endl;
    cout << "Total comparisons to perform: " << totalComparisons << endl;
    cout << "Progress: ";
    
    // Perform matching for all combinations
    ResumeNode * resumeCurrent = resumes.head;
    int resumeCount = 0;

    while (resumeCurrent != NULL) {
        resumeCount++;
        JobNode * jobCurrent = jobs.head;
        int jobCount = 0;

        while (jobCurrent != NULL) {
            jobCount++;
            completedComparisons++;

            if (completedComparisons % 100 == 0 || completedComparisons == totalComparisons) {
                double progress = (double)completedComparisons / totalComparisons * 100.0;
                cout << "\rProgress: " << completedComparisons << "/" << totalComparisons 
                     << " (" << fixed << setprecision(1) << progress << "%)";
                cout.flush();
            }

            double score = calculateWeightedScore(
                resumeCurrent -> resumeDescription, 
                jobCurrent -> jobDescription, 
                skillWeights
            );
            
            results.insertResult(resumeCurrent -> resumeId, jobCurrent -> jobId, score);
            metrics.totalMatches++;
            
            jobCurrent = jobCurrent -> next;
        }

        // Show resume-level progress
        cout << "\nCompleted resume " << resumeCount << "/" << resumes.size 
             << " - " << resumeCurrent->resumeId;

        resumeCurrent = resumeCurrent -> next;
    }
    
    // End timing
    clock_t endTime = clock();
    metrics.matchingTime = (endTime - startTime) * 1000 / CLOCKS_PER_SEC; // Convert to milliseconds
    
    cout << "\nMatching completed!" << endl;
    
    // Estimate memory usage (simplified calculation)
    metrics.memoryUsed = sizeof(JobLinkedList) + sizeof(ResumeLinkedList) + 
                        sizeof(SkillWeightLinkedList) + sizeof(MatchResultLinkedList) +
                        (jobs.size * sizeof(JobNode)) + (resumes.size * sizeof(ResumeNode)) +
                        (results.size * sizeof(MatchResultNode));
    
    return metrics;
}

/*Reads job descriptions and resumes from files*/
void loadJobs(const string &filename, JobLinkedList &jobs) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        JobNode* newNode = new JobNode;
        newNode->jobDescription = line;                 // full text, keep commas
        newNode->jobId = "job_" + to_string(jobs.size + 1);
        newNode->next = nullptr;

        if (jobs.head == nullptr)
            jobs.head = jobs.tail = newNode;
        else {
            jobs.tail->next = newNode;
            jobs.tail = newNode;
        }
        jobs.size++;
    }
    file.close();
}

void loadResumes(const string &filename, ResumeLinkedList &resumes) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        ResumeNode* newNode = new ResumeNode;
        newNode->resumeDescription = line;              // full text, keep commas
        newNode->resumeId = "resume_" + to_string(resumes.size + 1);
        newNode->next = nullptr;

        if (resumes.head == nullptr)
            resumes.head = resumes.tail = newNode;
        else {
            resumes.tail->next = newNode;
            resumes.tail = newNode;
        }
        resumes.size++;
    }
    file.close();
}

void displayTopCandidatePerJob(MatchResultLinkedList& results, JobLinkedList& jobs) {
    cout << "\n=== TOP 3 CANDIDATES FOR EACH JOB ===\n";

    JobNode* job = jobs.head;
    while (job != NULL) {
        // collect all matches for this job into a temporary list
        MatchResultLinkedList temp;
        MatchResultNode* curr = results.head;
        while (curr != NULL) {
            if (curr->jobId == job->jobId)
                temp.insertResult(curr->resumeId, curr->jobId, curr->score);
            curr = curr->next;
        }

        if (temp.size > 0) {
            temp.sortByScore();
            cout << "\nJob: " << job->jobId << " → Top 3 Candidates:\n";
            cout << left << setw(12) << "Resume ID"
                 << setw(8)  << "Score" << endl;
            cout << string(20, '-') << endl;

            MatchResultNode* p = temp.head;
            int count = 0;
            while (p != NULL && count < 3) {
                cout << left << setw(12) << p->resumeId
                     << setw(8)  << fixed << setprecision(1) << p->score << endl;
                p = p->next;
                count++;
            }
        }
        job = job->next;
    }
}


void displayBestJobPerResume(MatchResultLinkedList& results, ResumeLinkedList& resumes) {
    cout << "\n=== TOP 3 JOBS FOR EACH RESUME ===\n";

    ResumeNode* res = resumes.head;
    while (res != NULL) {
        // collect all matches for this resume into a temporary list
        MatchResultLinkedList temp;
        MatchResultNode* curr = results.head;
        while (curr != NULL) {
            if (curr->resumeId == res->resumeId)
                temp.insertResult(curr->resumeId, curr->jobId, curr->score);
            curr = curr->next;
        }

        if (temp.size > 0) {
            temp.sortByScore();
            cout << "\nResume: " << res->resumeId << " → Top 3 Jobs:\n";
            cout << left << setw(12) << "Job ID"
                 << setw(8)  << "Score" << endl;
            cout << string(20, '-') << endl;

            MatchResultNode* p = temp.head;
            int count = 0;
            while (p != NULL && count < 3) {
                cout << left << setw(12) << p->jobId
                     << setw(8)  << fixed << setprecision(1) << p->score << endl;
                p = p->next;
                count++;
            }
        }
        res = res->next;
    }
}


int main(){
    JobLinkedList jobs;
    ResumeLinkedList resumes;
    SkillWeightLinkedList skillWeights;
    MatchResultLinkedList matchResults;

    cout << "Loading job descriptions and resumes..." << endl;
    loadJobs("data/job_description.csv", jobs);
    loadResumes("data/resume.csv", resumes);
    initializeSkillWeights(skillWeights);

    cout << "Jobs loaded: " << jobs.getSize() << endl;
    cout << "Resumes loaded: " << resumes.getSize() << endl;
    cout << "Skills with weights: " << skillWeights.size << endl;

    cout << "\nPerforming job matching with weighted scoring..." << endl;
    PerformanceMetrics metrics = performJobMatching(jobs, resumes, skillWeights, matchResults);

    //Display results
    matchResults.sortByScore();
    matchResults.displayTopMatches(10);
    displayTopCandidatePerJob(matchResults, jobs);
    displayBestJobPerResume(matchResults, resumes);

    //3. Performance summary (AFTER displaying everything)
    cout << "\n=== PERFORMANCE EVALUATION ===" << endl;
    cout << "Matching Time: " << fixed << setprecision(2) << metrics.matchingTime << " ms" << endl;
    cout << "Estimated Memory Used: " << metrics.memoryUsed << " bytes" << endl;
    cout << "Total Matches Calculated: " << metrics.totalMatches << endl;

    //4. Wait for user before program ends
    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get(); // pause until Enter pressed
    return 0;
}