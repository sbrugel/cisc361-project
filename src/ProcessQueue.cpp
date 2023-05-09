//
// Created by Simon on 5/9/2023.
// This is used for the Ready and Wait queues.
//

#include "ProcessQueue.h"

ProcessQueue::ProcessQueue(std::string n) {
    name = n;
}

void ProcessQueue::enqueue(Job job) {
    this->queue.push_back(job);
}

Job ProcessQueue::dequeue() {
    Job dequeued = this->queue.front();
    this->queue.pop_front();
    return dequeued;
}

int ProcessQueue::getMemory() {
    int mem;
    for (auto job : this->queue) {
        mem += job.memoryRequired;
    }
    return mem;
}

ProcessQueue::operator std::string() const {
    std::string out = std::string{name} + ": {\n";
    for (auto it = this->queue.begin(); it != this->queue.end(); ++it) {
        out += "  " + std::string{*it} + ",\n";
    }
    out += "}";
    return out;
}
