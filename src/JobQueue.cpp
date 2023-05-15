#include "JobQueue.h"
#include <iostream>
#include <string>

JobQueue::JobQueue(JobQueueSortType jobQueueSortType, std::string_view name_)
        : sortType(jobQueueSortType)
        , name(name_.data()) {}

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
    std::string out = this->name;
    switch (this->sortType) {
        case JobQueueSortType::FIFO:
            out += " (FIFO)";
            break;
        case JobQueueSortType::SJF:
            out += " (SJF)";
            break;
        case JobQueueSortType::RR:
            if (name == "Ready Queue") {
                out += " (RQ)";
            } else {
                out += " (WQ)";
            }
            break;
    }
    out += ": {\n";

    for (auto it = this->queue.begin(); it != this->queue.end(); ++it) {
        //out += "  " + std::string{*it} + ",\n";
        if (this->sortType == JobQueueSortType::FIFO || this->sortType == JobQueueSortType::SJF) {
            out += "\tJob ID: " + std::to_string(it->id) + "\tRun Time: " + std::to_string(it->runningTime) + "\n";
        }
        else if  (this->sortType == JobQueueSortType::RR) {
            out += "\tJob ID: " + std::to_string(it->id) + "\tRun Time: " + std::to_string(it->runningTime) + "\tTime Accrued: " + std::to_string(it->currentTime) + "\n";
        }
        else { // complete queue
            out += "\tJob ID: " + std::to_string(it->id) + "\tArrival Time: " + std::to_string(it->arrivalTime) + "\tFinish Time: " + std::to_string(it->finishTime) + "\tTurnaround: " + std::to_string(it->finishTime - it->arrivalTime) + "\n";
        }
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
