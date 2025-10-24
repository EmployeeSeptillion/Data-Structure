
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

// single linked list node for jobs
struct JobNode {
    string jobId;
    string jobDescription;
    JobNode* next;
};

// single linked list node for resumes
struct ResumeNode {
    string resumeId;
    string resumeDescription;
    ResumeNode* next;
};

// single linked list node for skillweight pairs
struct SkillWeightNode {
    string skill;
    double weight;
    SkillWeightNode* next;
};

// single linked list node for a scored result
struct TopMatchNode {
    string id;
    double score;
    TopMatchNode* next;
};

// simple list for skills
class SkillWeightLinkedList {
public:
    SkillWeightNode* head = NULL;
    void insertSkill(string skill, double weight) {
        SkillWeightNode* n = new SkillWeightNode{skill, weight, NULL};
        if (!head) head = n;
        else {
            SkillWeightNode* t = head;
            while (t->next) t = t->next;
            t->next = n;
        }
    }
};

// simple lists for jobs and resumes
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

// a bunch of skills set with score
void initializeSkillWeights(SkillWeightLinkedList& w) {
    w.insertSkill("data", 10);      w.insertSkill("sql", 10);
    w.insertSkill("agile", 9);      w.insertSkill("learning", 9);
    w.insertSkill("product", 9);    w.insertSkill("python", 9);
    w.insertSkill("engineer", 8);   w.insertSkill("management", 8);
    w.insertSkill("power", 8);      w.insertSkill("machine", 8);
    w.insertSkill("stories", 7);    w.insertSkill("user", 7);
    w.insertSkill("stakeholder", 7);w.insertSkill("scrum", 7);
    w.insertSkill("roadmap", 7);    w.insertSkill("deep", 7);
    w.insertSkill("design", 7);     w.insertSkill("bi", 7);
    w.insertSkill("computer", 7);   w.insertSkill("system", 7);
    w.insertSkill("cleaning", 6);   w.insertSkill("rest", 6);
    w.insertSkill("tableau", 6);    w.insertSkill("spring", 6);
    w.insertSkill("excel", 6);      w.insertSkill("reporting", 6);
    w.insertSkill("statistics", 5); w.insertSkill("pandas", 5);
    w.insertSkill("nlp", 5);        w.insertSkill("keras", 5);
    w.insertSkill("java", 8);       w.insertSkill("javascript", 7);
    w.insertSkill("c++", 8);        w.insertSkill("html", 6);
    w.insertSkill("css", 6);        w.insertSkill("api", 6);
    w.insertSkill("leadership", 6); w.insertSkill("communication", 6);
    w.insertSkill("teamwork", 5);   w.insertSkill("problem", 7);
    w.insertSkill("analytical", 7);
}

//keyword scoring linked-list traversal
double calculateWeightedScore(string resume, string job, SkillWeightLinkedList& weights) {
    transform(resume.begin(), resume.end(), resume.begin(), ::tolower);
    transform(job.begin(), job.end(), job.begin(), ::tolower);

    double totalWeight = 0.0;
    double matchedWeight = 0.0;
    int jobSkillCount = 0;

    //  totals and which skills appear in job
    for (SkillWeightNode* p = weights.head; p; p = p->next) {
        totalWeight += p->weight;
        if (job.find(p->skill) != string::npos) jobSkillCount++;
    }

    // matches + small penalty for job-only skills
    for (SkillWeightNode* p = weights.head; p; p = p->next) {
        bool jobHas = (job.find(p->skill) != string::npos);
        if (!jobHas) continue;
        bool resHas = (resume.find(p->skill) != string::npos);
        if (resHas) matchedWeight += p->weight;
        else matchedWeight -= p->weight * 0.3;
    }

    if (totalWeight <= 0.0) return 0.0;
    double score = (matchedWeight / totalWeight) * 100.0;
    if (jobSkillCount > 0 && matchedWeight >= totalWeight) score = 100.0;
    if (score < 0.0) score = 0.0;
    if (score > 100.0) score = 100.0;
    return score;
}

// load job and resume csv, skipping header row
void loadJobs(JobLinkedList& jobs) {
    ifstream f("data/job_description.csv");
    if (!f.is_open()) { cerr << "cannot open job_description.csv\n"; return; }
    string line; int idx = 1;
    getline(f, line); // header
    while (getline(f, line)) {
        JobNode* n = new JobNode{"job_" + to_string(idx++), line, NULL};
        if (!jobs.head) jobs.head = jobs.tail = n;
        else { jobs.tail->next = n; jobs.tail = n; }
        jobs.size++;
    }
    f.close();
}

void loadResumes(ResumeLinkedList& resumes) {
    ifstream f("data/resume.csv");
    if (!f.is_open()) { cerr << "cannot open resume.csv\n"; return; }
    string line; int idx = 1;
    getline(f, line); // header
    while (getline(f, line)) {
        ResumeNode* n = new ResumeNode{"resume_" + to_string(idx++), line, NULL};
        if (!resumes.head) resumes.head = resumes.tail = n;
        else { resumes.tail->next = n; resumes.tail = n; }
        resumes.size++;
    }
    f.close();
}

// build a full results linked list
void pushResult(TopMatchNode*& head, TopMatchNode*& tail, const string& id, double score){
    TopMatchNode* n = new TopMatchNode{ id, score, NULL };
    if (!head) { head = tail = n; } else { tail->next = n; tail = n; }
}

// bubble sort the linked list, desc by score
void bubbleSortListDesc(TopMatchNode*& head){
    if (!head) return;
    bool swapped;
    do {
        swapped = false;
        TopMatchNode *prev = NULL, *cur = head;
        while (cur && cur->next) {
            bool outOfOrder = (cur->score < cur->next->score) ||
                              (cur->score == cur->next->score && cur->id > cur->next->id);
            if (outOfOrder) {
                TopMatchNode* nxt = cur->next;
                cur->next = nxt->next; nxt->next = cur;
                if (prev) prev->next = nxt; else head = nxt;
                prev = nxt; swapped = true;
            } else {
                prev = cur; cur = cur->next;
            }
        }
    } while (swapped);
}

// free a results list
void freeTopList(TopMatchNode*& head){
    while (head) { TopMatchNode* d = head; head = head->next; delete d; }
}

// option 1: selected resume vs all jobs, bubble sort, then prints top 5 results
void findTopJobsForResume(ResumeLinkedList &resumes, JobLinkedList &jobs,
                          SkillWeightLinkedList &skills, int resumeIndex) {

    // find target resume by linear search
    ResumeNode* r = resumes.head;
    for (int i = 1; i < resumeIndex && r; i++) r = r->next;
    if (!r) { cout << "resume not found.\n"; return; }

    int scanned = 0, positive = 0;
    clock_t scanBeg = clock();

    TopMatchNode* lst = NULL;
    TopMatchNode* tail = NULL;

    for (JobNode* j = jobs.head; j; j = j->next) {
        double s = calculateWeightedScore(r->resumeDescription, j->jobDescription, skills);
        pushResult(lst, tail, j->jobId, s);
        if (s > 0.0) positive++;
        scanned++;
    }
    clock_t scanEnd = clock();

    clock_t sortBeg = clock();
    bubbleSortListDesc(lst);        // full-list sort
    clock_t sortEnd = clock();

    cout.setf(std::ios::fixed);
    cout << setprecision(2);

    cout << "\nTop 5 results for Resume " << r->resumeId << ":\n";
    int shown = 0;
    for (TopMatchNode* p = lst; p && shown < 5; p = p->next, ++shown)
        cout << "  " << p->id << " | Score: " << p->score << "\n";

    cout << "Scanned jobs: " << scanned
         << "  | Positive matches (>0): " << positive << "\n";
    cout << "Time (scan): " << double(scanEnd - scanBeg)/CLOCKS_PER_SEC << " s\n";
    cout << "Time (bubble sort full list): " << double(sortEnd - sortBeg)/CLOCKS_PER_SEC << " s\n";

    freeTopList(lst);
}

// option 2: selected job vs all resume, bubble sort, then prints top 5 results
void findTopResumesForJob(ResumeLinkedList &resumes, JobLinkedList &jobs,
                          SkillWeightLinkedList &skills, int jobIndex) {

    // find target job by linear search
    JobNode* j = jobs.head;
    for (int i = 1; i < jobIndex && j; i++) j = j->next;
    if (!j) { cout << "job not found.\n"; return; }

    int scanned = 0, positive = 0;
    clock_t scanBeg = clock();

    TopMatchNode* lst = NULL;
    TopMatchNode* tail = NULL;

    for (ResumeNode* r = resumes.head; r; r = r->next) {
        double s = calculateWeightedScore(r->resumeDescription, j->jobDescription, skills);
        pushResult(lst, tail, r->resumeId, s);
        if (s > 0.0) positive++;
        scanned++;
    }
    clock_t scanEnd = clock();

    clock_t sortBeg = clock();
    bubbleSortListDesc(lst);        // full-list sort 
    clock_t sortEnd = clock();

    cout.setf(std::ios::fixed);
    cout << setprecision(2);

    cout << "\nTop 5 results for Job " << j->jobId << ":\n";
    int shown = 0;
    for (TopMatchNode* p = lst; p && shown < 5; p = p->next, ++shown)
        cout << "  " << p->id << " | Score: " << p->score << "\n";

    cout << "Scanned resumes: " << scanned
         << "  | Positive matches (>0): " << positive << "\n";
    cout << "Time (scan): " << double(scanEnd - scanBeg)/CLOCKS_PER_SEC << " s\n";
    cout << "Time (bubble sort full list): " << double(sortEnd - sortBeg)/CLOCKS_PER_SEC << " s\n";

    freeTopList(lst);
}

// option 3: performance test, 10 jobs × all resumes
void runPerformanceTest(JobLinkedList& jobs, ResumeLinkedList& resumes, SkillWeightLinkedList& skills) {
    cout << "\n=== Performance Test (full linked-list sort) ===\n";
    if (!jobs.head || !resumes.head) { cout << "no data loaded.\n"; return; }

    int jobsToTest = 10;
    double totalScan = 0.0, totalSort = 0.0, totalAll = 0.0;

    JobNode* j = jobs.head;
    for (int ji = 1; ji <= jobsToTest && j; ++ji, j = j->next) {
        clock_t scanBeg = clock();

        TopMatchNode* lst = NULL;
        TopMatchNode* tail = NULL;

        int matches = 0;
        for (ResumeNode* r = resumes.head; r; r = r->next) {
            double s = calculateWeightedScore(r->resumeDescription, j->jobDescription, skills);
            pushResult(lst, tail, r->resumeId, s);
            if (s > 0.0) matches++;
        }

        clock_t scanEnd = clock();
        double scanTime = double(scanEnd - scanBeg) / CLOCKS_PER_SEC;

        clock_t sortBeg = clock();
        bubbleSortListDesc(lst);                  // full-list sort
        clock_t sortEnd = clock();
        double sortTime = double(sortEnd - sortBeg) / CLOCKS_PER_SEC;

        // touch top 5 (optional read)
        int shown = 0; for (TopMatchNode* p = lst; p && shown < 5; p = p->next, ++shown) {}

        freeTopList(lst);

        double total = scanTime + sortTime;
        totalScan += scanTime; totalSort += sortTime; totalAll += total;

        cout << "job " << ji << ": " << matches
             << " matches | scan " << fixed << setprecision(2) << scanTime
             << " s | sort " << sortTime << " s | total "
             << setprecision(2) << total << " s\n";
    }

    cout << "\navg scan per job: " << fixed << setprecision(2) << (totalScan / jobsToTest) << " s\n";
    cout << "avg full-list bubble sort per job: " << fixed << setprecision(2) << (totalSort / jobsToTest) << " s\n";
    cout << "avg total per job: " << fixed << setprecision(2) << (totalAll / jobsToTest) << " s\n";

    // classic estimate line (same style as your screenshot)
    auto estimateMemoryKB = [](int jobCount, int resumeCount) -> double {
        return (jobCount + resumeCount) * 200.0 / 1024.0; // very rough: ~200 bytes/record
    };
    cout << "Total memory used: ~" << fixed << setprecision(0)
         << estimateMemoryKB(jobs.size, resumes.size) << " KB (estimated)\n";
}



int main() {
    JobLinkedList jobs;
    ResumeLinkedList resumes;
    SkillWeightLinkedList skills;
    initializeSkillWeights(skills);

    cout << "loading job descriptions...\n";
    loadJobs(jobs);
    cout << "loading resumes...\n";
    loadResumes(resumes);
    cout << "data loaded. jobs=" << jobs.size << " resumes=" << resumes.size << "\n";

    int choice;
    do {
        cout << "\n=== job matching system (linked list version) ===\n";
        cout << "1. top 5 jobs for a resume (full-list sort)\n";
        cout << "2. top 5 candidates for a job (full-list sort)\n";
        cout << "3. run performance test (10 x all resumes, full-list sort)\n";
        cout << "4. exit\n";
        cout << "enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int idx;
            cout << "enter resume index (1-" << resumes.size << "): ";
            cin >> idx;
            findTopJobsForResume(resumes, jobs, skills, idx);
        } else if (choice == 2) {
            int idx;
            cout << "enter job index (1-" << jobs.size << "): ";
            cin >> idx;
            findTopResumesForJob(resumes, jobs, skills, idx);
        } else if (choice == 3) {
            runPerformanceTest(jobs, resumes, skills);
        } else if (choice == 4) {
            cout << "bye\n";
        } else {
            cout << "invalid choice\n";
        }
    } while (choice != 4);

    return 0;
}
