//
// Created by Simon on 5/2/2023.
//

#pragma once

#include <list>

#include "Job.h"

class HoldQueueFIFO {
    std::list<Job> queue;

public:
    void enqueue(Job j);
    void printQueue();
    // todo: implement sort
};

