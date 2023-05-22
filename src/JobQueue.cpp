#include "JobQueue.h"
#include "Job.h"
#include <string>

JobQueue::JobQueue(JobQueueSortType jobQueueSortType, std::string_view name_)
        : sortType(jobQueueSortType)
        , name(name_.data()) {}

/**
* Gets the memory required by every Job in the queue.
* @returns An integer, representing the total memory required.
*/
[[nodiscard]] int JobQueue::getTotalMemoryRequired() const {
    int mem = 0;
    for (auto job : this->queue) {
        mem += job.memoryRequired;
    }
    return mem;
}

/**
 * Gets the # of devices required by every Job in the queue.
 * @returns An integer, representing the total devices required.
 */
[[nodiscard]] int JobQueue::getTotalDevicesRequired() const {
    int dev = 0;
    for (auto job : this->queue) {
        dev += job.devicesRequired;
    }
    return dev;
}

/**
 * Removes the Job object at the front of the queue, and returns it.
 * @return The Job at the front most position
 */
Job JobQueue::pop() {
    Job job = this->queue.front();
    this->queue.pop_front();
    return job;
}

/**
 * Sorts the queue and returns the front most Job in the queue.
 * @returns The Job object at the front of the queue.
 */
Job JobQueue::peek() {
    this->sortJobs();
    return this->queue.front();
}

/**
 * Adds a job to the back of queue, and calls the sort function.
 * Sorting criteria varies based on queue type (some queues do not have sorting implemented at all).
 */
void JobQueue::push(Job job) {
    this->queue.push_back(job);
    this->sortJobs();
}

/**
 * Removes all jobs from the queue.
 */
void JobQueue::clear() {
    this->queue.clear();
}

/**
 * Removes true if there aren't any jobs in the queue.
 * @return True/False
 */
bool JobQueue::isEmpty() const{
    return this->queue.empty();
}

/**
 * A toString function for this queue.
 * @return Prints queue type and the jobs within. (Properties printed vary between queue types)
 */
JobQueue::operator std::string() const {
    std::string out = this->name;
    if (this->sortType == JobQueueSortType::NONE) {
        // cpu queue
        out = "Currently running job";
    }
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
        case JobQueueSortType::COMPLETE:
            out += " (COMPLETE)";
            break;
        case JobQueueSortType::NONE:
            // do nothing
            break;

    }
    out += ": {\n";

    for (auto job : this->queue) {
        //out += "  " + std::string{*it} + ",\n";
        if (this->sortType == JobQueueSortType::FIFO || this->sortType == JobQueueSortType::SJF) {
            out += "\tJob ID: " + std::to_string(job.id) + "\tRun Time: " + std::to_string(job.runningTime) + "\n";
        } else if  (this->sortType == JobQueueSortType::RR) {
            out += "\tJob ID: " + std::to_string(job.id) + "\tRun Time: " + std::to_string(job.runningTime) + "\tTime Accrued: " + std::to_string(job.currentTime) + "\n";
        } else if (this->sortType == JobQueueSortType::COMPLETE) { // complete queue
            out += "\tJob ID: " + std::to_string(job.id) + "\tArrival Time: " + std::to_string(job.arrivalTime) + "\tFinish Time: " + std::to_string(job.finishTime) + "\tTurnaround: " + std::to_string(job.finishTime - job.arrivalTime) + "\n";
        } else {
            out += "\tJob ID: " + std::to_string(job.id) + "\tTime Accrued: " + std::to_string(job.currentTime) + "\tTime Left: " + std::to_string(job.runningTime - job.currentTime) + "\n";
        }
    }
    out += "}";
    return out;
}

/**
 * This sorts all jobs in the queue based on its type.
 *
 * FIFO = arrival time (ascending)
 * SJF = running time (ascending)
 * COMPLETE = job ID (ascending)
 */
void JobQueue::sortJobs() { // this sorts all the jobs based on the type of queue
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

        case JobQueueSortType::COMPLETE:
            std::sort(jobs.begin(), jobs.end(), [](Job lhs, Job rhs) {
                return lhs.id < rhs.id; // sort by ascending order of job ID
            });
            break;

        case JobQueueSortType::RR:
            // I don't think Round-Robin sort is done here
        case JobQueueSortType::NONE:
            // do nothing
            break;
    }

    this->queue.clear();
    for (auto job : jobs) {
        this->queue.push_back(job);
    }
}

/**
 * This gets the sum of all jobs' turnaround times. Only works with the complete queue.
 * @returns The sum of the turnaround times
 */
int JobQueue::getTurnarounds() const {
    if (sortType != JobQueueSortType::COMPLETE) {
        return 0; // can't get turnarounds of incomplete jobs
    }

    int turnaroundSum = 0;
    for (auto job : this->queue) {
        turnaroundSum += job.finishTime - job.arrivalTime;
    }
    return turnaroundSum;
}

/**
 * This returns the number of Jobs in the queue.
 * @return Queue length
 */
int JobQueue::getNumJobs() const {
    return static_cast<int>(this->queue.size());
}
