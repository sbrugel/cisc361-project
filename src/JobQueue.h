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
};

class JobQueue {
public:
    JobQueue(JobQueueSortType jobQueueSortType, std::string_view name_);

    [[nodiscard]] int getTotalMemoryRequired() const;
    [[nodiscard]] int getTotalDevicesRequired() const;

    [[nodiscard]] Job pop();
    [[nodiscard]] Job peek();
    void push(Job job);
    void clear();

    explicit operator std::string() const;

protected:
    void sortJobs();

    std::list<Job> queue;
    JobQueueSortType sortType;
    std::string_view name;
};
