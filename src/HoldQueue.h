//
// Created by Simon on 5/3/2023.
//

#pragma once

#include <list>

#include "Job.h"

class HoldQueue {
    bool isSJF;
    std::list<Job> queue;

    static bool compareJobsSJF(Job a, Job b);
    static bool compareJobsFIFO(Job a, Job b);

public:
    explicit HoldQueue(bool isPriority1);
    void enqueue(Job j);
    void printQueue();
    void sortJobs();
};