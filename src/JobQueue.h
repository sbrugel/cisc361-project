//
// Created by Simon on 5/3/2023.
//

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
    NONE
};

class JobQueue {
public:
    JobQueue(JobQueueSortType jobQueueSortType, std::string_view name_);

    [[nodiscard]] int getTotalMemoryRequired() const;
    [[nodiscard]] int getTotalDevicesRequired() const;

    Job pop();
    [[nodiscard]] Job peek();
    void push(Job job);
    void clear();
    bool isEmpty();
    void push_back(Job job);
    Job dequeue_front();

        explicit operator std::string() const;

    std::list<Job> queue;
protected:
    void sortJobs();

    JobQueueSortType sortType;
    std::string name;
};
