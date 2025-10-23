#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

struct JobNode {
    string jobId;
    string jobDescription;
    JobNode* next;
};

struct ResumeNode {
    string resumeId;
    string resumeDescription;
    ResumeNode* next;
};

struct SkillWeightNode {
    string skill;
    double weight;
    SkillWeightNode* next;
};

struct TopMatch {
    string id;
    double score;
};

class SkillWeightLinkedList {
public:
    SkillWeightNode* head = NULL;
    void insertSkill(string skill, double weight) {
        SkillWeightNode* newNode = new SkillWeightNode{skill, weight, NULL};
        if (!head) head = newNode;
        else {
            SkillWeightNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }
};

class JobLinkedList {
public:
    JobNode* head = NULL;
    JobNode* tail = NULL;
    int size = 0;
};

class ResumeLinkedList {
public:
    ResumeNode* head = NULL;
    ResumeNode* tail = NULL;
    int size = 0;
};

void initializeSkillWeights(SkillWeightLinkedList& skillWeights) {
    skillWeights.insertSkill("data", 10);
    skillWeights.insertSkill("sql", 10);
    skillWeights.insertSkill("agile", 9);
    skillWeights.insertSkill("learning", 9);
    skillWeights.insertSkill("product", 9);
    skillWeights.insertSkill("python", 9);
    skillWeights.insertSkill("engineer", 8);
    skillWeights.insertSkill("management", 8);
    skillWeights.insertSkill("power", 8);
    skillWeights.insertSkill("machine", 8);
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
    skillWeights.insertSkill("cleaning", 6);
    skillWeights.insertSkill("rest", 6);
    skillWeights.insertSkill("tableau", 6);
    skillWeights.insertSkill("spring", 6);
    skillWeights.insertSkill("excel", 6);
    skillWeights.insertSkill("reporting", 6);
    skillWeights.insertSkill("statistics", 5);
    skillWeights.insertSkill("pandas", 5);
    skillWeights.insertSkill("nlp", 5);
    skillWeights.insertSkill("keras", 5);
    skillWeights.insertSkill("java", 8);
    skillWeights.insertSkill("javascript", 7);
    skillWeights.insertSkill("c++", 8);
    skillWeights.insertSkill("html", 6);
    skillWeights.insertSkill("css", 6);
    skillWeights.insertSkill("api", 6);
    skillWeights.insertSkill("leadership", 6);
    skillWeights.insertSkill("communication", 6);
    skillWeights.insertSkill("teamwork", 5);
    skillWeights.insertSkill("problem", 7);
    skillWeights.insertSkill("analytical", 7);
}

double calculateWeightedScore(string resume, string job, SkillWeightLinkedList& weights) {
    transform(resume.begin(), resume.end(), resume.begin(), ::tolower);
    transform(job.begin(), job.end(), job.begin(), ::tolower);

    double totalWeight = 0.0;
    double matchedWeight = 0.0;
    int jobSkillCount = 0;

    SkillWeightNode* current = weights.head;
    while (current) {
        totalWeight += current->weight;
        if (job.find(current->skill) != string::npos)
            jobSkillCount++;
        current = current->next;
    }

    current = weights.head;
    while (current) {
        bool jobHas = (job.find(current->skill) != string::npos);
        bool resumeHas = (resume.find(current->skill) != string::npos);
        if (jobHas && resumeHas)
            matchedWeight += current->weight;
        else if (jobHas && !resumeHas)
            matchedWeight -= current->weight * 0.3;
        current = current->next;
    }

    double score = (totalWeight > 0) ? (matchedWeight / totalWeight) * 100.0 : 0.0;
    if (jobSkillCount > 0 && matchedWeight >= totalWeight) score = 100.0;
    if (score < 0.0) score = 0.0;
    if (score > 100.0) score = 100.0;
    return score;
}

void loadJobs(JobLinkedList& jobs) {
    ifstream file("data/job_description.csv");
    if (!file.is_open()) { cerr << "Cannot open job_description.csv\n"; return; }
    string line; int count = 1;
    getline(file, line); // skip header
    while (getline(file, line)) {
        JobNode* node = new JobNode{"job_" + to_string(count++), line, NULL};
        if (!jobs.head) jobs.head = jobs.tail = node;
        else { jobs.tail->next = node; jobs.tail = node; }
        jobs.size++;
    }
    file.close();
}

void loadResumes(ResumeLinkedList& resumes) {
    ifstream file("data/resume.csv");
    if (!file.is_open()) { cerr << "Cannot open resume.csv\n"; return; }
    string line; int count = 1;
    getline(file, line); // skip header
    while (getline(file, line)) {
        ResumeNode* node = new ResumeNode{"resume_" + to_string(count++), line, NULL};
        if (!resumes.head) resumes.head = resumes.tail = node;
        else { resumes.tail->next = node; resumes.tail = node; }
        resumes.size++;
    }
    file.close();
}

void displayTopMatches(string baseId, string baseType, double scores[], string ids[], int total, int topN) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (scores[j] < scores[j] + 1) { /* keeps original style; not used now */ }
            if (scores[j] < scores[j + 1]) {
                swap(scores[j], scores[j + 1]);
                swap(ids[j], ids[j + 1]);
            }
        }
    }
    cout << "\nTop " << topN << " results for " << baseType << " " << baseId << ":\n";
    for (int i = 0; i < topN && i < total; i++) {
        if (scores[i] > 0)
            cout << setw(12) << ids[i] << " | Score: " << fixed << setprecision(2) << scores[i] << endl;
    }
}

// helper to maintain top 5
void keepTopFive(TopMatch top5[], int &count, string id, double score) {
    if (count < 5) {
        top5[count++] = {id, score};  
    } else {
        int minIndex = 0;
        for (int i = 1; i < 5; i++)
            if (top5[i].score < top5[minIndex].score) minIndex = i;
        if (score > top5[minIndex].score)
            top5[minIndex] = {id, score};
    }
}

// Sort TopMatch array by score (DESC). Tie-break by id (ASC) for stability.
void bubbleSortTop(TopMatch a[], int n){
    for (int i = 0; i < n - 1; ++i){
        for (int j = 0; j < n - 1 - i; ++j){
            if (a[j].score < a[j+1].score ||
               (a[j].score == a[j+1].score && a[j].id > a[j+1].id)){
                swap(a[j], a[j+1]);
            }
        }
    }
}

// Find Top 5 Jobs for a given Resume
void findTopJobsForResume(ResumeLinkedList &resumes, JobLinkedList &jobs,
                          SkillWeightLinkedList &skills, int resumeIndex) {
    clock_t start = clock();

    // navigate to target resume
    ResumeNode *resume = resumes.head;
    for (int i = 1; i < resumeIndex && resume; i++) resume = resume->next;
    if (!resume) {
        cout << "Resume not found.\n";
        return;
    }

    TopMatch top5[5];
    int count = 0;

    JobNode *job = jobs.head;
    while (job) {
        double score = calculateWeightedScore(resume->resumeDescription,
                                              job->jobDescription, skills);
        keepTopFive(top5, count, job->jobId, score);
        job = job->next;
    }

    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;

    cout << "\nExecution time: " << fixed << setprecision(2)
         << elapsed << " seconds\n";

    // sort before printing
    bubbleSortTop(top5, count);

    cout << "\nTop 5 results for Resume " << resume->resumeId << ":\n";
    for (int i = 0; i < count; i++)
        cout << "  " << top5[i].id << " | Score: " << top5[i].score << endl;
}

// Find Top 5 Resumes for a given Job
void findTopResumesForJob(ResumeLinkedList &resumes, JobLinkedList &jobs,
                          SkillWeightLinkedList &skills, int jobIndex) {
    clock_t start = clock();

    // navigate to target job
    JobNode *job = jobs.head;
    for (int i = 1; i < jobIndex && job; i++) job = job->next;
    if (!job) {
        cout << "Job not found.\n";
        return;
    }

    TopMatch top5[5];
    int count = 0;

    ResumeNode *resume = resumes.head;
    while (resume) {
        double score = calculateWeightedScore(resume->resumeDescription,
                                              job->jobDescription, skills);
        keepTopFive(top5, count, resume->resumeId, score);
        resume = resume->next;
    }

    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;

    cout << "\nExecution time: " << fixed << setprecision(2)
         << elapsed << " seconds\n";

    // sort before printing
    bubbleSortTop(top5, count);

    cout << "\nTop 5 results for Job " << job->jobId << ":\n";
    for (int i = 0; i < count; i++)
        cout << "  " << top5[i].id << " | Score: " << top5[i].score << endl;
}

void runPerformanceTest(JobLinkedList& jobs, ResumeLinkedList& resumes, SkillWeightLinkedList& skillWeights) {
    cout << "\n=== Performance Test ===\n";

    if (!jobs.head || !resumes.head) {
        cout << "Error: Please load job and resume data first.\n";
        return;
    }

    int jobsToTest = 10;
    double totalTime = 0.0;
    int totalMatches = 0;
    double totalSortTime = 0.0;

    auto estimateMemoryUsage = [](int jobCount, int resumeCount) -> double {
        return (jobCount + resumeCount) * 200.0 / 1024.0;
    };

    JobNode* jobPtr = jobs.head;
    int jobIndex = 0;

    while (jobPtr && jobIndex < jobsToTest) {
        clock_t start = clock();
        ResumeNode* resPtr = resumes.head;
        int matchCount = 0;

        TopMatch top5[5];
        int topCount = 0;

        while (resPtr) {
            double score = calculateWeightedScore(resPtr->resumeDescription, jobPtr->jobDescription, skillWeights);
            if (score > 0.0) matchCount++;
            keepTopFive(top5, topCount, resPtr->resumeId, score);
            resPtr = resPtr->next;
        }

        clock_t sortStart = clock();
        bubbleSortTop(top5, topCount);
        clock_t sortEnd = clock();
        double sortElapsed = double(sortEnd - sortStart) / CLOCKS_PER_SEC;
        totalSortTime += sortElapsed;

        clock_t end = clock();
        double elapsed = double(end - start) / CLOCKS_PER_SEC;
        totalTime += elapsed;
        totalMatches += matchCount;

        cout << "Job " << jobIndex + 1 << ": " << matchCount
             << " matches found in " << fixed << setprecision(2)
             << elapsed << " seconds\n";

        jobPtr = jobPtr->next;
        jobIndex++;
    }

    double averageTime = totalTime / jobsToTest;
    double memoryKB = estimateMemoryUsage(jobs.size, resumes.size);

    cout << "\nAverage matching time per job: " << fixed << setprecision(2)
         << averageTime << " seconds\n";
    cout << "Average bubble-sort time per job (Top-5): "
         << fixed << setprecision(4) << (totalSortTime / jobsToTest) << " seconds\n";
    cout << "Total memory used: ~" << fixed << setprecision(0)
         << memoryKB << " KB (estimated)\n";
}

int main() {
    JobLinkedList jobs;
    ResumeLinkedList resumes;
    SkillWeightLinkedList skillWeights;
    initializeSkillWeights(skillWeights);

    cout << "Loading job descriptions...\n";
    loadJobs(jobs);
    cout << "Loading resumes...\n";
    loadResumes(resumes);
    cout << "Data loaded successfully.\n";

    int choice;
    do {
        cout << "\n=== Job Matching System (Linked List Version) ===\n";
        cout << "1. Find Top 5 Jobs for a Resume\n";
        cout << "2. Find Top 5 Candidates for a Job\n";
        cout << "3. Run Performance Test\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int index;
            cout << "Enter Resume index (1-" << resumes.size << "): ";
            cin >> index;
            findTopJobsForResume(resumes, jobs, skillWeights, index);
        }
        else if (choice == 2) {
            int index;
            cout << "Enter Job index (1-" << jobs.size << "): ";
            cin >> index;
            findTopResumesForJob(resumes, jobs, skillWeights, index);
        }
        else if (choice == 3) runPerformanceTest(jobs, resumes, skillWeights);
        else if (choice == 4) cout << "Exiting system. Goodbye!\n";
        else cout << "Invalid choice. Try again.\n";

    } while (choice != 4);

    return 0;
}
