#pragma once

#include <algorithm>
#include <list>
#include <string>
#include <string_view>
#include <vector>

#include "Job.h"

enum class JobQueueSortType {
    FIFO,
    SJF,
    RR,
    COMPLETE, // Admittedly this one doesn't make much sense as a sort type but it works well
    NONE
};

class JobQueue {
public:
    JobQueue(JobQueueSortType jobQueueSortType, std::string_view name_);
    JobQueue(const JobQueue& other) = default;
    JobQueue& operator=(const JobQueue& other) = default;
    JobQueue(JobQueue&& other) = default;
    JobQueue& operator=(JobQueue&& other) = default;

    [[nodiscard]] int getTotalMemoryRequired() const;
    [[nodiscard]] int getTotalDevicesRequired() const;
    [[nodiscard]] int getTotalDevicesHeld() const;

    Job pop();
    [[nodiscard]] Job peek();
    void push(Job job);
    void remove(int jobID);
    void clear();
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int getTurnarounds() const;
    [[nodiscard]] int getNumJobs() const;

    /// Used in for-each constructs
    [[nodiscard]] inline auto begin() const noexcept { return this->queue.begin(); }
    [[nodiscard]] inline auto begin() noexcept { return this->queue.begin(); }
    [[nodiscard]] inline auto end() const noexcept { return this->queue.end(); }
    [[nodiscard]] inline auto end() noexcept { return this->queue.end(); }

    explicit operator std::string() const;

protected:
    void sortJobs();

    std::list<Job> queue;
    JobQueueSortType sortType;
    std::string name;
};
