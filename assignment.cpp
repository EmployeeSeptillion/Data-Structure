
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

// singly linked list node for jobs
struct JobNode {
    string jobId;
    string jobDescription;
    JobNode* next;
};

// singly linked list node for resumes
struct ResumeNode {
    string resumeId;
    string resumeDescription;
    ResumeNode* next;
};

// singly linked list node for skillweight pairs with keyword scoring
struct SkillWeightNode {
    string skill;
    double weight;
    SkillWeightNode* next;
};

// tiny record used to store top 5 buffered records
struct TopMatch {
    string id;
    double score;
};

//for bubble-sort a linked list 
// small singly linked list node just for sorting/printing the top-5
struct TopMatchNode { 
    string id; 
    double score;
    TopMatchNode* next; };

// simple singly linked list holder for skills
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

// singly linked list holders for jobs and resumes
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

// bunch of skills with weights with keywords are lowercase on purpose
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

// simple keyword scoring:
// - lower both texts for normalization
// - totalWeight = sum of all skill weights
// - matchedWeight += w if job & resume both contain the skill
// - matchedWeight -= 0.3*w if job has it but resume doesnt have it (light penalty)
// - final score = (matchedWeight / totalWeight)*100, then clamped to [0..100]
// - bonus rule: if job mentions at least one skill and match or exceed total, push to 100
double calculateWeightedScore(string resume, string job, SkillWeightLinkedList& weights) {
    transform(resume.begin(), resume.end(), resume.begin(), ::tolower);
    transform(job.begin(), job.end(), job.begin(), ::tolower);

    double totalWeight = 0.0;
    double matchedWeight = 0.0;
    int jobSkillCount = 0;

    // linear search over the skills list
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

// load csv with skipping header then append nodes at tail
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

//just to keep the top 5 result
//idea is to replace current min with new score that beats it
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



// quick adapter: build a short singly linked list from the top-5 array
TopMatchNode* topArrayToList(TopMatch a[], int n){
    TopMatchNode* head = NULL;
    TopMatchNode** tail = &head;
    for(int i=0;i<n;i++){
        *tail = new TopMatchNode{a[i].id, a[i].score, NULL};
        tail = &((*tail)->next);
    }
    return head;
}

// classic bubble sort for singly linked list by swapping pointers, desc by score, stable by id
void bubbleSortListDesc(TopMatchNode*& head){
    if(!head) return;
    bool swapped;
    do{
        swapped = false;
        TopMatchNode *prev = NULL, *cur = head;
        while(cur && cur->next){
            if (cur->score < cur->next->score ||
               (cur->score == cur->next->score && cur->id > cur->next->id)){
                TopMatchNode* nxt = cur->next;
                cur->next = nxt->next; nxt->next = cur;
                if (prev) prev->next = nxt; else head = nxt;
                prev = nxt; swapped = true;
            } else {
                prev = cur; cur = cur->next;
            }
        }
    } while(swapped);
}

// clean up that tiny list
void freeTopList(TopMatchNode*& head){
    while(head){ TopMatchNode* d=head; head=head->next; delete d; }
}

// option 1: pick a resume then scan all jobs in linked list, then, store in top-5 array, and make it a tiny linked list, bubble sort it for output
void findTopJobsForResume(ResumeLinkedList &resumes, JobLinkedList &jobs,
                          SkillWeightLinkedList &skills, int resumeIndex) {

    // linear search to the target resume
    ResumeNode *resume = resumes.head;
    for (int i = 1; i < resumeIndex && resume; i++) resume = resume->next;
    if (!resume) {
        cout << "Resume not found.\n";
        return;
    }

    TopMatch top5[5];
    int count = 0;

    // count how much we scanned and how many non-zero scores we saw
    int scanned = 0, positive = 0;
    clock_t scanBeg = clock();

    JobNode *job = jobs.head;
    while (job) {
        double score = calculateWeightedScore(resume->resumeDescription,
                                              job->jobDescription, skills);
        keepTopFive(top5, count, job->jobId, score);
        if (score > 0.0) positive++;
        scanned++;
        job = job->next;
    }
    clock_t scanEnd = clock();

    // now do the linked-list bubble sort just for these 5 which shows the sorting
    clock_t sortBeg = clock();
    TopMatchNode* lst = topArrayToList(top5, count);
    bubbleSortListDesc(lst);
    clock_t sortEnd = clock();

    cout.setf(std::ios::fixed);
    cout << setprecision(2);

    cout << "\nTop 5 results for Resume " << resume->resumeId << ":\n";
    int printed = 0;
    for(TopMatchNode* p = lst; p && printed < count; p = p->next, ++printed)
        cout << "  " << p->id << " | Score: " << p->score << endl;

    cout << "Scanned jobs: " << scanned
         << "  | Positive matches (>0): " << positive << "\n";
    cout << "Time (scan): " << double(scanEnd - scanBeg) / CLOCKS_PER_SEC << " s\n";
    cout << "Time (bubble sort Top-5, linked list): "
         << double(sortEnd - sortBeg) / CLOCKS_PER_SEC << " s\n";

    freeTopList(lst);
}

// option 2: pick a job then scan all resumes in linked list, then, store in top-5 array, and make it a tiny linked list, bubble sort it for output
void findTopResumesForJob(ResumeLinkedList &resumes, JobLinkedList &jobs,
                          SkillWeightLinkedList &skills, int jobIndex) {

    // linear search to the target job
    JobNode *job = jobs.head;
    for (int i = 1; i < jobIndex && job; i++) job = job->next;
    if (!job) {
        cout << "Job not found.\n";
        return;
    }

    TopMatch top5[5];
    int count = 0;

    int scanned = 0, positive = 0;
    clock_t scanBeg = clock();

    ResumeNode *resume = resumes.head;
    while (resume) {
        double score = calculateWeightedScore(resume->resumeDescription,
                                              job->jobDescription, skills);
        keepTopFive(top5, count, resume->resumeId, score);
        if (score > 0.0) positive++;
        scanned++;
        resume = resume->next;
    }
    clock_t scanEnd = clock();

    clock_t sortBeg = clock();
    TopMatchNode* lst = topArrayToList(top5, count);
    bubbleSortListDesc(lst);
    clock_t sortEnd = clock();

    cout.setf(std::ios::fixed);
    cout << setprecision(2);

    cout << "\nTop 5 results for Job " << job->jobId << ":\n";
    int printed = 0;
    for(TopMatchNode* p = lst; p && printed < count; p = p->next, ++printed)
        cout << "  " << p->id << " | Score: " << p->score << endl;

    cout << "Scanned resumes: " << scanned
         << "  | Positive matches (>0): " << positive << "\n";
    cout << "Time (scan): " << double(scanEnd - scanBeg) / CLOCKS_PER_SEC << " s\n";
    cout << "Time (bubble sort Top-5, linked list): "
         << double(sortEnd - sortBeg) / CLOCKS_PER_SEC << " s\n";

    freeTopList(lst);
}

// option 3: quick performance run — first 10 jobs × scan all resumes.
// keeping a per-job top-5 and time the linked-list bubble sort on that tiny list.
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

        // linear scan over all resumes for this job
        while (resPtr) {
            double score = calculateWeightedScore(resPtr->resumeDescription, jobPtr->jobDescription, skillWeights);
            if (score > 0.0) matchCount++;
            keepTopFive(top5, topCount, resPtr->resumeId, score);
            resPtr = resPtr->next;
        }

        // time the linked-list bubble sort (top-5 only)
        clock_t sortStart = clock();
        TopMatchNode* lst = topArrayToList(top5, topCount);
        bubbleSortListDesc(lst);
        freeTopList(lst);
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
    cout << "Average bubble-sort time per job (Top-5, linked list): "
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
