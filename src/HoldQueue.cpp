//
// Created by Simon on 5/3/2023.
//

#include <algorithm>
#include <vector>

#include "HoldQueue.h"

HoldQueue::HoldQueue(bool isPriority1) {
    isSJF = isPriority1;
}

void HoldQueue::enqueue(Job j) {
    queue.push_back(j);
    sortJobs();
}

void HoldQueue::printQueue() {
    std::list<Job>::iterator it;
    std::cout << "Contents of ";
    if (isSJF) {
        std::cout << "Hold Queue 1:" << std::endl;
    } else {
        std::cout << "Hold Queue 2:" << std::endl;
    }

    for (it = queue.begin(); it != queue.end(); ++it) {
        std::cout << *it;
    }
}

bool HoldQueue::compareJobsSJF(Job a, Job b) {
    if (a.runningTime == b.runningTime) return a.arrivalTime < b.arrivalTime; // tie breaker
    else return a.runningTime < b.runningTime; // sort by ascending order of running time
}

bool HoldQueue::compareJobsFIFO(Job a, Job b) {
    return a.arrivalTime < b.arrivalTime;
}

void HoldQueue::sortJobs() {
    // first put all jobs in a vector so we can sort them
    std::vector<Job> vectorJobs;
    for (auto & it : queue) {
        vectorJobs.push_back(it);
    }

    if (isSJF) {
        // sort jobs by running time
        std::sort(vectorJobs.begin(), vectorJobs.end(), compareJobsSJF);
    } else {
        // sort jobs by arrival (probably not needed?)
        std::sort(vectorJobs.begin(), vectorJobs.end(), compareJobsFIFO);
    }

    queue.clear();
    for (auto & vectorJob : vectorJobs) {
        queue.push_back(vectorJob);
    }
}

void HoldQueue::clearQueue() {
    queue.clear();
}
