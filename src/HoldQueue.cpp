//
// Created by Simon on 5/3/2023.
//

#include <algorithm>
#include <vector>

#include "HoldQueue.h"

HoldQueue::HoldQueue(bool isPriority1)
        : isSJF(isPriority1) {}

void HoldQueue::enqueue(Job job) {
    this->queue.push_back(job);
    this->sortJobs();
}

void HoldQueue::sortJobs() {
    // first put all jobs in a vector so we can sort them
    std::vector<Job> jobs;
    for (auto job : this->queue) {
        jobs.push_back(job);
    }

    if (this->isSJF) {
        // sort jobs by running time
        std::sort(jobs.begin(), jobs.end(), [](Job lhs, Job rhs) {
            if (lhs.runningTime == rhs.runningTime)
                return lhs.arrivalTime < rhs.arrivalTime; // tie breaker
            else
                return lhs.runningTime < rhs.runningTime; // sort by ascending order of running time
        });
    } else {
        // sort jobs by arrival (probably not needed?)
        std::sort(jobs.begin(), jobs.end(), [](Job lhs, Job rhs) {
            return lhs.arrivalTime < rhs.arrivalTime;
        });
    }

    this->queue.clear();
    for (auto job : jobs) {
        this->queue.push_back(job);
    }
}

void HoldQueue::clearQueue() {
    this->queue.clear();
}

HoldQueue::operator std::string() const {
    std::string out = std::string{"Hold Queue "} + (this->isSJF ? '1' : '2') + ": {\n";
    for (auto it = this->queue.begin(); it != this->queue.end(); ++it) {
        out += "  " + std::string{*it} + ",\n";
    }
    out += "}";
    return out;
}
