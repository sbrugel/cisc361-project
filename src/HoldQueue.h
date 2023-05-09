//
// Created by Simon on 5/3/2023.
//

#pragma once

#include <list>
#include <string>

#include "Job.h"

class HoldQueue {
public:
    explicit HoldQueue(bool isPriority1);
    void enqueue(Job job);
    void clearQueue();
    void sortJobs();

    explicit operator std::string() const;
private:
    bool isSJF;
    std::list<Job> queue;
};
