#include "JobQueue.h"
#include <iostream>

JobQueue::JobQueue(JobQueueSortType jobQueueSortType, std::string_view name_)
        : sortType(jobQueueSortType)
        , name(name_) {}

[[nodiscard]] int JobQueue::getTotalMemoryRequired() const {
    int mem = 0;
    for (auto job : this->queue) {
        mem += job.memoryRequired;
    }
    return mem;
}

[[nodiscard]] int JobQueue::getTotalDevicesRequired() const {
    int dev = 0;
    for (auto job : this->queue) {
        dev += job.devicesRequired;
    }
    return dev;
}

Job JobQueue::pop() {
    this->sortJobs();
    Job job = this->queue.front();
    this->queue.pop_front();
    return job;
}

Job JobQueue::peek() {
    this->sortJobs();
    return this->queue.front();
}

void JobQueue::push(Job job) {
    this->queue.push_back(job);
    this->sortJobs();
}

void JobQueue::clear() {
    this->queue.clear();
}

JobQueue::operator std::string() const {
    std::string out = this->name.data();
    switch (this->sortType) {
        case JobQueueSortType::FIFO:
            out = " (FIFO)";
            break;
        case JobQueueSortType::SJF:
            out = " (SJF)";
            break;
        case JobQueueSortType::RR:
            out = " (RR)";
            break;
    }
    out += ": {\n";

    if (this->sortType == JobQueueSortType::FIFO) {
        std::cout << "fifo job" << std::endl;
    }
    else if (this->sortType == JobQueueSortType::SJF) {
        std::cout << "sjf job" << std::endl;
    }
    else if  (this->sortType == JobQueueSortType::RR) {
        if (name == "Ready Queue") {
            std::cout << "ready queue job" << std::endl;
        } else {
            std::cout << "wait queue job" << std::endl;
        }
    } else { // complete queue
        std::cout << "complete job" << std::endl;
    }

    for (auto it = this->queue.begin(); it != this->queue.end(); ++it) {
        //out += "  " + std::string{*it} + ",\n";

    }
    out += "}";
    return out;
}

void JobQueue::sortJobs() {
    // first put all jobs in a vector so we can sort them
    std::vector<Job> jobs;
    for (auto job : this->queue) {
        jobs.push_back(job);
    }

    // Sort based on type
    switch (this->sortType) {
        case JobQueueSortType::FIFO:
            // sort jobs by arrival (probably not needed?)
            std::sort(jobs.begin(), jobs.end(), [](Job lhs, Job rhs) {
                return lhs.arrivalTime < rhs.arrivalTime;
            });
            break;
        case JobQueueSortType::SJF:
            // sort jobs by running time
            std::sort(jobs.begin(), jobs.end(), [](Job lhs, Job rhs) {
                if (lhs.runningTime == rhs.runningTime)
                    return lhs.arrivalTime < rhs.arrivalTime; // tie breaker
                else
                    return lhs.runningTime < rhs.runningTime; // sort by ascending order of running time
            });
            break;
        case JobQueueSortType::RR:
            // todo: rr sort
            break;
    }

    this->queue.clear();
    for (auto job : jobs) {
        this->queue.push_back(job);
    }
}
