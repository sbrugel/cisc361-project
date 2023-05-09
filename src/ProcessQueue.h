//
// Created by Simon on 5/9/2023.
//

#pragma once

#include <list>
#include <string>

#include "Job.h"


class ProcessQueue {
public:
    explicit ProcessQueue(std::string n);
    void enqueue(Job job);
    Job dequeue();
    int getMemory();

    explicit operator std::string() const;
private:
    std::list<Job> queue;
    std::string name;
};
